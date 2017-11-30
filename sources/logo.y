%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "svg.h"

int
yyerror(const char *msg)
{
	fprintf(stderr, "[ERROR] %s\n", msg);
}

int
yywrap()
{
	return 1;
}

static struct node *root;

%}

%token NUMBER FORWARD LEFT RIGHT REPEAT STR COLOR STR_DELEMITER
%token CREATE_CANVAS SET_CANVAS DRAW_CANVAS

%union {
	struct node * node_type;
	int 	      int_type;
	char	      str_type[STR_LENGTH];
};

%type <str_type> STR
%type <int_type> NUMBER
%type <node_type> PROG INST

%%

LOGO: PROG {
	root = $1;
}

PROG: INST {
	$$ = $1;
} | PROG INST {
	node_append($1, $2);
	$$ = $1;
}

INST: FORWARD NUMBER {
	$$ = node_new(NODE_FORWARD, $2, NULL, NULL, execute_forward);
} | LEFT NUMBER {
	$$ = node_new(NODE_LEFT, $2, NULL, NULL, execute_left);
} | RIGHT NUMBER {
	$$ = node_new(NODE_RIGHT, $2, NULL, NULL, execute_right);
} | REPEAT NUMBER '[' PROG ']' {
	$$ = node_new(NODE_REPEAT, $2, NULL, $4, execute_repeat);
} | COLOR STR_DELEMITER STR STR_DELEMITER {
	$$ = node_new(NODE_COLOR, 0, $3, NULL, execute_color);
} | CREATE_CANVAS STR {
    $$ = node_new(NODE_CREATE_CANVAS, 0, $2, NULL, NULL);
} | SET_CANVAS STR {
    $$ = node_new(NODE_SET_CANVAS, 0, $2, NULL, NULL);
} | DRAW_CANVAS STR {
    $$ = node_new(NODE_DRAW_CANVAS, 0, $2, NULL, NULL);
}

%%

int main(int argc, char **argv)
{
	yyparse();
	
	char *out = "out.svg";
	
	for (int i = 1; i < argc - 1; i++) {
        if (strcmp("-o", argv[i]) == 0) {
            out = argv[i+1];
        }
	}
	
	svg(root, out);
	
    node_free(root);
    
	return 0;
}
