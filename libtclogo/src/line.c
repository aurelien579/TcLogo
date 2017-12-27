#include <tclogo/element.h>
#include <tclogo/utils.h>
#include <tclogo/constants.h>

#ifdef CAIRO
#include <gdk/gdk.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SVG "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\"/>\n"

struct line {
    char    color[STR_LENGTH];
    double  x1;
    double  x2;
    double  y1;
    double  y2;
};

#ifdef CAIRO
static void
line_draw(const struct element *el,
          cairo_t              *cr,
          int                   x,
          int                   y,
          draw_callback_t       callback)
{
    struct line *line = (struct line *) el->p;
    
    cairo_move_to(cr, x + line->x1, y + line->y1);
    cairo_line_to(cr, x + line->x2, y + line->y2);
    cairo_stroke(cr);
    
    callback();
}
#endif

static void
line_to_svg(const struct element *el,
            FILE                 *out)
{
    struct line *line = (struct line *) el->p;

    fprintf(out, LINE_SVG, line->x1, line->y1, line->x2, line->y2, line->color);
}

static void
line_move(struct element *el,
          double          x,
          double          y)
{
    struct line *line = (struct line *) el->p;
    
    line->x1 += x;
    line->x2 += x;
    line->y1 += y;
    line->y2 += y;
}

struct element *
line_new(double      x1,
         double      y1,
         double      x2,
         double      y2,
         const char *color)
{
    struct line *line = alloc(struct line);
    
    strncpy(line->color, color, STR_LENGTH);
    line->x1 = x1;
    line->x2 = x2;
    line->y1 = y1;
    line->y2 = y2;

    double x = min(x1, x2);
    double y = min(y1, y2);
    double w = max(x1, x2) - x;
    double h = max(y1, y2) - y;

    return element_new(x, y, w, h,
                       line_to_svg,
                       line_move,
#ifdef CAIRO
                       line_draw,
#endif
                       line);
}
