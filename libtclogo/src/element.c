#ifdef CAIRO
#include <gdk/gdk.h>
#endif

#include <tclogo/group.h>
#include <tclogo/element.h>
#include <tclogo/utils.h>
#include <tclogo/list.h>

#include <string.h>
#include <stdlib.h>
#include <float.h>

#define USE_SVG  "<use x=\"%f\" y=\"%f\" href=\"#%s\"/>\n"
#define RECT_SVG "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/ stroke=\"#%x\"/>\n"
#define LINE_SVG "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"#%x\"/>\n"

enum el_type {
    ELEMENT_TYPE_LINE,
    ELEMENT_TYPE_USE,
    ELEMENT_TYPE_RECT
};

struct use_data {
    const struct group *group;
};

struct element {
    enum el_type         type;
    unsigned int         linenumber;
    double               x;
    double               y;
    double               width;
    double               height;
    const struct color  *color;
    struct use_data     *use;
};

double
find_min_x(const struct list_head *elements)
{
    double min_x = DBL_MIN;

    for_each(struct element, el, elements, {
        min_x = min(min_x, el->x);
        min_x = min(min_x, el->x + el->width);
    });

    return min_x;
}

double
find_min_y(const struct list_head *elements)
{
    double min_y = DBL_MIN;

    for_each(struct element, el, elements, {
        min_y = min(min_y, el->y);
        min_y = min(min_y, el->y + el->height);
    });

    return min_y;
}

double
find_max_x(const struct list_head *elements)
{
    double max_x = DBL_MIN;

    for_each(struct element, el, elements, {
        max_x = max(max_x, el->x + el->width);
        max_x = max(max_x, el->x);
    });

    return max_x;
}

double
find_max_y(const struct list_head *elements)
{
    double max_y = DBL_MIN;

    for_each(struct element, el, elements, {
        max_y = max(max_y, el->y + el->height);
        max_y = max(max_y, el->y);
    });

    return max_y;
}

#ifdef CAIRO
void
element_draw(const struct element *el,
             cairo_t              *cr,
             int                   x,
             int                   y)
{
    if (el->color) {
        int rgb = el->color->rgb;
        cairo_set_source_rgb(cr, (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
    }
    
    switch (el->type)
    {
    case ELEMENT_TYPE_LINE:
        cairo_move_to(cr, x + el->x, y + el->y);
        cairo_line_to(cr, x + el->x + el->width, y + el->y + el->height);
        cairo_stroke(cr);
        break;
    case ELEMENT_TYPE_RECT:
        cairo_rectangle(cr, x + el->x, y + el->y, el->width, el->height);
        cairo_fill(cr);
        break;
    case ELEMENT_TYPE_USE:
        break;
    }
}
#endif

void
element_to_svg(const struct element *el,
               FILE                 *out)
{   
    switch (el->type) {
    case ELEMENT_TYPE_LINE:
        fprintf(out,
                LINE_SVG,
                el->x,
                el->y,
                el->x + el->width,
                el->y + el->height,
                el->color->rgb);
        break;
    case ELEMENT_TYPE_RECT:
        fprintf(out, RECT_SVG, el->x, el->y, el->width, el->height, el->color->rgb);
        break;
    case ELEMENT_TYPE_USE:
        fprintf(out, USE_SVG, el->x, el->y, group_get_name(el->use->group));
        break;
    }
}

void
element_move(struct element *el,
             double          x,
             double          y)
{
    el->x += x;
    el->y += y;
}

void
element_set_linenumber(struct element  *el,
                       unsigned int     linenumber)
{
    el->linenumber = linenumber;
}

unsigned int
element_get_linenumber(const struct element *el)
{
    return el->linenumber;
}

double
element_get_x(const struct element *el)
{
    return el->x;
}

double
element_get_y(const struct element *el)
{
    return el->y;
}

struct list_head *
element_get_children(const struct element *el)
{
    if (el->type == ELEMENT_TYPE_USE) {
        return group_get_elements(el->use->group);
    } else {
        return (struct list_head *) NULL;
    }
}

static struct element *
element_new(enum el_type        type,
            double              x,
            double              y,
            double              w,
            double              h,
            const struct color *c)
{
    struct element *el = alloc(struct element);
    
    el->type    = type;
    el->x       = x;
    el->y       = y;
    el->width   = w;
    el->height  = h;    
    el->color   = c;
    el->use     = NULL;
    
    return el;
}

struct element *
line_new(double              x1,
         double              y1, 
         double              x2,
         double              y2,
         const struct color *color)
{
    struct element *el;
    double x, y, w, h;
    
    x = x1;
    y = y1;
    w = x2 - x1;
    h = y2 - y1;
    
    el = element_new(ELEMENT_TYPE_LINE, x, y, w, h, color);
    
    return el;
}

struct element *
rect_new(double              x,
         double              y,
         double              width,
         double              height,
         const struct color *color)
{
    return element_new(ELEMENT_TYPE_RECT, x, y, width, height, color);
}

struct element *
group_use_new(const struct group    *group,
              double                 x,
              double                 y)
{
    struct use_data *use;
    struct element *el;
    
    use = alloc(struct use_data);
    use->group = group;
    
    el = element_new(ELEMENT_TYPE_USE,
                     x,
                     y,
                     group_width(group),
                     group_height(group),
                     NULL);

    el->use = use;
    
    return el;
}

void
element_free(struct element *el)
{
    if (el->use) {
        free(el->use);
    }
    
    free(el);
}

