#include <gtk/gtk.h>

#include <stdio.h>
#include <fcntl.h>

#include <tclogo/tclogo.h>
#include <tclogo/constants.h>
#include <tclogo/logo.tab.h>

#include <pthread.h>

#include "tclogo-app.h"
#include "tclogo-app-win.h"
#include "panic.h"

struct _TclogoApp
{
    GtkApplication   parent;
    TclogoAppWindow *window;
    struct logo     *logo;
};

G_DEFINE_TYPE(TclogoApp, tclogo_app, GTK_TYPE_APPLICATION);

static TclogoApp *_app;

static void
step_handler(const struct logo *logo)
{
    gtk_widget_queue_draw(GTK_WIDGET(_app->window));
}

static void
tclogo_app_execute(TclogoApp  *app,
                   const char *filename)
{
    struct node *root;
    int tempfd;
    
    app->logo = logo_new();
    
    tclogo_app_window_set_logo(app->window, app->logo);
    
    tempfd = dup(STDIN_FILENO);
    if (tempfd < 0) {
        panic("dup()");
    }
    
    if (close(STDIN_FILENO) < 0) {
        panic("close()");
    }
    
    if (open(filename, O_RDONLY) != STDIN_FILENO) {
        panic("open()");
    }
    
    if (yyparse(&root) < 0) {
        panic("yyparse()");
    }
        
    if (close(STDIN_FILENO) < 0) {
        panic("close()");
    }
    
    if (dup2(tempfd, STDIN_FILENO) < 0) {
        panic("dup2()");
    }
    
    logo_execute(app->logo, root);
    
    pthread_t thread;
    pthread_create(&thread, NULL, tclogo_app_window_draw_thread, app->window);
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
tclogo_app_activate(TclogoApp *app)
{
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
