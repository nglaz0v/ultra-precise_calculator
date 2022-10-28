/* Prologue (directives). */

/* Disable Flex features we don't need, to avoid warnings. */
%option noinput nounput noyywrap

%{
#ifdef __cplusplus
#include "parser.hh"
#else
#include "parser.h"
#endif
%}

%%
 /* Rules. */

" "       ;
[a-z]	{
			int c = yytext[0];
			yylval.a = c - 'a';
			return(LETTER);
		}
[0-9]	{
			int c = yytext[0];
			yylval.a = c - '0';
			return(DIGIT);
		}
[^a-z0-9\b]	{
			int c = yytext[0];
			return(c);
		}
%%
/* Epilogue. */
