%{
	#include<stdio.h> 
%}

%%
[ab]	   	{
                printf("%s ", yytext);
            }

[\ \t] 		{}

\n 			{}

. 			{
				printf("Error: unidentified token\n");
			}
%%

int main() {
	yylex();
	return 0;
}