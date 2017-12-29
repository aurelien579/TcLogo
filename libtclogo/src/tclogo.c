#include <tclogo/tclogo.h>
#include <tclogo/utils.h>
#include <tclogo/list.h>
#include <tclogo/group.h>
#include <tclogo/node.h>
#include <tclogo/constants.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_COLOR   "black"
#define PI              3.14159265

struct logo {
    double              saved_x;
    double              saved_y;
    double              saved_angle;
    double              x;                  /* Current x */
    double              y;                  /* Current y */
    double              angle;              /* Current angle in radians */
    char               *color;              /* Current color */
    struct group       *current_group;
    struct group       *root;
    struct list_head   *groups;
};

static void
logo_set_color(struct logo *logo,
               const char *color)
{
    int size = strlen(color);
    
    if (logo->color) {
        free(logo->color);
    }
    
    logo->color = alloc_n(char, size + 1);
    strncpy(logo->color, color, size);
}

struct group *
logo_get_group(const struct logo *logo,
               const char        *name)
{
    for_each(struct group, group, logo->groups, {
        if (strcmp(group_get_name(group), name) == 0) {
            return group;
        }
    });

    return NULL;
}

#ifdef CAIRO
void
logo_draw(const struct logo *logo,
          cairo_t           *cr,
          draw_callback_t    callback)
{
    for_each(struct element, el, group_get_elements(logo->root), {
        element_draw(el, cr, 0, 0, callback);
    });
}
#endif

void
logo_execute(struct logo        *logo,
             const struct node  *program)
{
    for (const struct node *cur = program; cur; cur = cur->next) {
        cur->execute(logo, cur);
    }
    
    group_relocate_elements(logo->root);
}

struct logo *
logo_new()
{
    struct logo *logo = alloc(struct logo);

    logo->angle         = 0;
    logo->x             = 0;
    logo->y             = 0;
    logo->groups        = list_new();
    logo->root          = group_new("root");
    logo->current_group = logo->root;
    logo->color         = NULL;
    
    logo_set_color(logo, DEFAULT_COLOR);

    return logo;
}

void
logo_free(struct logo *logo)
{    
    for_each(struct group, group, logo->groups, {
        group_free(group);
    });
    
    list_free(logo->groups);

    group_free(logo->root);
    
    free(logo->color);
    free(logo);
}

struct list_head *
logo_get_groups(const struct logo *logo)
{
    return logo->groups;
}

struct group *
logo_get_root(const struct logo *logo)
{
    return logo->root;
}

void
execute_group_begin(struct logo         *logo,
                    const struct node   *node)
{
    logo->saved_x = logo->x;
    logo->saved_y = logo->y;
    logo->saved_angle = logo->angle;

    logo->angle = 0;
    logo->x = 0;
    logo->y = 0;
    logo->current_group = group_new(node_get_str(node, 0));
}

void
execute_group_end(struct logo       *logo,
                  const struct node *node)
{
    logo->x = logo->saved_x;
    logo->y = logo->saved_y;
    logo->angle = logo->saved_angle;
    
    list_add(&logo->groups, logo->current_group);
    group_relocate_elements(logo->current_group);
    logo->current_group = logo->root;
}

void
execute_use(struct logo        *logo,
            const struct node  *node)
{
    struct group *src  = logo_get_group(logo, node_get_str(node, 0));
    assert(src);

    group_add(logo->root, group_use_new(src, logo->x, logo->y));
}

void
execute_rectangle(struct logo        *logo,
                  const struct node  *node)
{
    struct element *rect = rect_new(logo->x, logo->y,
                                    node_get_int(node, 0),
                                    node_get_int(node, 1));
    
    element_set_linenumber(rect, node->line);
    group_add(logo->current_group, rect);
}

void
execute_move_to(struct logo        *logo,
                const struct node  *node)
{
    logo->x = node_get_int(node, 0);
    logo->y = node_get_int(node, 1);
}

void
execute_set_angle(struct logo       *logo,
                  const struct node *node)
{
    logo->angle = node_get_int(node, 0);
}

void
execute_move(struct logo       *logo,
             const struct node *node)
{
    logo->x += node_get_int(node, 0) * cos(logo->angle);
    logo->y += node_get_int(node, 0) * sin(logo->angle);
}

void
execute_forward(struct logo        *logo,
                const struct node  *node)
{
    double new_x, new_y;

    new_x = logo->x + node_get_int(node, 0) * cos(logo->angle);
    new_y = logo->y + node_get_int(node, 0) * sin(logo->angle);

    struct element *line = line_new(logo->x, logo->y,
                                    new_x, new_y, logo->color);

    element_set_linenumber(line, node->line);

    group_add(logo->current_group, line);

    logo->x = new_x;
    logo->y = new_y;
}

void
execute_left(struct logo       *logo,
             const struct node *node)
{
    logo->angle -= ((double) node_get_int(node, 0)) * (PI/180.0);
}

void
execute_right(struct logo       *logo,
              const struct node *node)
{
    logo->angle += ((double) node_get_int(node, 0)) * (PI/180.0);
}

void
execute_repeat(struct logo       *logo,
               const struct node *node)
{
    struct node *program = node_get_subnode(node, 1);
    struct node *cur;

    for (int i = 0; i < node_get_int(node, 0); i++) {
        cur = program;

        while (cur) {
            cur->execute(logo, cur);
            cur = cur->next;
        }
    }
}

void
execute_color(struct logo        *logo,
              const struct node  *node)
{
    logo_set_color(logo, node_get_str(node, 0));
}
