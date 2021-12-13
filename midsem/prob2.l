%{
	#include <stdio.h>
    #include <string.h>
    void yyerror(char *);

    #include "y.tab.h"
%}

%%
S|(Si)|(Sim)|(Simp)|(Simpl)|(Simple)|(SimpleS)|(SimpleSu)|(SimpleSub)	{
				/* printf("Token:  SIMPLESUB; Lexeme: %s\n", yytext); */
                yylval.str = strdup(yytext);
                return  SIMPLESUB;
			}

(V)|(Vi)|(Vig)|(Vige)|(Vigen)|(Vigene)|(Vigener)|(Vigenere)	{
				/* printf("Token:  VIGENERE; Lexeme: %s\n", yytext); */
                yylval.str = strdup(yytext);
                return  VIGENERE;
			}

(L)|(Lo)|(Loc)|(LocT)|(LocTr)|(LocTra)|(LocTran)	{
				/* printf("Token:  LOCTRAN; Lexeme: %s\n", yytext); */
                yylval.str = strdup(yytext);
                return  LOCTRAN;
			}

[a-z]+ 		{
				/* printf("Token:  STRING; Lexeme: %s\n", yytext); */
                yylval.str = strdup(yytext);
                return  STRING;
			}

[0-9]+ 		{
				/* printf("Token:  DIGITS; Lexeme: %s\n", yytext); */
                yylval.str = strdup(yytext);
                return  DIGITS;
			} 

[()\+\-,]	{
				/* printf("Token and Lexeme: %s\n", yytext); */
                return  *yytext;
			}

[\ \t] 		{}

\n 			{
				/* printf("Token and Lexeme: <newline>\n"); */
                return  *yytext;
			}

. 			{
				/* printf("Error: unidentified token\n"); */
                yyerror("invalid character");
			}
%%

void yyerror(char *s) {
    fprintf(stderr, "line %d: yytext = %s.  Error msg: %s.\n", yylineno, yytext, s);
}


int yywrap(void) {
   return 1;
}