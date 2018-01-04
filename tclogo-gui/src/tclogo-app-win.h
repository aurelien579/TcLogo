#ifndef _TCLOGO_APP_WIN_H_
#define _TCLOGO_APP_WIN_H_

#include <gtk/gtk.h>

#include <tclogo/tclogo.h>

#include "tclogo-app.h"

TclogoAppWindow *tclogo_app_window_new          (TclogoApp *app);
                                                 
void             tclogo_app_window_set_surface  (TclogoAppWindow *win,
                                                 cairo_surface_t *surface);

void             tclogo_app_window_set_app      (TclogoAppWindow *win,
                                                 TclogoApp       *app);

void             tclogo_app_window_set_text     (TclogoAppWindow *win,
                                                 const gchar *text,
                                                 gsize size);
                                                 
void             tclogo_app_window_highlight    (TclogoAppWindow *win,
                                                 unsigned int     line);

void             tclogo_app_window_enable_highlight     (TclogoAppWindow *win);
void             tclogo_app_window_disable_highlight    (TclogoAppWindow *win);
#endif
