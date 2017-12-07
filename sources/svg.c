#include "svg.h"
#include "list.h"
#include "element.h"
#include "utils.h"
#include "canvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_COLOR   "black"
#define PI              3.14159265

#define HEADER  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" \
                "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" " \
                "width=\"%d\" height=\"%d\">\n" \
                "<title>Example LOGO</title>\n" \
                "<desc>Du LOGO</desc>\n"

struct logo {
    double              saved_x;
    double              saved_y;
    double              saved_angle;
    double              x;                  /* Current x */
    double              y;                  /* Current y */
    double              angle;              /* Current angle in radians */
    char                color[10];          /* Current color */
    struct canvas *     current_group;
    struct list_head *  groups;
    struct canvas *     root;
};



static void execute(struct logo *logo, const struct node* p);



static struct canvas *
find_group(const struct logo *logo, const char *name)
{
    for_each(struct canvas, canvas, logo->groups, {
        if (strcmp(canvas->name, name) == 0) {
            return canvas;
        }
    });
    
    return NULL;
}



void
execute_group_begin(struct logo* logo, const struct node *node)
{   
    logo->saved_x = logo->x;
    logo->saved_y = logo->y;
    logo->saved_angle = logo->angle;
    
    logo->angle = 0;
    logo->x = 0;
    logo->y = 0;
    logo->current_group = canvas_new(node_get_str(node, 0));
}

void
execute_group_end(struct logo* logo, const struct node *node)
{    
    logo->x = logo->saved_x;
    logo->y = logo->saved_y;
    logo->angle = logo->saved_angle;
    
    list_add(&logo->groups, logo->current_group);
    canvas_relocate_elements(logo->current_group);
    logo->current_group = logo->root;
}

void
execute_use(struct logo *logo, const struct node *node)
{
    struct canvas *src  = find_group(logo, node_get_str(node, 0));    
    assert(src);

#ifdef DONT_USE_USE
    for_each(struct element, el, src->elements, {
        struct element *new_el = el->copy(el);
        new_el->move(new_el, logo->x, logo->y);
        canvas_add(logo->root, new_el);
    });
#else
    canvas_add(logo->root, use_new(src, logo->x, logo->y));
#endif
    
}

void
execute_rectangle(struct logo *logo, const struct node *node)
{    
    struct element *rect = rect_new(logo->x, logo->y,
                                    node_get_int(node, 0),
                                    node_get_int(node, 1));
    
    canvas_add(logo->current_group, rect);
}

void
execute_move_to(struct logo* logo, const struct node *node)
{
    logo->x = node_get_int(node, 0);
    logo->y = node_get_int(node, 1);
}

void
execute_set_angle(struct logo *logo, const struct node *node)
{
    logo->angle = node_get_int(node, 0);
}

void
execute_move(struct logo *logo, const struct node *node)
{	
    logo->x += node_get_int(node, 0) * cos(logo->angle);
    logo->y += node_get_int(node, 0) * sin(logo->angle);
}

void
execute_forward(struct logo *logo, const struct node *node)
{
    double new_x, new_y;

    new_x = logo->x + node_get_int(node, 0) * cos(logo->angle);
    new_y = logo->y + node_get_int(node, 0) * sin(logo->angle);
    
    struct element *line = line_new(logo->x, logo->y,
                                    new_x, new_y, logo->color);
    
    canvas_add(logo->current_group, line);
    
    logo->x = new_x;
    logo->y = new_y;
}

void
execute_left(struct logo *logo, const struct node *node)
{
    logo->angle -= ((double)node_get_int(node, 0)) * (PI/180.0);
}

void
execute_right(struct logo *logo, const struct node *node)
{
    logo->angle += ((double)node_get_int(node, 0)) * (PI/180.0);
}

void
execute_repeat(struct logo *logo, const struct node *node)
{
    for (int i = 0; i < node_get_int(node, 0); i++) {
        execute(logo, node_get_subnode(node, 1));
    }
}

void
execute_color(struct logo *logo, const struct node *node)
{
    strncpy(logo->color, node_get_str(node, 0), STR_LENGTH);
}



static void
execute(struct logo *logo, const struct node* program)
{
    while (program) {
        program->execute(logo, program);        
        program = program->next;
    }
}

void
svg(const struct node *in, const char *out)
{
    FILE *file = fopen(out, "w");    
    struct logo logo;
    
    logo.angle             = 0;
    logo.x                 = 0;
    logo.y                 = 0;
    logo.groups            = list_new();
    logo.root              = canvas_new("root");
    logo.current_group     = logo.root;
    
    strncpy(logo.color, DEFAULT_COLOR, 9);    

    execute(&logo, in);
    
    canvas_relocate_elements(logo.root);
    
    fprintf(file, HEADER, (int) canvas_max_x(logo.root) + 5,
                          (int) canvas_max_y(logo.root) + 5);

#ifndef DONT_USE_USE
    fprintf(file, "<defs>\n");
    
    for_each(struct canvas, canvas, logo.groups, {
        canvas_define(canvas, file);
    });
    
    fprintf(file, "</defs>\n");
#endif
    canvas_to_svg(logo.root, file);

    fprintf(file, "</svg>");
    
    list_free(logo.groups);
    fclose(file);
}
