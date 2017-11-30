#ifndef SVG_H
#define SVG_H

struct state;
struct node;

void svg(const struct node *in, const char *out);

void execute_forward(struct state *state, const struct node *node);
void execute_repeat (struct state *state, const struct node *node);
void execute_right  (struct state *state, const struct node *node);
void execute_left   (struct state *state, const struct node *node);
void execute_color  (struct state *state, const struct node *node);


#endif
