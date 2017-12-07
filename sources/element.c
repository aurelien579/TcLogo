#include "element.h"
#include "utils.h"
#include <stdlib.h>
#include <float.h>

double
find_min_x(const struct list_head *elements)
{
    double min_x = DBL_MIN;
    
    for_each(struct element, el, elements, {
        min_x = min(min_x, el->x);
    });
    
    return min_x;
}

double
find_min_y(const struct list_head *elements)
{
    double min_y = DBL_MIN;
    
    for_each(struct element, el, elements, {
        min_y = min(min_y, el->y);
    });
    
    return min_y;
}

double
find_max_x(const struct list_head *elements)
{
    double max_x = DBL_MIN;
    
    for_each(struct element, el, elements, {
        max_x = max(max_x, el->x + el->width);
    });
    
    return max_x;
}

double
find_max_y(const struct list_head *elements)
{
    double max_y = DBL_MIN;
    
    for_each(struct element, el, elements, {
        max_y = max(max_y, el->y + el->height);
    });
    
    return max_y;
}

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
