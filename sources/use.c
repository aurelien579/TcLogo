#include "element.h"
#include "utils.h"
#include "canvas.h"
#include <stdlib.h>

#define USE_SVG "<use x=\"%f\" y=\"%f\" href=\"#%s\"/>\n"

static struct element *
use_copy(struct element *src)
{
    struct element *dest = cast_malloc(struct element);
    
    *dest = *src;
    
    return dest;
}

static void
use_to_svg(const struct element *el, FILE *out)
{
    struct canvas *canvas = (struct canvas *) el->private_data;
    fprintf(out, USE_SVG, el->x, el->y, canvas->name);
}

static void
use_move(struct element *el, double x, double y)
{
    el->x += x;
    el->y += y;
}

struct element *
use_new(const struct canvas *canvas, double x, double y)
{
    return element_new(x, y, canvas_width(canvas), canvas_height(canvas),
                       use_to_svg, use_move, use_copy, (void *) canvas);
}
