#include "canvas.h"
#include "utils.h"
#include "element.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define G_OPEN  "<g id=\"%s\">"
#define G_CLOSE "</g>"

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
canvas_define(const struct canvas *c, FILE *out)
{
    fprintf(out, G_OPEN, c->name);
    canvas_to_svg(c, out);
    fprintf(out, G_CLOSE);
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
canvas_max_x(const struct canvas *c)
{
    return find_max_x(c->elements);
}

double
canvas_max_y(const struct canvas *c)
{
    return find_max_y(c->elements);
}

double
canvas_width(const struct canvas *c)
{
    return find_max_x(c->elements) - find_min_x(c->elements);
}

double
canvas_height(const struct canvas *c)
{
    return find_max_y(c->elements) - find_min_y(c->elements);
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
