%{
	#include<stdio.h> 
%}

%%

"int"|"char"|"bool"|"if"|"then"|"else"|"while"|"do"|"FALSE"|"return"|"void"|"main" {
    printf("Token and Lexeme : %s\n", yytext);
}

[a-z]+ 		{
				printf("Token:  IDENTIFIER; Lexeme: %s\n", yytext);
			}

[\*=\[\]\(\){}] {
    printf("Token:  OPERATOR; Lexeme: %s\n", yytext);
}

[0-9]+ {
    printf ("Token:  NUMBERS; Lexeme: %s\n", yytext);
}

. 			{}
%%

int main() {
	yylex();
	return 0;
}