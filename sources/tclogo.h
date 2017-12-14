#ifndef TCLOGO_H
#define TCLOGO_H

struct node;
struct logo;

struct logo *logo_new();
void logo_execute(struct logo *logo, const struct node* program);
struct list_head *logo_get_groups(const struct logo *logo);
struct group *logo_get_root(const struct logo *logo);

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