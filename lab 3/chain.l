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

            yylval.str = strdup(yytext);
            return  STRING;

        }


Reverse  {
            /* printf("Token: REVERSE;  Lexeme: %s\n", yytext); */

            yylval.str = strdup(yytext);
            return  REVERSE;

         }


[#()]   {
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