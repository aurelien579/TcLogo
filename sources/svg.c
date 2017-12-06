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

#define DEFAULT_COLOR "black"
#define PI 		3.14159265
#define HEADER	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" \
				"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" " \
				"width=\"%d\" height=\"%d\">\n" \
				"<title>Example LOGO</title>\n" \
				"<desc>Du LOGO</desc>\n"
              
struct state {
    double          	x;          /* Current x */
    double          	y;          /* Current y */
    double          	angle;      /* Current angle in radians */
    char            	color[10];  /* Current color */
    struct canvas * 	current_canvas;
    struct list_head * 	canvas;
};

static void execute(struct state *state, const struct node* p);

static struct canvas *
find_canvas(const struct state *state, const char *name)
{
	struct list_head *cur = state->canvas;
	struct canvas *canvas;
	
	while (cur) {
		canvas = list_get(cur, struct canvas);
		
		if (strcmp(canvas->name, name) == 0) {
			return canvas;
		}
		
		list_next(cur);
	}
	
	return NULL;
}

void
execute_canvas(struct state* state, const struct node *node)
{
	list_add(&state->canvas, canvas_new(node_get_str(node, 0)));
}

void
execute_rectangle(struct state *state, const struct node *node)
{
	struct element *rect = rect_new(state->x, state->y,
									node_get_int(node, 0),
									node_get_int(node, 1));
	
	canvas_add(state->current_canvas, rect);
}

void execute_move_to(struct state* state, const struct node *node)
{
	state->x = node_get_int(node, 0);
	state->y = node_get_int(node, 1);
}

void
execute_draw_canvas(struct state *state, const struct node *node)
{
	struct canvas *src  = find_canvas(state, node_get_str(node, 0));
	struct canvas *dest = state->current_canvas;
	
	assert(src);
	
	struct list_head *cur = src->elements;
	struct element *el;
	
	while (cur) {
		el = list_get(cur, struct element);
		el = el->copy(el);
		el->move(el, state->x, state->y);
		canvas_add(dest, el);		
		list_next(cur);
	}
}

void
execute_set_canvas(struct state* state, const struct node *node)
{
	state->current_canvas = find_canvas(state, node_get_str(node, 0));
	assert(state->current_canvas);
}

void
execute_set_angle(struct state *state, const struct node *node)
{
	state->angle = node_get_int(node, 0);
}

void
execute_move(struct state *state, const struct node *node)
{	
	state->x += node_get_int(node, 0) * cos(state->angle);
	state->y += node_get_int(node, 0) * sin(state->angle);
}

void
execute_forward(struct state *state, const struct node *node)
{
	double new_x, new_y;

	new_x = state->x + node_get_int(node, 0) * cos(state->angle);
	new_y = state->y + node_get_int(node, 0) * sin(state->angle);
    
    struct element *line = line_new(state->x, state->y,
								    new_x, new_y, state->color);
	
    canvas_add(state->current_canvas, line);
    
	state->x = new_x;
	state->y = new_y;
	
	if (state->x < 0) {
		canvas_move_all(state->current_canvas, -state->x, 0);
		state->x = 0;
	}
	
	if (state->y < 0) {
		canvas_move_all(state->current_canvas, 0, -state->y);
		state->y = 0;		
	}
}

void
execute_left(struct state *state, const struct node *node)
{
	state->angle -= ((double)node_get_int(node, 0)) * (PI/180.0);
}

void
execute_right(struct state *state, const struct node *node)
{
	state->angle += ((double)node_get_int(node, 0)) * (PI/180.0);
}

void
execute_repeat(struct state *state, const struct node *node)
{
	for (int i = 0; i < node_get_int(node, 0); i++) {
		execute(state, node_get_subnode(node, 1));
	}
}

void
execute_color(struct state *state, const struct node *node)
{
	strncpy(state->color, node_get_str(node, 0), STR_LENGTH);
}

static void
execute(struct state *state, const struct node* program)
{
    while (program) {
        program->execute(state, program);        
        program = program->next;
    }
}

void
svg(const struct node *in, const char *out)
{
	FILE *file = fopen(out, "w");    
	struct state state;
	struct canvas *default_canvas = canvas_new("default");
    
    state.angle             = 0;
    state.x                 = 0;
    state.y                 = 0;
    state.canvas 			= list_new();
    list_add(&state.canvas, default_canvas);
    state.current_canvas    = default_canvas;
       
	strncpy(state.color, DEFAULT_COLOR, 9);    

	execute(&state, in);
	
	fprintf(file, HEADER, (int) canvas_max_x(default_canvas) + 1,
						  (int) canvas_max_y(default_canvas) + 1);

    canvas_to_svg(default_canvas, file);

	fprintf(file, "</svg>");
    
    list_free(state.canvas);
    fclose(file);
}
