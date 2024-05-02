%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%option noyywrap

%%
[0-9]+      { yylval.ival = atoi(yytext); return NUMBER; }
\"[^\"]*\"  { yylval.sval = strdup(yytext); return STRING; }
"sqrt"      { return SQRT; }
"strlen"    { return STRLEN; }
"="         { return '='; }
[ \t\n]+    { /* ignore whitespace */ }
.           { return yytext[0]; }

%%

