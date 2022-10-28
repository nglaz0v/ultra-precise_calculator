// Prologue (directives).

%{
#include <stdio.h>

int regs[26];
int base;

int yylex(void);
void yyerror(const char *msg);
%}

%union { int a; }

%token <a> DIGIT LETTER
%type <a> stat expr number

// Precedence (from lowest to highest) and associativity.
%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS  /*supplies precedence for unary minus */

%%
// Rules.
list:   list stat  '\n'
         |
        list error '\n'
         {
           yyerrok;
         }
         | /*empty */
         ;

stat:    expr
         {
           printf("%d\n",$1);
         }
         |
         LETTER '=' expr
         {
           regs[$1] = $3;
         }
         ;

expr:    '(' expr ')'
         {
           $$ = $2;
         }
         |
         expr '*' expr
         {
           $$ = $1 * $3;
         }
         |
         expr '/' expr
         {
           $$ = $1 / $3;
         }
         |
         expr '%' expr
         {
           $$ = $1 % $3;
         }
         |
         expr '+' expr
         {
           $$ = $1 + $3;
         }
         |
         expr '-' expr
         {
           $$ = $1 - $3;
         }
         |
         expr '&' expr
         {
           $$ = $1 & $3;
         }
         |
         expr '|' expr
         {
           $$ = $1 | $3;
         }
         |
        '-' expr %prec UMINUS
         {
           $$ = -$2;
         }
         |
         LETTER
         {
           $$ = regs[$1];
         }
         |
         number
         ;

number:  DIGIT
         {
           $$ = $1;
           base = ($1==0) ? 8 : 10;
         }       
         |
         number DIGIT
         {
           $$ = base * $1 + $2;
         }
         ;

%%
// Epilogue (C code).
int __main(void)
{
  return(yyparse());
}

void yyerror(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
}
