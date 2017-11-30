#ifndef SVG_H
#define SVG_H

#include "node.h"

struct state {
    double          x;          /* Current x */
    double          y;          /* Current y */
    double          angle;      /* Current angle in radians */
    char            color[10];  /* Current color */
    struct canvas * current_canvas;
    struct canvas * canvas;
};

void svg(const struct node *in, const char *out);

void execute_forward(struct state *state, const struct node *node);
void execute_repeat (struct state *state, const struct node *node);
void execute_right  (struct state *state, const struct node *node);
void execute_left   (struct state *state, const struct node *node);
void execute_color  (struct state *state, const struct node *node);


#endif
