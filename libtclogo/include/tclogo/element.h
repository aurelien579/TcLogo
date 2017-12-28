#ifndef _TCLOGO_ELEMENT_H_
#define _TCLOGO_ELEMENT_H_

#include <stdio.h>
#include <tclogo/list.h>
#include <tclogo/tclogo.h>

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

struct element;
struct group;

struct element *line_new        (double      x1,
                                 double      y1,
                                 double      x2,
                                 double      y2,
                                 const char *color);

struct element *rect_new        (double x,
                                 double y,
                                 double width,
                                 double height);

struct element *group_use_new   (const struct group *grp,
                                 double              x,
                                 double              y);

void            element_free    (struct element *el);

void            element_to_svg  (const struct element *el,
                                 FILE                 *out);
                             
void            element_move    (struct element *el,
                                 double          x,
                                 double          y);
                                 
#ifdef CAIRO
void            element_draw    (const struct element *el,
                                 cairo_t              *cr,
                                 int                   x,
                                 int                   y,
                                 draw_callback_t       callback);
#endif                               

void            element_set_linenumber (struct element *el,
                                        unsigned int    linenumber);

unsigned int    element_get_linenumber (const struct element *el);

double          find_max_y      (const struct list_head *elements);

double          find_max_x      (const struct list_head *elements);

double          find_min_y      (const struct list_head *elements);

double          find_min_x      (const struct list_head *elements);

#endif
