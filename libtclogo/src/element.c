#include <tclogo/element.h>
#include <tclogo/utils.h>

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

void
element_to_svg(const struct element *el,
               FILE                 *out)
{
    if (el->to_svg) {
        el->to_svg(el, out);
    }
}

void
element_move(struct element *el,
             double          x,
             double          y)
{
    el->x += x;
    el->y += y;
    
    if (el->move) {
        el->move(el, x, y);
    }
}

struct element *
element_new(double   x,
            double   y,
            double   w,
            double   h,
            to_svg_t to_svg,
            move_t   move,
            void    *private)
{
    struct element *el = alloc(struct element);
    
    el->x            = x;
    el->y            = y;
    el->width        = w;
    el->height       = h;
    el->to_svg       = to_svg;
    el->move         = move;
    el->private_data = private;
    
    return el;
}