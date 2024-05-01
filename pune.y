%{
    #include"lex.yy.c"
    #include<stdio.h>
    int valid=1;
    int yyerror();
%}

%token digit letter
%%

start : letter s

s :     letter s

      | digit s

      |

      ;
%%

int yyerror()
{
    printf("\nIts not a valid variable name!\n");
    valid=0;
    return 0;
}
int main()
{

    printf("\nEnter a variable name to tested ");
    yyparse();
    if(valid)
    {
        printf("\nIt is a valid variable name!\n");
    }
}