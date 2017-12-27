#include <tclogo/group.h>
#include <tclogo/utils.h>
#include <tclogo/element.h>

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define G_OPEN  "<g id=\"%s\">"
#define G_CLOSE "</g>"

struct group *
group_new(const char *str)
{
    struct group *c = alloc(struct group);

    c->elements = list_new();
    c->name = alloc_n(char, strlen(str) + 1);
    strcpy(c->name, str);

    return c;
}

void
group_to_svg(const struct group *c,
             FILE               *out)
{
    for_each(struct element, el, c->elements, {
        element_to_svg(el, out);
    });
}

void
group_define(const struct group *c,
             FILE               *out)
{
    fprintf(out, G_OPEN, c->name);
    group_to_svg(c, out);
    fprintf(out, G_CLOSE);
}

void
group_move_all(struct group *c,
               double        x,
               double        y)
{
    for_each(struct element, el, c->elements, {
        element_move(el, x, y);
    });
}

void
group_relocate_elements(struct group *c)
{
    double min_x = find_min_x(c->elements);
    double min_y = find_min_y(c->elements);
    
    group_move_all(c, -min_x, -min_y);
}

void
group_add(struct group   *c,
          struct element *el)
{
    list_add(&c->elements, el);
}

double
group_max_x(const struct group *c)
{
    return find_max_x(c->elements);
}

double
group_max_y(const struct group *c)
{
    return find_max_y(c->elements);
}

double
group_min_x(const struct group *c)
{
    return find_min_x(c->elements);
}

double
group_min_y(const struct group *c)
{
    return find_min_y(c->elements);
}

double
group_width(const struct group *c)
{
    return find_max_x(c->elements) - find_min_x(c->elements);
}

double
group_height(const struct group *c)
{
    return find_max_y(c->elements) - find_min_y(c->elements);
}


void
group_free(struct group *c)
{
    for_each(struct element, el, c->elements, {
        free(el->p);
    });

    list_free(c->elements);

    free(c->name);

    free(c);
}
