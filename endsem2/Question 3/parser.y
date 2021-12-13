%{
	#include "symtab.c"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern FILE *yyin;
	extern FILE *yyout;
	extern int lineno;
	extern int yylex();
	void yyerror();
%}

/* YYSTYPE union */
%union{
    char char_val;
	int int_val;
	double double_val;
	char* str_val;
	list_t* symtab_item;
}

/* token definition */
%token<int_val> CHAR INT IF ELSE WHILE VOID RETURN BOOL DO THEN
%token<int_val> ADDOP MULOP DIVOP INCR OROP ANDOP NOTOP EQUOP RELOP
%token<int_val> LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMI DOT COMMA ASSIGN
%token <symtab_item> ID
%token <int_val> 	 ICONST
%token <str_val>     STRING
%token <str_val>     TRUE
%token <str_val>     FALSE

/* precedencies and associativities */
%left LPAREN RPAREN LBRACK RBRACK
%right NOTOP INCR
%left MULOP DIVOP
%left ADDOP
%left RELOP
%left EQUOP
%left OROP
%left ANDOP
%right ASSIGN
%left COMMA

%start program

%%
program: declarations SEMI functions_optional ;

/* declarations */
declarations: declarations declaration | declaration;

declaration: { declare = 1; } type names { declare = 0; } SEMI ;

type: INT | CHAR | VOID | BOOL;

names: names COMMA variable | variable;

variable: ID |
    ID array
;

array: LBRACK sign constant RBRACK ;

/* statements */
statements: statements statement | statement ;

statement:
	if_statement | while_statement | assigment SEMI | function_call SEMI 
;

if_statement:
		IF expression THEN tail optional_else
;

optional_else: ELSE tail | /* empty */ ;

while_statement: WHILE expression DO tail ;

tail: LBRACE statements RBRACE | statement ;

expression:
    expression ADDOP expression |
    expression MULOP expression |
    expression DIVOP expression |
    expression OROP expression |
    expression ANDOP expression |
    NOTOP expression |
    expression EQUOP expression |
    expression RELOP expression |
	var_ref |
    sign constant |
	function_call
;

sign: ADDOP | /* empty */ ; 

constant: ICONST | TRUE | FALSE;

assigment: var_ref ASSIGN expression ;

var_ref  : variable ; 

function_call: ID LPAREN call_params RPAREN;

call_params: call_param | STRING | /* empty */;

call_param : call_param COMMA expression | expression ; 

/* functions */
functions_optional: functions | /* empty */ ;

functions: functions function | function ;

function: { incr_scope(); } function_head function_tail { hide_scope(); } ;
		
function_head: type ID LPAREN parameters_optional RPAREN ;

return_type: type ;

parameters_optional:  parameters | /* empty */ ;

parameters: parameters COMMA parameter | parameter ;

parameter : { declare = 1; } type variable { declare = 0; };

function_tail: LBRACE declarations_optional statements_optional return_optional RBRACE ;

declarations_optional: declarations | /* empty */ ;

statements_optional: statements | /* empty */ ;

return_optional: RETURN expression SEMI | /* empty */ ;

%%

void yyerror ()
{
  fprintf(stderr, "Syntax error at line %d\n", lineno);
  exit(1);
}

int main (int argc, char *argv[]){

	// initialize symbol table
	init_hash_table();

	// parsing
	int flag;
	yyin = fopen(argv[1], "r");
	flag = yyparse();
	fclose(yyin);
	
	printf("Parsing finished!");
	
	// symbol table dump
	yyout = fopen("symtab_dump.out", "w");
	symtab_dump(yyout);
	fclose(yyout);
	
	return flag;
}