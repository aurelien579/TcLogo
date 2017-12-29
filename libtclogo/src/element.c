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
#define RECT_SVG "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/>\n"
#define LINE_SVG "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\"/>\n"

enum el_type {
    ELEMENT_TYPE_LINE,
    ELEMENT_TYPE_USE,
    ELEMENT_TYPE_RECT
};

struct line_data {
    char *color;
};

struct use_data {
    const struct group *group;
};

struct element {
    enum el_type    type;
    unsigned int    linenumber;
    double          x;
    double          y;
    double          width;
    double          height;
    
    union {
        struct line_data    *line;
        struct use_data     *use;
    };
};

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
    case ELEMENT_TYPE_USE:;
        struct list_head *elements = group_get_elements(el->use->group);
        
        for_each(struct element, cur_el, elements, {
            element_draw(cur_el, cr, x + el->x, y + el->y, callback);
        });
        break;
    }
    
    if (el->type != ELEMENT_TYPE_USE) {
        callback(el->linenumber);
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
                el->line->color);
        break;
    case ELEMENT_TYPE_RECT:
        fprintf(out, RECT_SVG, el->x, el->y, el->width, el->height);
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

static struct element *
element_new(enum el_type type,
            double       x,
            double       y,
            double       w,
            double       h)
{
    struct element *el = alloc(struct element);
    
    el->type    = type;
    el->x       = x;
    el->y       = y;
    el->width   = w;
    el->height  = h;

    return el;
}

struct element *
line_new(double      x1,
         double      y1, 
         double      x2,
         double      y2,
         const char *color)
{
    struct line_data *line;
    struct element *el;
    size_t size;
    double x, y, w, h;
    
    size = strlen(color);
    
    line = alloc(struct line_data);
    
    line->color = alloc_n(char, size + 1);    
    strcpy(line->color, color);

    x = min(x1, x2);
    y = min(y1, y2);
    w = max(x1, x2) - x;
    h = max(y1, y2) - y;
    
    el = element_new(ELEMENT_TYPE_LINE, x, y, w, h);
    el->line = line;
    
    return el;
}

struct element *
rect_new(double x,
         double y,
         double width,
         double height)
{
    return element_new(ELEMENT_TYPE_RECT, x, y, width, height);
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
                     group_height(group));
    el->use = use;
    
    return el;
}

void
element_free(struct element *el)
{
    switch (el->type) {
    case ELEMENT_TYPE_LINE:
        free(el->line->color);
        free(el->line);
        break;
    case ELEMENT_TYPE_USE:
        free(el->use);
        break;
    case ELEMENT_TYPE_RECT:
        break;
    }
    
    free(el);
}

