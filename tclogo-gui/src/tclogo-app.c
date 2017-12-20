#include <gtk/gtk.h>

#include "tclogo-app.h"
#include "tclogo-app-win.h"

struct _TclogoApp
{
	GtkApplication parent;
};

G_DEFINE_TYPE(TclogoApp, tclogo_app, GTK_TYPE_APPLICATION);

static void
tclogo_app_init(TclogoApp *app)
{
	
}

static void
tclogo_app_activate(TclogoApp *app)
{
	TclogoAppWindow *win;
	
	win = tclogo_app_window_new(TCLOGO_APP(app));
	gtk_window_present(GTK_WINDOW(win));
}

static void
quit_activated(GSimpleAction *action,
			   GVariant      *param,
			   gpointer      app)
{
	g_application_quit(G_APPLICATION(app));
}

static GActionEntry app_entries[] =
{
	{ "quit", quit_activated, NULL, NULL, NULL }
};

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
