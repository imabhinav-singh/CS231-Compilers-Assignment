%{ 
#include<stdio.h> 
int bracesc=0, keywordsc=0; /*Global variables*/
%} 
  
/*Rule Section*/
%%
[{\(\)\[\]}] bracesc++; //braces
(auto|double|int|struct|break|else|long|switch) keywordsc++; //keywords
(case|enum|register|typedef|char|extern|return|union) keywordsc++; //keywords
(continue|for|signed|void|do|if|static|while) keywordsc++; //keywords
(default|goto|sizeof|volatile|const|float|short|unsigned) keywordsc++; //keywords
%% 
  
int main() 
{ 
    // The function that starts the analysis 
    yylex(); 
      
    printf("No. of braces = %d\n", bracesc); 
    printf("No. of keywords = %d\n", keywordsc); 

    return 0;      
}