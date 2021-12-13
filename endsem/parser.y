%{
#include <stdio.h>
#include <string.h>
void yyerror(char *);
int yylex(void);
int yydebug=0;  
extern int lineno;
%}

/* YYSTYPE union */
%union{
    char *str;
}

/* token definition */
%token<str> CHAR INT FLOAT DOUBLE IF ELSE WHILE FOR CONTINUE BREAK VOID RETURN BOOL DO THEN
%token<str> ADDOP MULOP DIVOP INCR OROP ANDOP NOTOP EQUOP RELOP
%token<str> LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMI DOT COMMA ASSIGN REFER
%token <str> ID
%token <str> 	 ICONST
%token <str>  FCONST
%token <str> 	 CCONST
%token <str>     STRING

/* precedencies and associativities */
%left LPAREN RPAREN LBRACK RBRACK
%right NOTOP INCR REFER
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

program: declarations functions_optional ;

/* declarations */
declarations: declarations declaration | declaration;

declaration: { declare = 1; } type names { declare = 0; } SEMI ;

type: INT | CHAR | FLOAT | DOUBLE | VOID | BOOL;

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

constant: ICONST | FCONST | CCONST ;

assigment: var_ref ASSIGN expression ;

var_ref  : variable ; 

function_call: ID LPAREN call_params RPAREN;

call_params: call_param | STRING | /* empty */;

call_param : call_param COMMA expression | expression ; 

/* functions */
functions_optional: functions | /* empty */ ;

functions: functions function | function ;

function: { incr_scope(); } function_head function_tail { hide_scope(); };
		
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

void yyerror (char *s)
{
  fprintf(stderr, "Syntax error at line %d %s\n", lineno);
  exit(1);
}

int main(void) {
    yyparse();
    return 0;
}