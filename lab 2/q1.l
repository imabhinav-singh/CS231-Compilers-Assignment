%{
	#include<stdio.h> 
%}

%%
\#|\(|\)	{
				printf("Token and Lexeme : %s\n", yytext);
			}

Reverse		{
				printf("Token:  REVERSE; Lexeme: Reverse\n");
			}

[\ \t] 		{}

[a-z]* 		{
				printf("Token:  STRING; Lexeme: %s\n", yytext);
			}

\n 			{
				printf("Token and Lexeme: <newline>\n");
			}

. 			{
				printf("Error: unidentified token\n");
			}
%%

int main() {
	yylex();
	return 0;
}