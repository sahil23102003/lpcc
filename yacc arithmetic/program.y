%{
#include<stdio.h>
int flag = 0;

int yylex();
void yyerror(const char *s);
%}

%token NUMBER
%left '+''-'
%left '*''/'

%%

ArithmeticExpression: expression{
	printf("\nResult=%d\n",$$);
	return 0;
}

expression : expression '+' expression {$$ = $1 + $3 ;}
	| expression '-' expression {$$ = $1 - $3 ;}
	| expression '*' expression {$$ = $1 * $3 ;}
	| expression '/' expression {$$ = $1 / $3 ;}
	| '(' expression ')' {$$ = $2;}
	| NUMBER {$$ = $1;}
	;
%%

int main(){
	printf("Enter an arithmatic expression: ");
	yyparse();
	if(flag == 0)
	{
		printf("Expression is correct");
	}
	return 0;
}

void yyerror(const char *s){
	printf("Error occured");
	flag = 1;
}
