#ifndef _TCLOGO_TCLOGO_H_
#define _TCLOGO_TCLOGO_H_

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

struct node;
struct logo;

typedef void (*step_handler_t) (const struct logo *logo);

struct logo      *logo_new();
void              logo_add_step_handler (struct logo    *logo,
                                         step_handler_t  handler);

void              logo_set_step_delay   (struct logo    *logo,
                                         double          delay);

void              logo_set_relocate_step(struct logo *logo,
                                         int val);

struct group     *logo_get_group        (const struct logo *logo,
                                         const char        *name);

#ifdef CAIRO
void              logo_draw             (const struct logo *logo,
                                         cairo_t           *cr);
#endif

void              logo_free             (struct logo *logo);

void              logo_execute          (struct logo *logo,
                                         const struct node* program);

struct list_head *logo_get_groups       (const struct logo *logo);

struct group     *logo_get_root         (const struct logo *logo);

void              execute_forward       (struct logo *logo,
                                         const struct node *node);

void              execute_repeat        (struct logo *logo,
                                         const struct node *node);

void              execute_right         (struct logo *logo,
                                         const struct node *node);

void              execute_left          (struct logo *logo,
                                         const struct node *node);

void              execute_color         (struct logo *logo,
                                         const struct node *node);

void              execute_move          (struct logo *logo,
                                         const struct node *node);

void              execute_set_angle     (struct logo *logo,
                                         const struct node *node);

void              execute_group_begin   (struct logo* logo,
                                         const struct node *node);

void              execute_group_end     (struct logo* logo,
                                         const struct node *node);

void              execute_use           (struct logo* logo,
                                         const struct node *node);

void              execute_move_to       (struct logo* logo,
                                         const struct node *node);

void              execute_rectangle     (struct logo *logo,
                                         const struct node *node);

#endif
