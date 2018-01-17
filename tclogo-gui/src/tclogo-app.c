#include <gtk/gtk.h>
#include <glib/gprintf.h>

#include <stdio.h>
#include <time.h>
#include <string.h>

#include <tclogo/tclogo.h>
#include <tclogo/node.h>
#include <tclogo/group.h>

#include <pthread.h>

#include "tclogo-app.h"
#include "tclogo-app-win.h"
#include "panic.h"

struct _TclogoApp
{
    GtkApplication      parent;
    gboolean            running;
    GThread            *thread;
    TclogoAppWindow    *window;
    struct logo        *logo;
    struct node        *root_node;
    double              delay_secs;
    cairo_surface_t    *surface;
    double              x_offset;
    double              y_offset;
};


G_DEFINE_TYPE(TclogoApp, tclogo_app, GTK_TYPE_APPLICATION);

GMainContext *context;

static TclogoApp *app_instance;

static void
tclogo_app_sleep(TclogoApp *app)
{
    if (app->delay_secs > 0.0) {
        struct timespec t;
        t.tv_sec = (long) app->delay_secs;

        long long milli_secs = ((long long) (app->delay_secs * 1000)) -
                                            (t.tv_sec * 1000);

        t.tv_nsec = milli_secs * 1000 * 1000;

        nanosleep(&t, NULL);
    }
}

static void
tclogo_app_draw(TclogoApp            *app,
                const struct element *el,
                cairo_t              *cr,
                double                x,
                double                y)
{
    if (!app->running) {
        g_thread_exit(NULL);
    }

    struct list_head *children = element_get_children(el);

    element_draw(el, cr, x, y);

    for_each(struct element, cur_el, children, {
        tclogo_app_draw(app, cur_el, cr, x + element_get_x(el), y + element_get_y(el));
    });

    if (!app->running) {
        g_thread_exit(NULL);
    }
    gtk_widget_queue_draw(GTK_WIDGET(app->window));
    tclogo_app_sleep(app);
}

static void
step_handler(const struct group   *current_group,
             const struct node    *node,
             const struct element *el)
{
    if (!app_instance->running) {
        g_thread_exit(NULL);
    }

    tclogo_app_window_highlight(app_instance->window, node->line);

    if (strcmp(group_get_name(current_group), "root") == 0 && el) {
        cairo_t *cr = cairo_create(app_instance->surface);

        tclogo_app_draw(app_instance,
                        el,
                        cr,
                        app_instance->x_offset,
                        app_instance->y_offset);

        cairo_destroy(cr);
    }
}

void
tclogo_app_stop_drawing(TclogoApp *app)
{
    app->running = FALSE;

    if (app->thread) {
        g_thread_join(app->thread);
        g_thread_unref(app->thread);
        app->thread = NULL;

        if (app->logo) {
            logo_free(app->logo);
            app->logo = NULL;
        }

        if (app->surface) {
            cairo_surface_destroy(app->surface);
            app->surface = NULL;
        }

        if (app->root_node) {
            node_free(app->root_node);
            app->root_node = NULL;
        }
    }
}

static gpointer
logo_thread(gpointer _app)
{
    TclogoApp       *app = TCLOGO_APP(_app);
    struct group    *root_group;

    app->running = TRUE;
    app->logo = logo_new();

    tclogo_app_window_enable_highlight(app->window);

    logo_execute(app->logo, app->root_node);
    root_group = logo_get_root(app->logo);

    app->x_offset = -group_min_x(root_group);
    app->y_offset = -group_min_y(root_group);

    app->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                              app->x_offset + group_max_x(root_group),
                                              app->y_offset + group_max_y(root_group));

    tclogo_app_window_set_surface(app->window, app->surface);

    logo_free(app->logo);

    app->logo = logo_new();
    logo_set_handler(app->logo, step_handler);
    logo_execute(app->logo, app->root_node);

    logo_free(app->logo);
    app->logo = NULL;

    node_free(app->root_node);
    app->root_node = NULL;

    cairo_surface_destroy(app->surface);
    app->surface = NULL;

    tclogo_app_window_disable_highlight(app->window);

    return NULL;
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
        gchar          *filename, *contents;
        gsize           size;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

        filename = gtk_file_chooser_get_filename(chooser);

        if (!g_file_get_contents(filename, &contents, &size, NULL)) {
            panic("g_file_get_contents()");
        }

        tclogo_app_window_set_text(TCLOGO_APP(app)->window, contents, size);

        if (parse_file(filename, &TCLOGO_APP(app)->root_node) < 0) {
            panic("parse_file()");
        }

        g_free(contents);
        g_free(filename);

        TCLOGO_APP(app)->thread = g_thread_new("logo_thread", logo_thread, app);
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
    TclogoApp *app = TCLOGO_APP(_app_);
    TclogoAppWindow *win;

    app_instance = app;

    win = tclogo_app_window_new(app);

    app->window = win;
    app->logo = NULL;
    app->root_node = NULL;
    app->surface = NULL;
    app->thread = NULL;
    app->delay_secs = 1;
    app->running = FALSE;

    tclogo_app_window_set_app(app->window, app);

    gtk_window_present(GTK_WINDOW(win));
}

static void
tclogo_app_startup(GApplication *app)
{
    GtkBuilder *builder;
    GMenuModel *app_menu;

    G_APPLICATION_CLASS(tclogo_app_parent_class)->startup(app);

    context = g_main_context_default();

    g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries,
                                    G_N_ELEMENTS(app_entries), app);

    builder = gtk_builder_new_from_resource("/fr/aurelien/tclogoapp/ui/menu.ui");
    app_menu = G_MENU_MODEL(gtk_builder_get_object(builder, "appmenu"));
    gtk_application_set_app_menu(GTK_APPLICATION(app), app_menu);
    g_object_unref(builder);
}

static void
tclogo_app_shutdown(GApplication *app)
{
    tclogo_app_stop_drawing(TCLOGO_APP(app));
}

static void
tclogo_app_class_init(TclogoAppClass *class)
{
    G_APPLICATION_CLASS(class)->activate = tclogo_app_activate;
    G_APPLICATION_CLASS(class)->startup = tclogo_app_startup;
    G_APPLICATION_CLASS(class)->shutdown = tclogo_app_shutdown;
}

TclogoAppWindow *
tclogo_app_get_window(TclogoApp *app)
{
    return app->window;
}

void
tclogo_app_set_delay(TclogoApp *app,
                     double secs)
{
    app->delay_secs = secs;
}

TclogoApp *
tclogo_app_new()
{
    return g_object_new(TCLOGO_APP_TYPE,
                        "application-id",
                        "fr.aurelien.tclogo-gui",
                        NULL);
}
