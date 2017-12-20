#ifndef TCLOGO_APP_H
#define TCLOGO_APP_H

#include <gtk/gtk.h>

#define TCLOGO_APP_TYPE (tclogo_app_get_type())
G_DECLARE_FINAL_TYPE(TclogoApp, tclogo_app, TCLOGO, APP, GtkApplication)

TclogoApp	*tclogo_app_new();

#endif
