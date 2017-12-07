#include "canvas.h"
#include "utils.h"
#include "element.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>

static double
find_min_x(const struct list_head *elements)
{
    double min_x = DBL_MIN;
    
    for_each(struct element, el, elements, {
        min_x = min(min_x, el->x);
    });
    
    return min_x;
}

static double
find_min_y(const struct list_head *elements)
{
    double min_y = DBL_MIN;
    
    for_each(struct element, el, elements, {
        min_y = min(min_y, el->y);
    });
    
    return min_y;
}

static double
find_max_x(const struct list_head *elements)
{
    double max_x = DBL_MIN;
    
    for_each(struct element, el, elements, {
        max_x = max(max_x, el->x + el->width);
    });
    
    return max_x;
}

static double
find_max_y(const struct list_head *elements)
{
    double max_y = DBL_MIN;
    
    for_each(struct element, el, elements, {
        max_y = max(max_y, el->y + el->height);
    });
    
    return max_y;
}

struct canvas *
canvas_new(const char *str)
{
    struct canvas *c = cast_malloc(struct canvas);

    c->elements = list_new();
    strncpy(c->name, str, STR_LENGTH);
    
    return c;
}

void
canvas_to_svg(const struct canvas *c, FILE *out)
{        
    for_each(struct element, el, c->elements, {
        el->to_svg(el, out);
    });
}

void
canvas_move_all(struct canvas *c, double x, double y)
{
    for_each(struct element, el, c->elements, {
        el->move(el, x, y);
    });
}

void
canvas_relocate_elements(struct canvas *c)
{    
    double min_x = find_min_x(c->elements);
    double min_y = find_min_y(c->elements);
    
    canvas_move_all(c, -min_x, -min_y);
}

void
canvas_add(struct canvas *c, struct element *el)
{
    list_add(&c->elements, el);
}

double
canvas_max_x(struct canvas *c)
{
    return find_max_x(c->elements);
}

double
canvas_max_y(struct canvas *c)
{
    return find_max_y(c->elements);
}

void
canvas_free(struct canvas *c)
{    
    for_each(struct element, el, c->elements, {
        free(el->private_data);
    });
    
    list_free(c->elements);
    
    free(c);
}
