#ifndef _TCLOGO_GROUP_H_
#define _TCLOGO_GROUP_H_

#include <tclogo/list.h>
#include <tclogo/element.h>

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

struct group {
    char               *name;
    struct list_head   *elements;
};

/**
 * Create a new group. This new group must be freed with group_free
 */
struct group   *group_new               (const char *str);

void            group_free              (struct group *c);

/**
 * Write svg code representing the content of the groups. This function,
 * write svg code for each element in elements.
 */
void            group_to_svg            (const struct group *c,
                                         FILE               *out);

/**
 * Write the svg group block which appears in the def section. This
 * function just use group_to_svg in a <g></g> block.
 */
void            group_define            (const struct group *c,
                                         FILE               *out);

void            group_add               (struct group   *c,
                                         struct element *el);

/**
 * Relocate all elements so that there is no negative coordinate.
 */
void            group_relocate_elements (struct group *c);

void            group_move_all          (struct group *c,
                                         double x,
                                         double y);

double          group_min_x             (const struct group *c);
double          group_min_y             (const struct group *c);
double          group_max_x             (const struct group *c);
double          group_max_y             (const struct group *c);
double          group_width             (const struct group *c);
double          group_height            (const struct group *c);

#endif
