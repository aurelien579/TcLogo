%{
#include <stdio.h>
#include <stdlib.h>
#include <tclogo/constants.h>
#include <tclogo/node.h>
#include <tclogo/logo.tab.h>
%}

%%
[0-9]+          yylval.int_type = atoi(yytext); return NUMBER;

"REPEAT"        return REPEAT;
"LEFT"          return LEFT;
"RIGHT"         return RIGHT;
"FORWARD"       return FORWARD;
"COLOR"         return COLOR;
"MOVE"          return MOVE;
"SET_ANGLE"     return SET_ANGLE;
"GROUP_BEGIN"   return GROUP_BEGIN;
"GROUP_END"     return GROUP_END;
"USE"           return USE;
"MOVE_TO"       return MOVE_TO;
"RECTANGLE"     return RECTANGLE;


([a-z]|[A-Z])+ {
    strncpy(yylval.str_type, yytext, STR_LENGTH - 1);
    return STR;
}

"\""            return STR_DELEMITER;
"["             return '[';
"]"             return ']';
\n              /* Skip whitespaces */
[ \t]           /* Skip whitespaces */
%%
