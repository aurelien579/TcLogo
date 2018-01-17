#include <gtk/gtk.h>

#include <pthread.h>

#include "tclogo-app-win.h"

struct _TclogoAppWindow
{
    GtkApplicationWindow     parent;
    TclogoApp               *app;
    GtkWidget               *drawing_area;
    GtkWidget               *delay_range;
    GtkWidget               *textview;
    cairo_surface_t         *surface;
    unsigned int             previous_line;
    unsigned int             current_line;
};

G_DEFINE_TYPE(TclogoAppWindow, tclogo_app_window,
              GTK_TYPE_APPLICATION_WINDOW);

static GSource *source = NULL;

static gboolean
draw_callback(GtkWidget *widget,
              cairo_t   *cr,
              gpointer   data)
{
    TclogoAppWindow *win = TCLOGO_APP_WINDOW(gtk_widget_get_toplevel(widget));

    if (win->surface) {
        cairo_set_source_surface(cr, win->surface, 0, 0);
        cairo_paint(cr);
    }

    return FALSE;
}

static void
range_delay_changed(GtkRange *range,
                    gpointer  user_data)
{
    double value = gtk_range_get_value(GTK_RANGE(range));

    TclogoAppWindow *win = TCLOGO_APP_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(range)));
    tclogo_app_set_delay(win->app, value);
}

static void
tclogo_app_window_destroy(GtkWidget *object,
                          gpointer   data)
{
    if (TCLOGO_APP_WINDOW(object)->surface) {
        cairo_surface_destroy(TCLOGO_APP_WINDOW(object)->surface);
        TCLOGO_APP_WINDOW(object)->surface = NULL;
    }

    tclogo_app_window_disable_highlight(TCLOGO_APP_WINDOW(object));
}

static void
tclogo_app_window_init(TclogoAppWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));

    win->surface = NULL;
    win->app = NULL;
    win->previous_line = 0;

    g_signal_connect(G_OBJECT(win),
                     "destroy",
                     G_CALLBACK(tclogo_app_window_destroy),
                     NULL);

    g_signal_connect(G_OBJECT(win->drawing_area),
                     "draw",
                     G_CALLBACK(draw_callback),
                     NULL);

    g_signal_connect(G_OBJECT(win->delay_range),
                     "value-changed",
                     G_CALLBACK(range_delay_changed),
                     NULL);
}

static void
tclogo_app_window_class_init(TclogoAppWindowClass *class)
{
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                                "/fr/aurelien/tclogoapp/ui/window.ui");

    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class),
                                         TclogoAppWindow,
                                         drawing_area);

    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class),
                                         TclogoAppWindow,
                                         delay_range);

    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class),
                                         TclogoAppWindow,
                                         textview);
}

gboolean
update_textview_highlight(gpointer data)
{
    if (!G_TYPE_CHECK_INSTANCE_TYPE(data, TCLOGO_APP_WINDOW_TYPE)) {
        return TRUE;
    }

    TclogoAppWindow *win = TCLOGO_APP_WINDOW(data);

    if (win->previous_line == win->current_line) {
        return TRUE;
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(win->textview));

    GtkTextIter i1, i2;

    gtk_text_buffer_get_iter_at_line(buffer, &i1, win->previous_line);
    gtk_text_buffer_get_iter_at_line(buffer, &i2, win->previous_line + 1);
    gtk_text_buffer_remove_tag_by_name(buffer, "gray_bg", &i1, &i2);

    gtk_text_buffer_get_iter_at_line(buffer, &i1, win->current_line);
    gtk_text_buffer_get_iter_at_line(buffer, &i2, win->current_line + 1);
    gtk_text_buffer_apply_tag_by_name(buffer, "gray_bg", &i1, &i2);

    win->previous_line = win->current_line;

    tclogo_app_window_disable_highlight(win);

    return TRUE;
}

void
tclogo_app_window_enable_highlight(TclogoAppWindow *win)
{
    if (!source) {
        source = g_idle_source_new();

        g_source_set_callback(source, update_textview_highlight, win, NULL);
        g_source_attach(source, context);
        g_source_unref(source);
    }
}

void
tclogo_app_window_disable_highlight(TclogoAppWindow *win)
{
    if (source) {
        g_source_destroy(source);
        source = NULL;
    }
}

void
tclogo_app_window_highlight(TclogoAppWindow *win,
                            unsigned int     line)
{
    if (!source) {
        tclogo_app_window_enable_highlight(win);
    }

    win->current_line = line;
}

void
tclogo_app_window_set_app(TclogoAppWindow *win,
                          TclogoApp       *app)
{
    win->app = app;
}

void
tclogo_app_window_set_surface(TclogoAppWindow *win,
                              cairo_surface_t *surface)
{
    if (win->surface) {
        cairo_surface_destroy(win->surface);
        win->surface = NULL;
    }

    win->surface = cairo_surface_reference(surface);
}

void
tclogo_app_window_set_text(TclogoAppWindow *win,
                           const gchar *text,
                           gsize size)
{
    GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_create_tag(buffer, "gray_bg","background", "gray", NULL);

    gtk_text_buffer_insert_at_cursor(buffer, text, size);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(win->textview), buffer);
    g_object_unref(buffer);
}

TclogoAppWindow *
tclogo_app_window_new(TclogoApp *app)
{
    TclogoAppWindow *window = g_object_new(TCLOGO_APP_WINDOW_TYPE,
                                           "application", app, NULL);
    return window;
}
