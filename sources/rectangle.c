#include "element.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECT_SVG "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/>\n"

static struct element *
rect_copy(struct element *src)
{
	struct element *dest = cast_malloc(struct element);
	
	*dest = *src;
		
	return dest;
}

static void
rect_to_svg(const struct element *el, FILE *out)
{   
    fprintf(out, RECT_SVG,
            el->bounds.x, el->bounds.y, el->bounds.width, el->bounds.height);
}

static void
rect_move(struct element *el, double x, double y)
{    
    el->bounds.x += x;
    el->bounds.y += y;    
}

struct element *
rect_new(double x, double y, double width, double height)
{
    struct element      *el     = cast_malloc(struct element);
       
    el->bounds.x        = x;
    el->bounds.y        = y;
    el->bounds.width    = width;
    el->bounds.height   = height;
    
    el->to_svg          = rect_to_svg;
    el->move            = rect_move;
    el->copy 			= rect_copy;
    
    el->private_data    = NULL;
    
    return el;
}
