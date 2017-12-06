#ifndef ELEMENT_H
#define ELEMENT_H

#include <stdio.h>
#include "node.h"

struct bounds {
    double x;
    double y;
    double width;
    double height;    
};

enum element_type {
    EL_LINE    
};

struct element {
    enum element_type   type;
    struct bounds       bounds;
    char                color[STR_LENGTH];
    
    void              (*to_svg) (const struct element *, FILE *);
    void              (*move)   (struct element *, double, double);
    struct element *  (*copy)   (struct element *);
    void               *private_data;
};

struct element *line_new(double x1, double y1, double x2, double y2, const char *color);

#endif
