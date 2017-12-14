#include "element.h"
#include "utils.h"
#include "group.h"
#include <stdlib.h>

#define USE_SVG "<use x=\"%f\" y=\"%f\" href=\"#%s\"/>\n"

static void
use_to_svg(const struct element *el, FILE *out)
{
    struct group *group = (struct group *) el->private_data;
    fprintf(out, USE_SVG, el->x, el->y, group->name);
}

struct element *
use_new(const struct group *group, double x, double y)
{
    return element_new(x, y, group_width(group), group_height(group),
                       use_to_svg, NULL, (void *) group);
}
