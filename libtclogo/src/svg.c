#include <tclogo/svg.h>
#include <tclogo/list.h>
#include <tclogo/utils.h>
#include <tclogo/group.h>

#include <stdio.h>
#include <stdlib.h>

#define HEADER  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" \
                "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" " \
                "width=\"%d\" height=\"%d\">\n" \
                "<title>Example LOGO</title>\n" \
                "<desc>Du LOGO</desc>\n"

void
svg_write(const struct logo *logo,
          const char        *out)
{
    struct group        *root = logo_get_root(logo);
    struct list_head    *groups = logo_get_groups(logo);

    FILE *file = fopen(out, "w");

    fprintf(file, HEADER, (int) group_max_x(root) + 5,
                          (int) group_max_y(root) + 5);

    fprintf(file, "<defs>\n");

    for_each(struct group, group, groups, {
        group_define(group, file);
    });

    fprintf(file, "</defs>\n");

    group_to_svg(root, file);

    fprintf(file, "</svg>");

    fclose(file);
}
