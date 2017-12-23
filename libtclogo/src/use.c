#include <tclogo/element.h>
#include <tclogo/utils.h>
#include <tclogo/group.h>

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

#include <stdlib.h>

#define USE_SVG "<use x=\"%f\" y=\"%f\" href=\"#%s\"/>\n"

struct use_private {
    const struct group *g;
};

#ifdef CAIRO
static void
use_draw(const struct element *el,
         cairo_t              *cr)
{
    struct use_private *p = (struct use_private *) el->private_data;
    group_move_all(p->g, el->x, el->y);
    group_draw(p->g, cr);
    group_move_all(p->g, -el->x, -el->y);
}
#endif

static void
use_to_svg(const struct element *el,
		   FILE 			    *out)
{
    struct use_private *p = (struct use_private *) el->private_data;
    fprintf(out, USE_SVG, el->x, el->y, p->g->name);
}

struct element *
use_new(const struct group *group,
		double 				x,
		double 				y)
{
    struct use_private *p = alloc(struct use_private);
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
