#include <tclogo/element.h>
#include <tclogo/utils.h>

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECT_SVG "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/>\n"

#ifdef CAIRO
static void
rect_draw(const struct element *el,
          cairo_t              *cr)
{
    cairo_rectangle(cr, el->x, el->y, el->width, el->height);
}
#endif

static void
rect_to_svg(const struct element *el,
			FILE 				 *out)
{
    fprintf(out, RECT_SVG,
            el->x, el->y, el->width, el->height);
}

struct element *
rect_new(double x,
		 double y,
		 double width,
		 double height)
{
    return element_new(x, y, width, height,
                       rect_to_svg,
                       NULL,
#ifdef CAIRO
                       rect_draw,
#endif
                       NULL);
}
