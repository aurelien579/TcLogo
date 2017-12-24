#include <gtk/gtk.h>

#include <tclogo/tclogo.h>

#include "tclogo-app-win.h"
#include "drawing.h"

struct _TclogoAppWindow
{
    GtkApplicationWindow     parent;
    GtkWidget               *drawing_area;
    const struct logo       *logo;
};

G_DEFINE_TYPE(TclogoAppWindow, tclogo_app_window,
              GTK_TYPE_APPLICATION_WINDOW);

static gboolean
draw_callback(GtkWidget *widget,
              cairo_t   *cr,
              gpointer   data)
{
    TclogoAppWindow *win = TCLOGO_APP_WINDOW(gtk_widget_get_toplevel(widget));
        
    if (win->logo) {
        logo_draw(win->logo, cr);
    }
    
    return FALSE;
}

static void
tclogo_app_window_init(TclogoAppWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
    
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
