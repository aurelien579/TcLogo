#include "svg.h"
#include "list.h"
#include "element.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"
#include "canvas.h"

#define DEFAULT_COLOR "black"
#define PI 		3.14159265
#define HEADER	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" \
				"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" " \
				"width=\"200\" height=\"200\">\n" \
				"<title>Example LOGO</title>\n" \
				"<desc>Du LOGO</desc>\n"

static void execute(struct state *state, const struct node* p);

static void
execute_create_canvas(struct state *state, const char *str)
{
    
}

static void
execute_set_canvas(struct state *state, const char *str)
{
    
}

void
execute_forward(struct state *state, const struct node *node)
{
	double new_x, new_y;

	new_x = state->x + node->int_value * cos(state->angle);
	new_y = state->y + node->int_value * sin(state->angle);
    
    struct element *line = line_new(state->x, state->y, new_x, new_y, state->color);    
    canvas_add(state->canvas, line);
    
	state->x = new_x;
	state->y = new_y;
}

void
execute_left(struct state *state, const struct node *node)
{
	state->angle -= ((double)node->int_value) * (PI/180.0);
}

void
execute_right(struct state *state, const struct node *node)
{
	state->angle += ((double)node->int_value) * (PI/180.0);
}

void
execute_repeat(struct state *state, const struct node *node)
{
	for (int i = 0; i < node->int_value; i++) {
		execute(state, node->subnode);
	}
}

void
execute_color(struct state *state, const struct node *node)
{
	strncpy(state->color, node->str_value, STR_LENGTH);
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
    
    state.angle             = 0;
    state.x                 = 0;
    state.y                 = 0;
    state.canvas            = canvas_new("default");  
    state.current_canvas    = state.canvas;
    
	strncpy(state.color, DEFAULT_COLOR, 10);
    
	fprintf(file, HEADER);

	execute(&state, in);
    
    canvas_relocate_elements(state.canvas);    
    canvas_to_svg(state.canvas, file);

	fprintf(file, "</svg>");
    
    canvas_free(state.canvas);
    fclose(file);
}
