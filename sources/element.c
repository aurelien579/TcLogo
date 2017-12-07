#include "element.h"
#include "utils.h"
#include <stdlib.h>

struct element *
element_new(double x, double y, double w, double h,
            to_svg_t to_svg, move_t move, copy_t copy, void *private)
{
    struct element *el = cast_malloc(struct element);
        
    el->x        = x;
    el->y        = y;
    el->width    = w;
    el->height   = h;
    el->to_svg          = to_svg;
    el->move            = move;
    el->copy            = copy;    
    el->private_data    = private;
    
    return el;
}
