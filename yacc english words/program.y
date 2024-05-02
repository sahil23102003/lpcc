%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex(void);
%}

%union {
    char* str; // For storing strings
}

%token <str> IDENTIFIER
%token EOL

%%

input:
      | input line
      ;

line: IDENTIFIER EOL { printf("%s is a valid variable name.\n", $1); free($1); }
    | EOL
    | error EOL { yyerror("Invalid variable name."); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter variable names, one per line:\n");
    yyparse();
    return 0;
}

