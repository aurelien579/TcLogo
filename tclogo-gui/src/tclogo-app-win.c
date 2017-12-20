#include <gtk/gtk.h>
#include "tclogo-app-win.h"

struct _TclogoAppWindow
{
	GtkApplicationWindow parent;
};

G_DEFINE_TYPE(TclogoAppWindow, tclogo_app_window,
			  GTK_TYPE_APPLICATION_WINDOW);

static void
tclogo_app_window_init(TclogoAppWindow *win)
{
	gtk_widget_init_template(GTK_WIDGET(win));
}

static void
tclogo_app_window_class_init(TclogoAppWindowClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
									"/fr/aurelien/tclogoapp/ui/window.ui");
}

TclogoAppWindow *
tclogo_app_window_new(TclogoApp *app)
{
	return g_object_new(TCLOGO_APP_WINDOW_TYPE, "application", app, NULL);
}
