%{
#include <stdio.h>
#include <stdlib.h>
#include <tclogo/constants.h>
#include <tclogo/node.h>
#include <tclogo/logo.tab.h>

unsigned int _current_line_ = 0;
%}

%%
[0-9]+          yylval.int_type = atoi(yytext); return INT_IMEDIATE;

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
"RAND"          return RAND;
"COLOR_ID"      return COLOR_ID;

([a-z]|[A-Z])+ {
    strncpy(yylval.str_type, yytext, STR_LENGTH - 1);
    return STR;
}

"\""            return STR_DELEMITER;
"["             return '[';
"]"             return ']';
\n              _current_line_++;
[ \t]           /* Skip whitespaces */
%%
