%option noyywrap
%{
#ifdef __cplusplus
#include "syntax.hh"
#else
#include "syntax.h"
#endif
int nlines = 0;
%}
DIGIT [0-9]
 /* ID [a-zA-Z][a-zA-Z0-9_]* */
%%
{DIGIT}+("."{DIGIT}+)? { yylval.real = atof(yytext); return(TKN_NUM); }
=       { return TKN_ASIGN; }
\(      { return TKN_PAA; }
\)      { return TKN_PAC; }
sin     { return TKN_SIN; }
cos     { return TKN_COS; }
tan     { return TKN_TAN; }
sqrt    { return TKN_SQRT; }
 /* {ID}    { yylval.str = strdup(yytext); return TKN_ID; } */
\+      { return TKN_PLUS; }
\-      { return TKN_MINUS; }
\*      { return TKN_MULT; }
\/      { return TKN_DIV; }
\^      { return TKN_POW; }
\!      { return TKN_FACT; }
\n      { nlines++; return '\n'; }
.
%%
