#include <stdio.h>
#include <string.h>

#include <tclogo/constants.h>
#include <tclogo/logo.tab.h>
#include <tclogo/tclogo.h>
#include <tclogo/svg.h>
#include <tclogo/node.h>

int 
main(int    argc,
     char **argv)
{
    struct node *root;
    struct logo *logo;    
    char *out = "out.svg";
    
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp("-o", argv[i]) == 0) {
            out = argv[i+1];
        }
    }
        
    yyparse(&root);
    
    logo = logo_new();
    logo_execute(logo, root);
    
    svg_write(logo, out);    
    
    logo_free(logo);
    node_free(root);
    
    return 0;
}
