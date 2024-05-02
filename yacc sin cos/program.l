%{
#include "y.tab.h" // make sure this matches your Bison-generated header
#include <math.h>
#include <string.h>
%}

%option noyywrap
%option yylineno

%%

[uU][a-zA-Z_]*          { yylval.var = strdup(yytext); return VARIABLE; }
[0-9]+(\.[0-9]+)?       { yylval.num = atof(yytext); return NUMBER; }
"sin"                   { return SIN; }
"cos"                   { return COS; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return TIMES; }
"/"                     { return DIVIDE; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"="                     { return EQUALS; }
";"                     { return SEMICOLON; }
[ \t]+                  ; // Ignore whitespace
.                       { return yytext[0]; } // Handle other characters

%%

