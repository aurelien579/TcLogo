#ifndef _TCLOGO_APP_WIN_H_
#define _TCLOGO_APP_WIN_H_

#include <gtk/gtk.h>
#include "tclogo-app.h"

#define TCLOGO_APP_WINDOW_TYPE (tclogo_app_window_get_type())
G_DECLARE_FINAL_TYPE(TclogoAppWindow, tclogo_app_window, TCLOGO,
					 APP_WINDOW, GtkApplicationWindow)

TclogoAppWindow	*tclogo_app_window_new 	(TclogoApp *app);

#endif
