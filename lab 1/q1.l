%{ 
#include<stdio.h> 
int codec=0, commentc=0, blankc=0; /*Global variables*/
%} 
  
/*Rule Section*/
%%
(" "*"\t"*)*"/*"(.*\n?)*"*/"(" "*"\t"*)*\n {
	for(int i=0; i<yyleng; i++) {
		if(yytext[i] == '\n') commentc++; 
	}
}
(" "*"\t"*)*"//".*\n commentc++; //comment line 
(" "*"\t"*)*\n blankc++; //blank lines
.*\n codec++;  //code line counter 
%% 
  
int main() 
{ 
    // The function that starts the analysis 
    yylex(); 
      
    printf("No. of comment lines = %d\n", commentc);   
    printf("No. of blank lines = %d\n", blankc); 
    printf("No. of code lines = %d\n", codec);  

    return 0;      
}