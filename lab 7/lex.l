%{
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
void yyerror(char *);
%}
num [0-9]
letter [a-e]

%%

{num}+      { yylval.str = strdup(yytext); return DIGIT; }
{letter}   { yylval.str = strdup(yytext); return ID; }
[+-/*\n]    { return *yytext; }
[ \t]	    { }
.           { return yytext[0]; }

%%

int yywrap(void) {
	return 1;
}
