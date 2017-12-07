#ifndef ELEMENT_H
#define ELEMENT_H

#include <stdio.h>
#include "node.h"

struct element;

typedef void (*to_svg_t) (const struct element *, FILE *);
typedef void (*move_t)   (struct element *, double, double);
typedef struct element * (*copy_t)   (struct element *);

enum element_type {
    EL_LINE    
};

struct element {
    double x;
    double y;
    double width;
    double height;
    
    to_svg_t            to_svg;
    move_t              move;
    copy_t              copy;
    void               *private_data;
};

struct element *line_new(double x1, double y1, double x2, double y2, const char *color);
struct element *rect_new(double x, double y, double width, double height);
struct element *element_new(double x, double y, double w, double h,
                            to_svg_t to_svg, move_t move, copy_t copy,
                            void *private);

#endif
