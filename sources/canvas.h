#ifndef CANVAS_H
#define CANVAS_H

#include "list.h"
#include "node.h"
#include "element.h"
#include <stdio.h>

struct line {
	double x1, y1, x2, y2;
    char color[STR_LENGTH];
};

struct canvas {
    char                name[STR_LENGTH];
    struct list_head *  elements;
};


struct canvas * canvas_new(const char *str);

void canvas_to_svg(const struct canvas *c, FILE *out);

void canvas_add(struct canvas *c, struct element *el);
void canvas_relocate_elements(struct canvas *c);
void canvas_move_all(struct canvas *c, double x, double y);

double canvas_max_x(struct canvas *c);
double canvas_max_y(struct canvas *c);

void canvas_free(struct canvas *c);

#endif
