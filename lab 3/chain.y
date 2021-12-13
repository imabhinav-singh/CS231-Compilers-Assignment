%{
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(char *);
int yydebug=0;   /*  set to 1 if using with yacc's debug/verbose flags   */
char *reverse(char *);
%}

%union {
    char *str;
};

%token  <str>  STRING
%token  <str>  REVERSE

%type  <str>  start
%type  <str>  expr

%start  start

/* S -: E
    | \epsilon
 E -: E # E 
 |  STRING 
 | REVERSE (STRING) */

%%
start   :   expr '\n'  {
            /* $$ = $1; */
            printf("%s\n", $$);
        }
        |               {}
        ;

expr    :   expr '#' expr {
            $$ = strcat($1, $3);
            if ($1.type === $2.type)
        }
        |   STRING         {
            $$ = $1;
            /* printf("%s", $$); */
        }
        |   REVERSE '(' STRING ')'      {
            $$ = reverse($3);
            /* printf("%s", $$); */
        }
        ;

%%      /*   programs   */


char *reverse(char *str1) {
    int i, len;
    int temp = 0;

    for(len=0; str1[len] != 0; len++);
    for(i = 0; i <len/2; i++)
    {
        temp = str1[i];
        str1[i]=str1[len - 1 - i];
        str1[len - 1 - i] = temp;
    }
    return str1;
}


int main(void) {
    yyparse();
    return 0;
}
