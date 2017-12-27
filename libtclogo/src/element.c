#ifdef CAIRO
#include <gdk/gdk.h>
#endif

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

#ifdef CAIRO
void
element_draw(const struct element *el,
             cairo_t              *cr,
             int                   x,
             int                   y,
             draw_callback_t       callback)
{
    if (el->draw) {
        el->draw(el, cr, x, y, callback);
    }
}
#endif

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

void
element_set_linenumber(struct element *el, unsigned int linenumber)
{
    el->linenumber = linenumber;
}

unsigned int
element_get_linenumber(const struct element *el)
{
    return el->linenumber;
}

struct element *
element_new(double   x,
            double   y,
            double   w,
            double   h,
            to_svg_t to_svg,
            move_t   move,
#ifdef CAIRO
            draw_t   draw,
#endif
            void    *private)
{
    struct element *el = alloc(struct element);
    
    el->x            = x;
    el->y            = y;
    el->width        = w;
    el->height       = h;
    el->to_svg       = to_svg;
    el->move         = move;
#ifdef CAIRO
    el->draw        = draw;
#endif    

    el->p = private;

    return el;
}
