%{
	#include<stdio.h> 
%}

%%
S|(Si)|(Sim)|(Simp)|(Simpl)|(Simple)|(SimpleS)|(SimpleSu)|(SimpleSub)	{
				printf("Token:  SIMPLESUB; Lexeme: %s\n", yytext);
			}

(V)|(Vi)|(Vig)|(Vige)|(Vigen)|(Vigene)|(Vigener)|(Vigenere)	{
				printf("Token:  VIGENERE; Lexeme: %s\n", yytext);
			}

(L)|(Lo)|(Loc)|(LocT)|(LocTr)|(LocTra)|(LocTran)	{
				printf("Token:  LOCTRAN; Lexeme: %s\n", yytext);
			}

[a-z]+ 		{
				printf("Token:  STRING; Lexeme: %s\n", yytext);
			}

[0-9]+ 		{
				printf("Token:  DIGITS; Lexeme: %s\n", yytext);
			} 

[()\+\-,]	{
				printf("Token and Lexeme: %s\n", yytext);
			}

[\ \t] 		{}

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