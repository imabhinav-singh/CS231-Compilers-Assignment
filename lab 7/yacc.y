%{
#include <stdio.h>
#include <string.h>
void yyerror(char *);
int yylex(void);
int yydebug=0;   /*  set to 1 if using with yacc's debug/verbose flags   */
int n=1;
char tmp[100];
char lhs[100][100];
char rhs[100][100];
int n_rhs = 0;
char *addAddress(char *a, char *op, char *b);
%}


%union{
  char* str;
}

%left '+' '-'
%left '*' '/'

%token <str>DIGIT
%token <str>ID

%type <str> E T F

%%
S	:	E '\n' { 
		// printf("This is start symbol\n"); 
	}
	;

E	:	E '+' T {
		// printf("Calling add address\n");
		$$ =  addAddress(strdup($1), strdup("+"), strdup($3));
		printf("%s = %s + %s\n", $$, $1, $3); 
	}
	|	E '-' T { 
		// printf("Calling add address\n");
		$$ =  addAddress(strdup($1), strdup("-"), strdup($3));
		printf("%s = %s - %s\n", $$, $1, $3); 
	}
	|	T  { 
		$$ = $1; 
	}
	;

T	:	T '*' F { 
		// printf("Calling add address\n");
		$$ =  addAddress(strdup($1), strdup("*"), strdup($3));
		printf("%s = %s * %s\n", $$, $1, $3); 
	}
	|	T '/' F { 
		// printf("Calling add address\n");
		$$ =  addAddress(strdup($1), strdup("/"), strdup($3));
		printf("%s = %s / %s\n", $$, $1, $3); 
	}
	|	F { $$ = $1; }
	;

F	:	DIGIT { $$ = yylval.str; }
	| 	ID { $$ = yylval.str; }
	|	'(' E ')' { $$ = $2; }
	;

%%

char *addAddress(char *a, char *op, char *b) {
	// printf("%d %s %s %s", n, a, op, b);
	char *temp = strcat(a, strcat(op, b));
	// printf("%s", temp);
	int i = 0;
	for(; i < n_rhs; i++) {
		if (strcmp(rhs[i], temp) == 0) {
			break;
		}
	}
	if (i < n_rhs) {
		// printf("true\n");
		// sprintf(tmp, "t%s", lhs[i]);
		return strdup(strcat(strdup("t"), lhs[i]));
	} else {
		sprintf(rhs[n_rhs], "%s", temp);
		sprintf(lhs[n_rhs], "%d", n);
		n_rhs++;
		sprintf(tmp, "t%d", n++);
		return strdup(tmp);
	}
}

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
