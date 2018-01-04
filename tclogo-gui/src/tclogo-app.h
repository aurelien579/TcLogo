#ifndef _TCLOGO_APP_H_
#define _TCLOGO_APP_H_

#include <gtk/gtk.h>

#define TCLOGO_APP_WINDOW_TYPE (tclogo_app_window_get_type())
G_DECLARE_FINAL_TYPE(TclogoAppWindow, tclogo_app_window, TCLOGO,
                     APP_WINDOW, GtkApplicationWindow)

#define TCLOGO_APP_TYPE (tclogo_app_get_type())
G_DECLARE_FINAL_TYPE(TclogoApp, tclogo_app, TCLOGO, APP, GtkApplication)

TclogoApp   *tclogo_app_new                 ();
void         tclogo_app_set_delay           (TclogoApp *app,
                                             double secs);
TclogoAppWindow *tclogo_app_get_window      (TclogoApp *app);

void tclogo_app_stop_drawing                (TclogoApp *app);

extern GMainContext *context;

#endif
