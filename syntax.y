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

%token <real> NUM
%token ASIGN
%token PAA
%token PAC
%token SIN COS TAN SQRT FACT
// %token <str> VAR

%type <real> Calculator
%type <real> Expression

%left PLUS MINUS
%left MULT DIV
%left POW
%left UMINUS

%%
Calculator: { $$ = 0; }
    | Calculator Expression '\n' { printf("%f\n", $2); }
    | Calculator '\n'
    ;
Expression: NUM { $$=$1; }
//    | VAR { $$ = get_symbol($1)->value; }
//    | VAR ASIGN Expression { put_symbol($1, $3); $$ = $3; }
    | Expression PLUS Expression { $$ = $1 + $3; }
    | Expression MINUS Expression { $$ = $1 - $3; }
    | Expression MULT Expression { $$ = $1 * $3; }
    | Expression DIV Expression { $$ = $1 / $3; }
    | Expression POW Expression { $$ = pow($1, $3); }
    | MINUS Expression %prec UMINUS { $$ = -$2; }
    | PAA Expression PAC { $$ = $2; }
    | SIN PAA Expression PAC { $$ = sin($3); }
    | COS PAA Expression PAC { $$ = cos($3); }
    | TAN PAA Expression PAC { $$ = tan($3); }
    | SQRT PAA Expression PAC { $$ = sqrt($3); }
    | FACT PAA Expression PAC { $$ = 1; for (unsigned i = 1; i <= unsigned($3); ++i) $$ *= i; }
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
