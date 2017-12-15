#ifndef ELEMENT_H
#define ELEMENT_H

#include <stdio.h>
#include <tclogo/list.h>

struct element;
struct group;

typedef void (*to_svg_t) (const struct element *, FILE *);
typedef void (*move_t)   (struct element *, double, double);

struct element {
    double      x;
    double      y;
    double      width;
    double      height;
    
    to_svg_t    to_svg;
    move_t      move;
    void       *private_data;
};

struct element *line_new        (double      x1,
                                 double      y1,
                                 double      x2,
                                 double      y2,
                                 const char *color);

struct element *rect_new        (double x,
                                 double y,
                                 double width,
                                 double height);

struct element *use_new         (const struct group *grp,
                                 double              x,
                                 double              y);


void            element_to_svg  (const struct element *el,
                                 FILE                 *out);
                             
void            element_move    (struct element *el,
                                 double          x,
                                 double          y);

struct element *element_new     (double     x,
                                 double     y,
                                 double     w,
                                 double     h,
                                 to_svg_t   to_svg,
                                 move_t     move,
                                 void      *p);

double          find_max_y      (const struct list_head *elements);

double          find_max_x      (const struct list_head *elements);

double          find_min_y      (const struct list_head *elements);

double          find_min_x      (const struct list_head *elements);

#endif
