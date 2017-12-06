%{
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "logo.tab.h"
%}

%%
[0-9]+ 			yylval.int_type = atoi(yytext); return NUMBER;

"REPEAT" 		return REPEAT;
"LEFT" 			return LEFT;
"RIGHT" 		return RIGHT;
"FORWARD"       return FORWARD;
"COLOR"			return COLOR;
"MOVE"			return MOVE;
"SET_ANGLE"		return SET_ANGLE;
"CANVAS"		return CANVAS;
"SET_CANVAS"	return SET_CANVAS;
"DRAW_CANVAS"	return DRAW_CANVAS;
"MOVE_TO"		return MOVE_TO;


([a-z]|[A-Z])+ {
	strncpy(yylval.str_type, yytext, 10);
	return STR;
}

"\""			return STR_DELEMITER;
"[" 			return '[';
"]" 			return ']';
\n 				/* Skip whitespaces */
[ \t] 			/* Skip whitespaces */
%%
