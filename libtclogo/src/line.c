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

struct line_data {
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
    struct line_data *data = (struct line_data *) el->private_data;
    cairo_move_to(cr, x + data->x1, y + data->y1);
    cairo_line_to(cr, x + data->x2, y + data->y2);
    cairo_stroke(cr);
    callback();
}
#endif

static void
line_to_svg(const struct element *el,
            FILE                 *out)
{
    struct line_data *data = (struct line_data *) el->private_data;

    fprintf(out, LINE_SVG,
            data->x1, data->y1, data->x2, data->y2, data->color);
}

static void
line_move(struct element *el,
          double          x,
          double          y)
{
    struct line_data *data = (struct line_data *) el->private_data;
    data->x1 += x;
    data->x2 += x;
    data->y1 += y;
    data->y2 += y;
}

struct element *
line_new(double      x1,
         double      y1,
         double      x2,
         double      y2,
         const char *color)
{
    struct line_data *data = alloc(struct line_data);
    strncpy(data->color, color, STR_LENGTH);
    data->x1 = x1;
    data->x2 = x2;
    data->y1 = y1;
    data->y2 = y2;

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
                       data);
}
