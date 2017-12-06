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
%token MOVE

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
	$$ = node_new(NODE_FORWARD, $2, NULL, NULL);
} | LEFT NUMBER {
	$$ = node_new(NODE_LEFT, $2, NULL, NULL);
} | RIGHT NUMBER {
	$$ = node_new(NODE_RIGHT, $2, NULL, NULL);
} | REPEAT NUMBER '[' PROG ']' {
	$$ = node_new(NODE_REPEAT, $2, NULL, $4);
} | COLOR STR_DELEMITER STR STR_DELEMITER {
	$$ = node_new(NODE_COLOR, 0, $3, NULL);
} | MOVE NUMBER {
	$$ = node_new(NODE_MOVE, $2, 0, NULL);
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
