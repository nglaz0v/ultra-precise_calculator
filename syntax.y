%{
#include <stdio.h>
#include <math.h>

extern int yylex(void);
// extern char *yytext;
extern int nlines;
extern FILE *yyin;

void yyerror(const char *msg);
/*
struct symbol_entry {
    char name[50];
    float value;
};
void put_symbol(char *name, float value) {}
struct symbol_entry *get_symbol(char *name) {return NULL;}
*/
%}
%union
{
//    char* str;
    float real;
}
%start Calculator
%token <real> TKN_NUM
%token TKN_ASIGN
%token TKN_PAA
%token TKN_PAC
%token TKN_SIN TKN_COS TKN_TAN TKN_SQRT
// %token <str> TKN_ID
%type <real> Calculator
%type <real> Expression
%left TKN_PLUS TKN_MINUS
%left TKN_MULT TKN_DIV
%left TKN_POW
%%
Calculator: { $$ = 0; }
    | Calculator Expression '\n' { printf("%f\n", $2); }
    | Calculator '\n'
    ;
Expression: TKN_NUM { $$=$1; }
//    | TKN_ID { $$ = get_symbol($1)->value; }
//    | TKN_ID TKN_ASIGN Expression { put_symbol($1, $3); $$ = $3; }
    | Expression TKN_PLUS Expression { $$ = $1 + $3; }
    | Expression TKN_MINUS Expression { $$ = $1 - $3; }
    | Expression TKN_MULT Expression { $$ = $1 * $3; }
    | Expression TKN_DIV Expression { $$ = $1 / $3; }
    | Expression TKN_POW Expression { $$ = pow($1, $3); }
    | TKN_PAA Expression TKN_PAC { $$ = $2; }
    | TKN_SIN TKN_PAA Expression TKN_PAC { $$ = sin($3); }
    | TKN_COS TKN_PAA Expression TKN_PAC { $$ = cos($3); }
    | TKN_TAN TKN_PAA Expression TKN_PAC { $$ = tan($3); }
    | TKN_SQRT TKN_PAA Expression TKN_PAC { $$ = sqrt($3); }
    ;
%%
void yyerror(const char *msg)
{
    fprintf(stderr, "ERROR: %s\n", msg);
}

int main(int argc, char **argv)
{
    yyin = (argc > 1) ? fopen(argv[1], "rt") : stdin;
    yyparse();
    return 0;
}
