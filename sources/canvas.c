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
    const struct list_head *cur = elements;
    struct element *el;
    double min_x = DBL_MIN;
    
     while (cur) {
        el = list_get(cur, struct element);
        min_x = min(min_x, el->bounds.x);
        list_next(cur);
    }
    
    return min_x;
}

static double
find_min_y(const struct list_head *elements)
{    
    const struct list_head *cur = elements;
    struct element *el;
    double min_y = DBL_MIN;
    
     while (cur) {
        el = list_get(cur, struct element);
        min_y = min(min_y, el->bounds.y);
        list_next(cur);
    }
    
    return min_y;
}

static double
find_max_x(const struct list_head *elements)
{    
    const struct list_head *cur = elements;
    struct element *el;
    double max_x = DBL_MIN;
    
     while (cur) {
        el = list_get(cur, struct element);
        max_x = max(max_x, el->bounds.x);
        list_next(cur);
    }
    
    return max_x;
}

static double
find_max_y(const struct list_head *elements)
{    
    const struct list_head *cur = elements;
    struct element *el;
    double max_y = DBL_MIN;
    
     while (cur) {
        el = list_get(cur, struct element);
        max_y = max(max_y, el->bounds.y);
        list_next(cur);
    }
    
    return max_y;
}

void
canvas_to_svg(const struct canvas *c, FILE *out)
{	
    struct list_head    *cur = c->elements;
    struct element      *el;
    
    while (cur) {
        el = list_get(cur, struct element);
        
        el->to_svg(el, out);
        
        cur = cur->next;
    }
}

void
canvas_move_all(struct canvas *c, double x, double y)
{
	struct list_head    *cur;
    struct element      *el;
    
	cur = c->elements;    
    while (cur) {
        el = list_get(cur, struct element);        
        el->move(el, x, y);        
        cur = cur->next;
    }
}

void
canvas_relocate_elements(struct canvas *c)
{    
    double min_x = find_min_x(c->elements);
    double min_y = find_min_y(c->elements);
    
    canvas_move_all(c, -min_x, -min_y);
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
    struct list_head *cur = c->elements;
    struct element *el;
    
    while (cur) {
        el = list_get(cur, struct element);
        
        free(el->private_data);
        
        list_next(cur);
    }
    
    list_free(c->elements);
    
    free(c);
}
