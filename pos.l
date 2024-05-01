%{
#include<stdio.h>
%}


%%
(Destiny) {printf("Noun:%s\n",yytext);}
(Dread|Run|arrives) {printf("verb:%s\n",yytext);}
(it) {printf("pronoun:%s\n",yytext);}
(from) {printf("preposition:%s\n",yytext);}
(same) {printf("adjective:%s\n",yytext);}
(all|the) {printf("Determiner/article:%s\n",yytext);}
[\t\n]+ 


%%
int yywrap(void){}
int main(){
yyin=fopen("input_pos.txt","r");
yylex();
fclose(yyin);
return 0;
}




dusra

/* Definitions and includes */
%{
#include <stdio.h>
%}
/* Regular expressions for identifying parts of speech */
%%
^the$                   { printf("Article\n"); }
^a$                     { printf("Article\n"); }
^an$                    { printf("Article\n"); }
^[A-Za-z]+(ed|ing)$     { printf("Verb\n"); }
^[A-Za-z]+(s|es)$       { printf("Noun\n"); }
^[A-Za-z]+ly$           { printf("Adverb\n"); }
^[A-Za-z]+(able|ible)$  { printf("Adjective\n"); }
^[A-Za-z]+(ment|tion)$  { printf("Noun\n"); }
^[A-Za-z]+$             { printf("Noun\n"); }
.                       { printf("Symbol\n"); } /* Catch-all for unrecognized tokens */
%%
int yywrap(){
	return 1;
}
int main(int argc, char* argv[]) {
    /* Call the generated lexer */
    yylex();
    return 0;
}