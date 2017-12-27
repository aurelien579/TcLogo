#ifndef _TCLOGO_TCLOGO_H_
#define _TCLOGO_TCLOGO_H_

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

struct node;
struct logo;

typedef void (*step_handler_t) (const struct logo *logo);
typedef void (*draw_callback_t) (void);

struct logo      *logo_new              ();
void              logo_free             (struct logo *logo);

struct group     *logo_get_group        (const struct logo *logo,
                                         const char        *name);

struct list_head *logo_get_groups       (const struct logo *logo);

struct group     *logo_get_root         (const struct logo *logo);

#ifdef CAIRO
void              logo_draw             (const struct logo *logo,
                                         cairo_t           *cr,
                                         draw_callback_t    callback);
#endif

void              logo_execute          (struct logo       *logo,
                                         const struct node *program);

void              execute_forward       (struct logo       *logo,
                                         const struct node *node);

void              execute_repeat        (struct logo       *logo,
                                         const struct node *node);

void              execute_right         (struct logo       *logo,
                                         const struct node *node);

void              execute_left          (struct logo       *logo,
                                         const struct node *node);

void              execute_color         (struct logo       *logo,
                                         const struct node *node);

void              execute_move          (struct logo       *logo,
                                         const struct node *node);

void              execute_set_angle     (struct logo       *logo,
                                         const struct node *node);

void              execute_group_begin   (struct logo       *logo,
                                         const struct node *node);

void              execute_group_end     (struct logo       *logo,
                                         const struct node *node);

void              execute_use           (struct logo       *logo,
                                         const struct node *node);

void              execute_move_to       (struct logo       *logo,
                                         const struct node *node);

void              execute_rectangle     (struct logo       *logo,
                                         const struct node *node);

#endif
