#include <gtk/gtk.h>

#include "tclogo-app-win.h"

struct _TclogoAppWindow
{
    GtkApplicationWindow     parent;
    GtkWidget               *drawing_area;
    cairo_surface_t         *surface;
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
tclogo_app_window_init(TclogoAppWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
    
    win->surface = NULL;
    
    g_signal_connect(G_OBJECT(win->drawing_area), "draw",
                     G_CALLBACK(draw_callback), NULL);
}

static void
tclogo_app_window_class_init(TclogoAppWindowClass *class)
{
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                    "/fr/aurelien/tclogoapp/ui/window.ui");
    
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class),
                                         TclogoAppWindow,
                                         drawing_area);
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
    return g_object_new(TCLOGO_APP_WINDOW_TYPE, "application", app, NULL);
}
