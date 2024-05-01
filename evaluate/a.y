%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Function prototypes
int yylex(void);
void yyerror(const char *);

// Define YYSTYPE as a double
#define YYSTYPE double

%}

%token NUM

%left '+' '-'
%left '*' '/'

%%

expression:  expr {
	printf("Result: %.2f\n", $1); 
 	return 0;
};
    
expr: expr '+' expr     { $$ = $1 + $3; }
    | expr '-' expr     { $$ = $1 - $3; }
    | expr '*' expr     { $$ = $1 * $3; }
    | expr '/' expr     { $$ = $1 / $3; }
    | '(' expr ')'      { $$ = $2; }
    | NUM               { $$ = $1; }
    ;


%%

int yylex() {
    int c;
    do {
        c = getchar();
    } while (c == ' ' || c == '\t');
    if (isdigit(c) || c == '.') {
        ungetc(c, stdin);
        scanf("%lf", &yylval);
        return NUM;
    }
    return c;
}

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
printf("\nEnter Any Arithmetic Expression :");
    yyparse();
    return 0;
}