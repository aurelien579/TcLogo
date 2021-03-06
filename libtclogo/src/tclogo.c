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

static struct color logo_colors[] =
{
    { "black",      0x000000 },
    { "blue",       0x0000CC },
    { "green",      0x00CC00 },
    { "cyan",       0x00CCCC },
    { "red",        0xCC0000 },
    { "magenta",    0xCC00CC },
    { "yellow",     0xFFFF00 },
    { "brown",      0xCC6600 },
    { "dark_green", 0x006600 },
    { "aqua",       0x006666 },
    { "salmon",     0xFFA07A },
    { "purple",     0x6600CC },
    { "orange",     0xFF8000 },
    { "gray",       0x808080 }
};

static int logo_colors_count = sizeof(logo_colors) / sizeof(struct color);

struct logo {
    double              saved_x;
    double              saved_y;
    double              saved_angle;
    double              x;                  /* Current x */
    double              y;                  /* Current y */
    double              angle;              /* Current angle in radians */
    const struct color *color;              /* Current color */
    struct group       *current_group;
    struct group       *root;
    struct list_head   *groups;

    handler_t           handler;
};

static struct color *
find_color(const char *name)
{
    for (int i = 0; i < logo_colors_count; i++) {
        if (strcmp(logo_colors[i].name, name) == 0) {
            return &logo_colors[i];
        }
    }
    
    return NULL;
}

static void
logo_set_color(struct logo *logo,
               const char  *name)
{
    const struct color *c = find_color(name);
    
    if (c) {
        logo->color = c;
    }
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

static void
logo_do_step(struct logo       *logo,
             const struct node *node)
{
    struct element *el = node->execute(logo, node);

    if (logo->handler) {
        logo->handler(logo->current_group, node, el);
    }
}

void
logo_execute(struct logo        *logo,
             const struct node  *program)
{
    for (const struct node *cur = program; cur; cur = cur->next) {
        logo_do_step(logo, cur);
    }
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
    logo->handler       = NULL;

    logo_set_color(logo, DEFAULT_COLOR);

    return logo;
}

void
logo_set_handler(struct logo *logo,
                 handler_t    handler)
{
    logo->handler = handler;
}

void
logo_free(struct logo *logo)
{
    for_each(struct group, group, logo->groups, {
        group_free(group);
    });

    list_free(logo->groups);

    group_free(logo->root);

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

struct element *
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

    return NULL;
}

struct element *
execute_color_id(struct logo         *logo,
                 const struct node   *node)
{
    logo->color = &logo_colors[node_get_int(node, 0)];
    return NULL;
}

struct element *
execute_color_id_rand(struct logo         *logo,
                 const struct node   *node)
{
    logo->color = &logo_colors[rand() % node_get_int(node, 0)];
    return NULL;
}

struct element *
execute_group_end(struct logo       *logo,
                  const struct node *node)
{
    logo->x = logo->saved_x;
    logo->y = logo->saved_y;
    logo->angle = logo->saved_angle;

    list_add(&logo->groups, logo->current_group);
    group_relocate_elements(logo->current_group);
    logo->current_group = logo->root;

    return NULL;
}

struct element *
execute_use(struct logo        *logo,
            const struct node  *node)
{
    struct group *src  = logo_get_group(logo, node_get_str(node, 0));
    assert(src);

    struct element *use = group_use_new(src, logo->x, logo->y);

    group_add(logo->root, use);

    return use;
}

struct element *
execute_rectangle(struct logo        *logo,
                  const struct node  *node)
{
    struct element *rect = rect_new(logo->x, logo->y,
                                    node_get_int(node, 0),
                                    node_get_int(node, 1),
                                    logo->color);

    element_set_linenumber(rect, node->line);
    group_add(logo->current_group, rect);

    return rect;
}

struct element *
execute_move_to(struct logo        *logo,
                const struct node  *node)
{
    logo->x = node_get_int(node, 0);
    logo->y = node_get_int(node, 1);

    return NULL;
}

struct element *
execute_set_angle(struct logo       *logo,
                  const struct node *node)
{
    logo->angle = node_get_int(node, 0);

    return NULL;
}

struct element *
execute_move(struct logo       *logo,
             const struct node *node)
{
    logo->x += node_get_int(node, 0) * cos(logo->angle);
    logo->y += node_get_int(node, 0) * sin(logo->angle);

    return NULL;
}

struct element *
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

    return line;
}

struct element *
execute_left(struct logo       *logo,
             const struct node *node)
{
    logo->angle -= ((double) node_get_int(node, 0)) * (PI/180.0);

    return NULL;
}

struct element *
execute_right(struct logo       *logo,
              const struct node *node)
{
    logo->angle += ((double) node_get_int(node, 0)) * (PI/180.0);

    return NULL;
}

struct element *
execute_repeat(struct logo       *logo,
               const struct node *node)
{
    struct node *program = node_get_subnode(node, 1);
    struct node *cur;

    for (int i = 0; i < node_get_int(node, 0); i++) {
        cur = program;

        while (cur) {
            logo_do_step(logo, cur);
            cur = cur->next;
        }
    }

    return NULL;
}

struct element *
execute_color(struct logo        *logo,
              const struct node  *node)
{
    logo_set_color(logo, node_get_str(node, 0));

    return NULL;
}
