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
%token MOVE SET_ANGLE CANVAS SET_CANVAS DRAW_CANVAS MOVE_TO

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
	$$ = node_new(NODE_FORWARD, 1);
	node_set_int($$, 0, $2);
} | LEFT NUMBER {
	$$ = node_new(NODE_LEFT, 1);
	node_set_int($$, 0, $2);
} | RIGHT NUMBER {
	$$ = node_new(NODE_RIGHT, 1);
	node_set_int($$, 0, $2);
} | REPEAT NUMBER '[' PROG ']' {
	$$ = node_new(NODE_REPEAT, 2);
	node_set_int($$, 0, $2);
	node_set_subnode($$, 1, $4);
} | COLOR STR_DELEMITER STR STR_DELEMITER {
	$$ = node_new(NODE_COLOR, 1);
	node_set_str($$, 0, $3);
} | MOVE NUMBER {
	$$ = node_new(NODE_MOVE, 1);
	node_set_int($$, 0, $2);
} | SET_ANGLE NUMBER {
	$$ = node_new(NODE_SET_ANGLE, 1);
	node_set_int($$, 0, $2);
} | CANVAS STR {
	$$ = node_new(NODE_CANVAS, 1);
	node_set_str($$, 0, $2);
} | SET_CANVAS STR {
	$$ = node_new(NODE_SET_CANVAS, 1);
	node_set_str($$, 0, $2);
} | DRAW_CANVAS STR {	
	$$ = node_new(NODE_DRAW_CANVAS, 1);
	node_set_str($$, 0, $2);
} | MOVE_TO NUMBER NUMBER {
	$$ = node_new(NODE_MOVE_TO, 2);
	node_set_int($$, 0, $2);
	node_set_int($$, 1, $3);
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
