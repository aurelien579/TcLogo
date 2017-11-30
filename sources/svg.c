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

struct state {
    double          x;          /* Current x */
    double          y;          /* Current y */
    double          angle;      /* Current angle in radians */
    char            color[10];  /* Current color */
    struct canvas * current_canvas;
    struct canvas * canvas;
};

static void execute(struct state *state, const struct node* p);

static void
execute_create_canvas(struct state *state, const char *str)
{
    
}

static void
execute_set_canvas(struct state *state, const char *str)
{
    
}

static void
execute_forward(struct state *state, int val)
{
	double new_x, new_y;

	new_x = state->x + val * cos(state->angle);
	new_y = state->y + val * sin(state->angle);
    
    struct element *line = line_new(state->x, state->y, new_x, new_y, state->color);    
    canvas_add(state->canvas, line);
    
	state->x = new_x;
	state->y = new_y;
}

static void
execute_left(struct state *state, int val)
{
	state->angle -= ((double)val) * (PI/180.0);
}

static void
execute_right(struct state *state, int val)
{
	state->angle += ((double)val) * (PI/180.0);
}

static void
execute_repeat(struct state *state, int n, const struct node *p)
{
	for (int i = 0; i < n; i++) {
		execute(state, p);
	}
}

static void
execute_color(struct state *state, const char *str)
{
	strncpy(state->color, str, 10);
}

static void
execute(struct state *state, const struct node* program)
{
    while (program) {
        switch (program->type) {
            case NODE_FORWARD:
                execute_forward(state, program->int_value);
                break;
            case NODE_RIGHT:
                execute_right(state, program->int_value);
                break;
            case NODE_LEFT:
                execute_left(state, program->int_value);
                break;
            case NODE_REPEAT:
                execute_repeat(state, program->int_value, program->subnode);
                break;
            case NODE_COLOR:
                execute_color(state, program->str_value);
                break;
        }
        
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
