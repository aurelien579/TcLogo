#include <gtk/gtk.h>
#include <time.h>

#include <tclogo/group.h>
#include <tclogo/tclogo.h>
#include <tclogo/list.h>

#include "tclogo-app-win.h"

struct _TclogoAppWindow
{
    GtkApplicationWindow     parent;
    GtkWidget               *drawing_area;
    const struct logo       *logo;
    cairo_surface_t         *surface;
};

G_DEFINE_TYPE(TclogoAppWindow, tclogo_app_window,
              GTK_TYPE_APPLICATION_WINDOW);

static TclogoAppWindow *_win;

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
    
    win->logo = NULL;
    win->surface = NULL;
    
    g_signal_connect(G_OBJECT(win->drawing_area), "draw",
                     G_CALLBACK(draw_callback), NULL);

    _win = win;
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
logo_draw_callback()
{
    gtk_widget_queue_draw(GTK_WIDGET(_win));
    
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 500000000L; // 0.5 secs
    
    nanosleep(&t, NULL);
}

void *
tclogo_app_window_draw_thread(void *_window)
{
    TclogoAppWindow *win = (TclogoAppWindow *) _window;
    
    win->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                              group_width(logo_get_root(win->logo)),
                                              group_height(logo_get_root(win->logo)));
    
    cairo_t *cr = cairo_create(win->surface);
    logo_draw(win->logo, cr, logo_draw_callback);
}

void
tclogo_app_window_set_logo(TclogoAppWindow   *win,
                           const struct logo *logo)
{
    win->logo = logo;
}

TclogoAppWindow *
tclogo_app_window_new(TclogoApp *app)
{
    return g_object_new(TCLOGO_APP_WINDOW_TYPE, "application", app, NULL);
}
