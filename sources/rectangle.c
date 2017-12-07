#include "element.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECT_SVG "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/>\n"

static struct element *
rect_copy(struct element *src)
{
    struct element *dest = cast_malloc(struct element);
    
    *dest = *src;
    
    return dest;
}

static void
rect_to_svg(const struct element *el, FILE *out)
{   
    fprintf(out, RECT_SVG,
            el->x, el->y, el->width, el->height);
}

static void
rect_move(struct element *el, double x, double y)
{    
    el->x += x;
    el->y += y;    
}


struct element *
rect_new(double x, double y, double width, double height)
{        
    return element_new(x, y, width, height, rect_to_svg, rect_move, rect_copy, NULL);
}
