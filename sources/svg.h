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
void execute_move	(struct state *state, const struct node *node);
void execute_set_angle(struct state *state, const struct node *node);
void execute_canvas(struct state* state, const struct node *node);
void execute_set_canvas(struct state* state, const struct node *node);
void execute_draw_canvas(struct state* state, const struct node *node);
void execute_move_to(struct state* state, const struct node *node);

#endif
