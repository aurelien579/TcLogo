#ifndef GROUP_H
#define GROUP_H

#include "list.h"
#include "element.h"

struct group {
    char               *name;
    struct list_head   *elements;
};

struct group * group_new  (const char *str);
void            group_free (struct group *c);

void group_to_svg(const struct group *c, FILE *out);
void group_define(const struct group *c, FILE *out);

void group_add                 (struct group *c, struct element *el);
void group_relocate_elements   (struct group *c);
void group_move_all            (struct group *c, double x, double y);

double group_max_x (const struct group *c);
double group_max_y (const struct group *c);
double group_width (const struct group *c);
double group_height(const struct group *c);

#endif
