#include <gtk/gtk.h>

#include <stdio.h>
#include <time.h>

#include <tclogo/tclogo.h>
#include <tclogo/node.h>
#include <tclogo/group.h>

#include <pthread.h>

#include "tclogo-app.h"
#include "tclogo-app-win.h"
#include "panic.h"

struct _TclogoApp
{
    GtkApplication   parent;
    TclogoAppWindow *window;
    struct logo     *logo;
    cairo_surface_t *surface;
};

G_DEFINE_TYPE(TclogoApp, tclogo_app, GTK_TYPE_APPLICATION);

static TclogoApp *_app;

static void
draw_callback()
{
    tclogo_app_window_draw_surface(_app->window, _app->surface);
    
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 500000000L; // 0.5 secs
    
    nanosleep(&t, NULL);
}

static void *
draw_thread(void *_app)
{
    TclogoApp *app = (TclogoApp *) _app;
    
    struct group *root = logo_get_root(app->logo);
    app->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                              group_width(root),
                                              group_height(root));
    
    cairo_t *cr = cairo_create(app->surface);
    logo_draw(app->logo, cr, draw_callback);
    
    return NULL;
}

static void
tclogo_app_execute(TclogoApp  *app,
                   const char *filename)
{
    struct node *root;
    
    app->logo = logo_new();
        
    if (parse_file(filename, &root) < 0) {
        panic("parse_file()");
    }
    
    logo_execute(app->logo, root);
    
    pthread_t thread;
    pthread_create(&thread, NULL, draw_thread, app);
}

static void
quit_activated(GSimpleAction *action,
               GVariant      *param,
               gpointer       app)
{
    g_application_quit(G_APPLICATION(app));
}

static void
open_activated(GSimpleAction *action,
               GVariant      *param,
               gpointer       app)
{
    
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         NULL,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        
        tclogo_app_execute(_app, filename);
        
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static GActionEntry app_entries[] = {
    { "quit", quit_activated, NULL, NULL, NULL },
    { "open", open_activated, NULL, NULL, NULL }
};

static void
tclogo_app_init(TclogoApp *app)
{
    
}

static void
tclogo_app_activate(GApplication *_app_)
{
    TclogoApp *app = (TclogoApp *) _app_;
    TclogoAppWindow *win;
    
    _app = app;
    
    app->logo = NULL;
    win = tclogo_app_window_new(TCLOGO_APP(app));
    app->window = win;
    
    gtk_window_present(GTK_WINDOW(win));
}

static void
tclogo_app_startup(GApplication *app)
{
    GtkBuilder *builder;
    GMenuModel *app_menu;
        
    G_APPLICATION_CLASS(tclogo_app_parent_class)->startup(app);
    
    g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries,
                                    G_N_ELEMENTS(app_entries), app);
    
    builder = gtk_builder_new_from_resource("/fr/aurelien/tclogoapp/ui/menu.ui");
    app_menu = G_MENU_MODEL(gtk_builder_get_object(builder, "appmenu"));
    gtk_application_set_app_menu(GTK_APPLICATION(app), app_menu);
    g_object_unref(builder);
}

static void
tclogo_app_class_init(TclogoAppClass *class)
{
    G_APPLICATION_CLASS(class)->activate = tclogo_app_activate;
    G_APPLICATION_CLASS(class)->startup = tclogo_app_startup;
}

TclogoApp *
tclogo_app_new()
{
    return g_object_new(TCLOGO_APP_TYPE,
                        "application-id", "fr.aurelien.tclogo-gui",
                        NULL);
}
