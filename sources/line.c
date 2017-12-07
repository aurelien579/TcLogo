#include "element.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SVG "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\"/>\n"

struct line_data {
    double x1;
    double x2;
    double y1;
    double y2;
};

static struct element *
line_copy(struct element *src)
{
    struct element *dest = cast_malloc(struct element);
    
    *dest = *src;
    
    struct line_data *dest_data = cast_malloc(struct line_data);
    struct line_data *src_data = (struct line_data*) src->private_data;
    *dest_data = *src_data;
    
    dest->private_data = dest_data;
    
    return dest;
}

static void
line_to_svg(const struct element *el, FILE *out)
{
    struct line_data *data = (struct line_data *) el->private_data;
    
    fprintf(out, LINE_SVG,
            data->x1, data->y1, data->x2, data->y2, el->color);
}

static void
line_move(struct element *el, double x, double y)
{
    struct line_data *data = (struct line_data *) el->private_data;
    data->x1 += x;
    data->x2 += x;
    data->y1 += y;
    data->y2 += y;
    
    el->bounds.x += x;
    el->bounds.y += y;    
}

struct element *
line_new(double x1, double y1, double x2, double y2, const char *color)
{
    struct element      *el     = cast_malloc(struct element);
    struct line_data    *data   = cast_malloc(struct line_data);
    
    strncpy(el->color, color, STR_LENGTH);
    
    el->bounds.x        = min(x1, x2);
    el->bounds.y        = min(y1, y2);
    el->bounds.width    = max(x1, x2) - el->bounds.x;
    el->bounds.height   = max(y1, y2) - el->bounds.y;
    el->to_svg          = line_to_svg;
    el->move            = line_move;
    el->copy 			= line_copy;
    
    data->x1 = x1;
    data->x2 = x2;
    data->y1 = y1;
    data->y2 = y2;
    
    el->private_data    = data;
    
    return el;
}
