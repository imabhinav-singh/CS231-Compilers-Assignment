%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int yylex(void);
void yyerror(char *);
int yydebug=0;   /*  set to 1 if using with yacc's debug/verbose flags   */
char *sum(char *str1 , char *str2) ;
char *difference(char *str1 , char *str2) ;
char *SimpleSub(char *str1 , char *str2) ;
char *Vigenere(char *str1 , char *str2) ;
char *LocTran(char *str1 , char *str2) ;
%}

%union {
    char *str;
};

%token  <str>  STRING
%token  <str>  SIMPLESUB
%token  <str>  VIGENERE
%token  <str>  LOCTRAN
%token  <str>  DIGITS


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
            $$ = $1; 
            printf("%s\n", $$);
        }
        |               {}
        ;

expr    :   expr '+' expr {
            $$ = sum($1, $3);
        }
        |   expr '-' expr {
            $$ = difference($1, $3);
        }
        |   SIMPLESUB '(' expr ',' STRING ')' {
            $$ = SimpleSub($3, $5);
        }
        |   VIGENERE '(' expr ',' STRING ')' {
            $$ = Vigenere($3, $5);
        }
        |   LOCTRAN '(' expr ',' DIGITS ')' {
            $$ = LocTran($3, $5);
        }
        |   STRING {
            $$ = $1;
        }
        ;
        

%%      /*   programs   */


char *sum(char *str1 , char *str2) 
{
    int i,len1,len2;

    for(len1=0; str1[len1] != 0; len1++); //len of str1
    for(len2=0; str2[len2] != 0; len2++); //len of str2
    int len = (len1 <= len2) ? len1 : len2;
    char *res = (char *)malloc((len+1)*sizeof(char));
    
    for(i = 0; i <len; i++)
    {
        int temp = ((int)(str1[i]-97 + str2[i]-97))%26;
        res[i] = (char)(temp+97);
    }
    res[i]='\0';
    return (char *)res;
}

char *difference(char *str1 , char *str2) 
{
    int i,len1,len2;

    for(len1=0; str1[len1] != 0; len1++); //len of str1
    for(len2=0; str2[len2] != 0; len2++); //len of str2
    int len = (len1 <= len2) ? len1 : len2;
    char *res = (char *)malloc((len+1)*sizeof(char));
    
    for(i = 0; i <len; i++)
    {
        int temp = ((int)(str1[i]-97 - str2[i]+97))%26;
        res[i] = (char)(temp+97);
    }
    res[i]='\0';
    return (char *)res;
}

char *Vigenere(char *str , char *key) 
{
    int len1,len2;

    for(len1=0; str[len1] != 0; len1++); //len of str
    for(len2=0; key[len2] != 0; len2++); //len of key
    
    int repeat; // no of times key concatenate
    if(len1 % len2 ==0)
    	repeat = len1/len2;
    else
    	repeat = len1/len2 + 1;
    
    char *original_key = (char *)malloc((len2+1)*sizeof(char));
    strcpy(original_key , key);
    while(repeat--)
    	strcat(key,original_key);
    
    return sum(str , key);
}

char *SimpleSub(char *str , char *key) 
{
    int len, i;

    for(len=0; str[len] != 0; len++); //len of str
    char *res = (char *)malloc((len+1)*sizeof(char));
    
    for(i=0 ; i<len ; ++i)
    {
    	int index = (int)(str[i] - 97);
    	res[i] = key[index];
    }

	res[i]='\0';
    return (char *)res;
}

char *LocTran(char *str1 , char *str2) 
{
    int i,j,len1,len2;

    for(len1=0; str1[len1] != 0; len1++); //len of str1
    for(len2=0; str2[len2] != 0; len2++); //len of str2

    int grps = (int)(len1/len2);
    char *res = (char *)malloc((len1+1)*sizeof(char));

    for(i=0; i<grps; i++)
    {
        for(j=0; j<len2; j++)
        {
            res[len2*i+j] = str1[len2*i+((int)str2[j]-48)];
        }
    }

    for(i=0; i<len1-grps*len2; i++)
    {
        res[grps*len2+i] = str1[grps*len2+i];
    }
    
    res[len1] = '\0';
    return (char *)res;
    
}

int main(void) {
    yyparse();
    return 0;
}
