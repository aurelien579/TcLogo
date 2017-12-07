#ifndef SVG_H
#define SVG_H

struct logo;
struct node;

void svg(const struct node *in, const char *out);

void execute_forward(struct logo *logo, const struct node *node);
void execute_repeat (struct logo *logo, const struct node *node);
void execute_right  (struct logo *logo, const struct node *node);
void execute_left   (struct logo *logo, const struct node *node);
void execute_color  (struct logo *logo, const struct node *node);
void execute_move	(struct logo *logo, const struct node *node);
void execute_set_angle(struct logo *logo, const struct node *node);
void execute_group_begin(struct logo* logo, const struct node *node);
void execute_group_end(struct logo* logo, const struct node *node);
void execute_use(struct logo* logo, const struct node *node);
void execute_move_to(struct logo* logo, const struct node *node);
void execute_rectangle(struct logo *logo, const struct node *node);
#endif
