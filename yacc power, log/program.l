%{
#include "y.tab.h"
#include <math.h>
#include <string.h>
extern void yyerror(const char *);
%}

%option noyywrap

%%

[ \t\n]+                  { /* Ignore whitespace */ }
[a-zA-Z_][a-zA-Z0-9_]*    {
    if (strcmp(yytext, "pow") == 0) return POW;
    if (strcmp(yytext, "log") == 0) return LOG;
    yylval.str = strdup(yytext); 
    return IDENTIFIER; 
}
[0-9]+(\.[0-9]+)?         { yylval.num = atof(yytext); return NUMBER; }
"="                       { return '='; }
";"                       { return ';'; }
"("                       { return '('; }
")"                       { return ')'; }
","                       { return ','; }
"+"                       { return '+'; }
"-"                       { return '-'; }
"/"                       { return '/'; }
.                        { yyerror("Invalid character"); }

%%

