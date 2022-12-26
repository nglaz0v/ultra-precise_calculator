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

%token <real> NUM
%token PLUS MINUS MULT DIV POW
// %token ASIGN
%token EOL
%token LB RB
%token SIN COS TAN SQRT FACT
// %token <str> VAR

%type <real> exp

%left PLUS MINUS
%left MULT DIV
%left POW
%left UMINUS

%%
input :
    | input line
    ;

line : EOL
    | exp EOL { printf("%f\n", $1); }

exp: NUM { $$=$1; }
//    | VAR { $$ = get_symbol($1)->value; }
//    | VAR ASIGN exp { put_symbol($1, $3); $$ = $3; }
    | exp PLUS exp { $$ = $1 + $3; }
    | exp MINUS exp { $$ = $1 - $3; }
    | exp MULT exp { $$ = $1 * $3; }
    | exp DIV exp { $$ = $1 / $3; }
    | exp POW exp { $$ = pow($1, $3); }
    | MINUS exp %prec UMINUS { $$ = -$2; }
    | LB exp RB { $$ = $2; }
    | SIN LB exp RB { $$ = sin($3); }
    | COS LB exp RB { $$ = cos($3); }
    | TAN LB exp RB { $$ = tan($3); }
    | SQRT LB exp RB { $$ = sqrt($3); }
    | FACT LB exp RB { $$ = 1; for (long i = 1; i <= long($3); ++i) $$ *= i; }
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
