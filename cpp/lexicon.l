%option noyywrap

%{
#ifdef __cplusplus
#include "syntax.hh"
#else
#include "syntax.h"
#endif
/* int nlines = 0; */
%}

DIGIT [0-9]
 /* ID [a-zA-Z][a-zA-Z0-9_]* */

%%
{DIGIT}+               { yylval/*.real*/ = atol(yytext); return NUM; }
{DIGIT}+("."{DIGIT}+)? { yylval/*.real*/ = atof(yytext); return NUM; }
 /* "="     { return ASIGN; } */
"("     { return LB; }
")"     { return RB; }
sin     { return SIN; }
cos     { return COS; }
tan     { return TAN; }
sqrt    { return SQRT; }
 /* {ID}    { yylval.str = strdup(yytext); return VAR; } */
"+"     { return PLUS; }
"-"     { return MINUS; }
"*"     { return MULT; }
"/"     { return DIV; }
"^"     { return POW; }
"!"     { return FACT; }
\n      { /* nlines++; */ return EOL; }
[\t ]*  /* throw away whitespace */
.       { /* yyerror("Illegal character"); */ return EOL; }
%%
