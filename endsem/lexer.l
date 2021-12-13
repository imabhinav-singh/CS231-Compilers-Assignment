%option noyywrap

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
	extern FILE *yyin;
	extern FILE *yyout;
	
	int lineno = 1; // initialize to 1
	void ret_print(char *token_type);
	void yyerror(char *);
%}

%x ML_COMMENT

alpha     [a-zA-Z]
digit     [0-9]
alnum     {alpha}|{digit}
print     [ -~]

ID        {alpha}+{alnum}*
ICONST    "0"|[0-9]{digit}*
FCONST    "0"|{digit}*"."{digit}+
CCONST    (\'{print}\')|(\'\\[nftrbv]\')
STRING    \"{print}*\"

%%

"//".*        { printf("Eat up comment at line %d\n", lineno); } 

"/*"          { printf("Eat up comment from line %d ", lineno); BEGIN(ML_COMMENT); }
<ML_COMMENT>"*/"    { printf("to line %d\n", lineno); BEGIN(INITIAL); }
<ML_COMMENT>[^*\n]+		
<ML_COMMENT>"*"			
<ML_COMMENT>"\n"    { lineno += 1; }


"char"|"CHAR"         { return CHAR; }
"int"|"INT"           { return INT; }
"float"|"FLOAT"       { return FLOAT; }
"double"|"DOUBLE"     { return DOUBLE; }
"if"|"IF"             { return IF; }
"else"|"ELSE"         { return ELSE; }
"while"|"WHILE"       { return WHILE; }
"for"|"FOR"           { return FOR; }
"continue"|"CONTINUE" { return CONTINUE; }
"break"|"BREAK"       { return BREAK; }
"void"|"VOID"         { return VOID; }
"return"|"RETURN"     { return RETURN; }
"bool"                { return BOOL; }
"do"                  { return DO; }
"then"                { return THEN; }


"+"|"-"             { return ADDOP; }
"*"                 { return MULOP; }
"/"                 { return DIVOP; }
"++"|"--"           { return INCR; }
"||"                { return OROP; }
"&&"                { return ANDOP; }
"!"                 { return NOTOP; }
"=="|"!="           { return EQUOP; }
">"|"<"|">="|"<="   { return RELOP; }


"("       { return LPAREN; }
")"       { return RPAREN; }
"]"       { return RBRACK; }
"["       { return LBRACK; }
"{"       { return LBRACE; }
"}"       { return RBRACE; }
";"       { return SEMI; }
"."       { return DOT; }
","       { return COMMA; }
"="       { return ASSIGN; }
"&"       { return REFER; }


{ID}    {
        // insert identifier into symbol table
        yylval.str = yytext;
        return ID;
        }
{ICONST}    { yylval.str   = yytext; return ICONST; }
{FCONST}    { yylval.str = yytext; return FCONST; }
{CCONST}    { yylval.str  = yytext;    return CCONST; }
{STRING}    { yylval.str = malloc(yyleng * sizeof(char));
              strcpy(yylval.str, yytext);  return STRING; }


"\n"        { lineno += 1; }
[ \t\r\f]+  /* eat up whitespace */

.       { yyerror("Unrecognized character"); }

%%