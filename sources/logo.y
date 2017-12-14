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
%token MOVE SET_ANGLE GROUP_BEGIN GROUP_END USE MOVE_TO RECTANGLE

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
} | USE STR {
    $$ = node_new(NODE_USE, 1);
    node_set_str($$, 0, $2);
} | MOVE_TO NUMBER NUMBER {
    $$ = node_new(NODE_MOVE_TO, 2);
    node_set_int($$, 0, $2);
    node_set_int($$, 1, $3);
} | RECTANGLE NUMBER NUMBER {
    $$ = node_new(NODE_RECTANGLE, 2);
    node_set_int($$, 0, $2);
    node_set_int($$, 1, $3);
} | GROUP_BEGIN STR {
    $$ = node_new(NODE_GROUP_BEGIN, 1);
    node_set_str($$, 0, $2);
} | GROUP_END {
    $$ = node_new(NODE_GROUP_END, 0);
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
    
    struct logo *logo = logo_new();
    logo_execute(logo, root);
    
    svg_write(logo, out);    
    
    logo_free(logo);
    node_free(root);
    
    return 0;
}
