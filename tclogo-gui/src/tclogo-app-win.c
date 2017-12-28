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
};

G_DEFINE_TYPE(TclogoAppWindow, tclogo_app_window,
              GTK_TYPE_APPLICATION_WINDOW);

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
tclogo_app_window_init(TclogoAppWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
    
    win->surface = NULL;
    win->app = NULL;
    win->previous_line = 0;
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(win->textview));
    gtk_text_buffer_create_tag(buffer, "gray_bg","background", "gray", NULL);
    
    g_signal_connect(G_OBJECT(win->drawing_area), "draw",
                     G_CALLBACK(draw_callback), NULL);
    
    g_signal_connect(G_OBJECT(win->delay_range), "value-changed",
                     G_CALLBACK(range_delay_changed), NULL);
}

static void
tclogo_app_window_class_init(TclogoAppWindowClass *class)
{
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                    "/fr/aurelien/tclogoapp/ui/window.ui");
    
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), TclogoAppWindow,
                                         drawing_area);
                                         
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), TclogoAppWindow,
                                         delay_range);
    
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), TclogoAppWindow,
                                         textview);
}

void
tclogo_app_window_highlight(TclogoAppWindow *win,
                            unsigned int     line)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(win->textview));
    GtkTextIter i1, i2;
    
    gtk_text_buffer_get_iter_at_line(buffer, &i1, win->previous_line);
    gtk_text_buffer_get_iter_at_line(buffer, &i2, win->previous_line + 1);
    gtk_text_buffer_remove_tag_by_name(buffer, "gray_bg", &i1, &i2);
    
    gtk_text_buffer_get_iter_at_line(buffer, &i1, line);
    gtk_text_buffer_get_iter_at_line(buffer, &i2, line + 1);
    gtk_text_buffer_apply_tag_by_name(buffer, "gray_bg", &i1, &i2);
    
    win->previous_line = line;
}

void
tclogo_app_window_set_app(TclogoAppWindow *win,
                          TclogoApp       *app)
{
    win->app = app;
}

void
tclogo_app_window_set_text(TclogoAppWindow *win,
                           const gchar *text,
                           gsize size)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(win->textview));
    gtk_text_buffer_insert_at_cursor(buffer, text, size);
}

void
tclogo_app_window_draw_surface(TclogoAppWindow *win,
                               cairo_surface_t *surface)
{
    win->surface = surface;
    gtk_widget_queue_draw(GTK_WIDGET(win));
}

TclogoAppWindow *
tclogo_app_window_new(TclogoApp *app)
{
    TclogoAppWindow *window = g_object_new(TCLOGO_APP_WINDOW_TYPE,
                                           "application", app, NULL);
    return window;
}
