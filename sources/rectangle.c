#include "element.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECT_SVG "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/>\n"

static void
rect_to_svg(const struct element *el, FILE *out)
{   
    fprintf(out, RECT_SVG,
            el->x, el->y, el->width, el->height);
}

struct element *
rect_new(double x, double y, double width, double height)
{        
    return element_new(x, y, width, height, rect_to_svg, NULL, NULL);
}
