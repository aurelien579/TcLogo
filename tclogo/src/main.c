#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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
    char *in = argv[1];
    
    if (argc < 2) {        fprintf(stderr, "Usage : %s input_file [ -o output_file ]\n", argv[0]);
        return -1;
    }
    
    for (int i = 2; i < argc - 1; i++) {
        if (strcmp("-o", argv[i]) == 0) {
            out = argv[i+1];
        }
    }
    
    int tempfd = dup(STDIN_FILENO);
    close(STDIN_FILENO);
    open(in, O_RDONLY);
    yyparse(&root);
    close(STDIN_FILENO);
    dup2(tempfd, STDIN_FILENO);
    close(tempfd);
    
    logo = logo_new();
    logo_execute(logo, root);
    group_relocate_elements(logo_get_root(logo));
    
    svg_write(logo, out);    
    
    logo_free(logo);
    node_free(root);
    
    return 0;
}
