%{
#include "y.tab.h"
#include <string.h> // Include for strdup function
%}

%%

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext); // Copy string to pass to Bison
    return IDENTIFIER;
}
[ \t]+ ; // Ignore spaces and tabs
\n     { return EOL; } // End of line, to process input
.      { return yytext[0]; } // Catch-all for any other character

%%

int yywrap() {
    return 1;
}

