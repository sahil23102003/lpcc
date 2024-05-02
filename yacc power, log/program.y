%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
extern int yylex();
%}

%union {
    double num;        // For numerical values
    char* str;         // For string values (e.g., identifiers)
}

%token <str> IDENTIFIER
%token <num> NUMBER
%token POW LOG
%type <num> expr     // Expressions return a numeric value
%type <num> statement  // Statements return a numeric value (for expression statements)

%left '+' '-'
%left '*' '/'
%right NEG

%%

program:
    | program statement
    ;

statement:
      expr ';' { printf("Result = %lf\n", $1); }
    | IDENTIFIER '=' expr ';' {
        printf("%s = %lf\n", $1, $3);
        free($1);
    }
    ;

expr:
      NUMBER                   { $$ = $1; }
    | IDENTIFIER               { printf("Variable [%s] used, but not defined in this scope.\n", $1); free($1); $$ = 0; }
    | expr '+' expr            { $$ = $1 + $3; }
    | expr '-' expr            { $$ = $1 - $3; }
    | expr '*' expr            { $$ = $1 * $3; }
    | expr '/' expr            { $$ = $1 / $3; }
    | '-' expr %prec NEG       { $$ = -$2; }
    | '(' expr ')'             { $$ = $2; }
    | POW '(' expr ',' expr ')' { $$ = pow($3, $5); }
    | LOG '(' expr ')'         { $$ = log($3); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    printf("Enter an expression:\n");
    yyparse();
    return 0;
}
