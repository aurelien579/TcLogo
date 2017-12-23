#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <tclogo/tclogo.h>
#include <gtk/gtk.h>

void draw_logo(const struct logo *logo,
               cairo_t           *cr);

#endif
