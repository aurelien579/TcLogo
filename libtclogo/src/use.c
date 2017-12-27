#include <tclogo/element.h>
#include <tclogo/utils.h>
#include <tclogo/group.h>

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

#include <stdlib.h>

#define USE_SVG "<use x=\"%f\" y=\"%f\" href=\"#%s\"/>\n"

struct group_use {
    const struct group *g;
};

#ifdef CAIRO
static void
use_draw(const struct element *el,
         cairo_t              *cr,
         int                   x,
         int                   y,
         draw_callback_t       callback)
{
    struct group_use *p = (struct group_use *) el->p;
    
    for_each(struct element, child, p->g->elements, {
        element_draw(child, cr, x + el->x, y + el->y, callback);
    });
}
#endif

static void
use_to_svg(const struct element *el,
           FILE                 *out)
{
    struct group_use *p = (struct group_use *) el->p;
    
    fprintf(out, USE_SVG, el->x, el->y, p->g->name);
}

struct element *
group_use_new(const struct group *group,
              double              x,
              double              y)
{
    struct group_use *p = alloc(struct group_use);
    p->g = group;
    
    return element_new(x,
                       y,
                       group_width(group),
                       group_height(group),
                       use_to_svg,
                       NULL,
#ifdef CAIRO
                       use_draw,
#endif
                       p);
}
