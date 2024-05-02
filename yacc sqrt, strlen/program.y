%{
#include <stdio.h>
#include <math.h>
#include <string.h>
extern int yylex();
void yyerror(const char *s) { fprintf(stderr, "%s\n", s); }
%}

%union {
    int ival;
    char *sval;
}

%token <sval> STRING
%token <ival> NUMBER SQRT STRLEN
%type <ival> expr function
%type <sval> var

%%
program:
    program statement
    |
    ;

statement:
    var '=' expr { printf("%s = %d\n", $1, $3); }
    ;

var:
    'u' { $$ = strdup("u"); }
    | 'v' { $$ = strdup("v"); }
    ;

expr:
    function
    ;

function:
    SQRT '(' NUMBER ')' { $$ = (int) sqrt($3); }
    | STRLEN '(' STRING ')' { $$ = strlen($3) - 2; /* Subtract 2 for the quotes */ }
    ;

%%

int main(void) {
    printf("Enter expressions like 'u = sqrt(36)' or 'v = strlen(\"pune\")':\n");
    return yyparse();
}
