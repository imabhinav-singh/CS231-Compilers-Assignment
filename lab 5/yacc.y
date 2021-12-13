%{
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(char *);
int yydebug=0;   /*  set to 1 if using with yacc's debug/verbose flags   */
char *reverse(char *);
int attChecker(char*, char*, char*);
%}

%union {
    struct {
        char *val;
        char *type;
    } node;
};

%token  <node>  STRING
%token  <node>  NUMBER
%token  <node>  FLOAT

%type  <node>  E T F S

%start  S

%%
S   :   E '\n' {
        printf("%s\n", $1.type);
    }
    ;

E   :   E '+' T  {
        if(attChecker($1.type, $3.type, $1.type)) {
            $$.type = $1.type;
            // calculate logic here
        } else if (attChecker($1.type, $3.type, "")) {
            $$.type = "float";
            // calculate logic here
        } else {
            yyerror("Invalid operand types for addition operation");
            YYERROR;
        }
        printf("%s %s\n", $$.val, $$.type);
    }
    |   E '-' T  {
        if(attChecker($1.type, $3.type, "int") || attChecker($1.type, $3.type, "float")) {
            $$.type = $1.type;
            // calculate logic here
        } else if (attChecker($1.type, $3.type, "")) {
            $$.type = "float";
            // calculate logic here
        } else {
            yyerror("Invalid operand types for subtraction operation");
            YYERROR;
        }
        printf("%s %s\n", $$.val, $$.type);
    }
    |   T  {
        // $$.val = $1.val;
        $$.type = $1.type;
        printf("%s %s\n", $$.val, $$.type);
    }
    ;

T   :   T '*' F {
        if(attChecker($1.type, $3.type, "int") || attChecker($1.type, $3.type, "float")) {
            $$.type = $1.type;
            // calculate logic here
        } else if (attChecker($1.type, $3.type, "")) {
            $$.type = "float";
            // calculate logic here
        } else {
            yyerror("Invalid operand types for multiplication operation");
            YYERROR;
        }
        printf("%s %s\n", $$.val, $$.type);
    }
    |   T '/' F {
        if(attChecker($1.type, $3.type, "int") || attChecker($1.type, $3.type, "float")) {
            $$.type = $1.type;
            // calculate logic here
        } else if (attChecker($1.type, $3.type, "")) {
            $$.type = "float";
            // calculate logic here
        } else {
            yyerror("Invalid operand types for division operation");
            YYERROR;
        }
        printf("%s %s\n", $$.val, $$.type);
    }
    |   F {
        // $$.val = $1.val;
        $$.type = $1.type;
        printf("%s %s\n", $$.val, $$.type);
    }
    ;

F   :   NUMBER {
        $1.type = "int";
        // $$.val = $1.val;
        $$.type = $1.type;
    }
    |   FLOAT {
        $1.type = "float";
        // $$.val = $1.val;
        $$.type = $1.type;
    }
    |   STRING {
        $1.type = "string";
        // $$.val = $1.val;
        $$.type = $1.type;
    }
    |   '(' E ')' {
        // $$.val = $2.val;
        $$.type = $2.type;
        printf("%s %s\n", $$.val, $$.type);
    }
    ;

%%

int attChecker(char* a, char* b, char *type) {
    if (strcmp(a, b) == 0) {
        if (strcmp(a, type) == 0) {
            return 1;
        } else {
            return 0;
        }
    }
    else if (strcmp(type, "") == 0 && ((strcmp(a, "int") == 0 && strcmp(b, "float") == 0) || (strcmp(a, "float") == 0 && strcmp(b, "int") == 0))) {
        return 1;
    }
    else {
        return 0;
    }
}

int main(void) {
    yyparse();
    return 0;
}
