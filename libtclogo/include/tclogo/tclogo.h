#ifndef _TCLOGO_TCLOGO_H_
#define _TCLOGO_TCLOGO_H_

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

struct element;
struct group;
struct node;
struct logo;

typedef void (*handler_t)      (const struct group   *current_group,
                                const struct node    *node,
                                const struct element *element);

struct logo      *logo_new              ();
void              logo_free             (struct logo *logo);

void              logo_set_handler      (struct logo *logo,
                                         handler_t    handler);

struct group     *logo_get_group        (const struct logo *logo,
                                         const char        *name);

struct list_head *logo_get_groups       (const struct logo *logo);

struct group     *logo_get_root         (const struct logo *logo);

void              logo_execute          (struct logo       *logo,
                                         const struct node *program);

struct element   *execute_forward       (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_repeat        (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_right         (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_left          (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_color         (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_move          (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_set_angle     (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_group_begin   (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_group_end     (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_use           (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_move_to       (struct logo       *logo,
                                         const struct node *node);

struct element   *execute_rectangle     (struct logo       *logo,
                                         const struct node *node);

#endif
