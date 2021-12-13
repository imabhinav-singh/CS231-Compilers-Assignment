%{
/* C code to be copied verbatim */
#include <stdio.h>
#include <string.h>
void yyerror(char *);

#include "y.tab.h"

%}


%%
    
[a-z]+  {
            /* printf("Token: STRING;  Lexeme: %s\n", yytext); */

            yylval.node.val = strdup(yytext);
            return  STRING;

        }


[0-9]+  {
            /* printf("Token: REVERSE;  Lexeme: %s\n", yytext); */

            yylval.node.val = strdup(yytext);
            return  NUMBER;

        }

([0-9]*[.])?[0-9]+ {

        yylval.node.val = strdup(yytext);
        return  FLOAT;

        }


[\+\-\/\*\(\)]   {
            /* printf("Token and Lexeme: %s\n", yytext); */

            return  *yytext;

        }

\n      {
            /* printf("Token and Lexeme: <newline>\n"); */

            return  *yytext;

        }

[ \t]   {    }     /*  skip whitespace  */

.       {
           /* printf("Invalid character.\n"); */

           yyerror("invalid character");

        }

%%

void yyerror(char *s) {
      fprintf(stderr, "line %d: yytext = %s.  Error msg: %s.\n", yylineno, yytext, s);
}


int yywrap(void) {
   return 1;
}