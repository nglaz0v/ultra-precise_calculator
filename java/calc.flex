%%

%byaccj

%{
  private Parser yyparser;

  public Yylex(java.io.Reader r, Parser yyparser) {
    this(r);
    this.yyparser = yyparser;
  }
%}

NUM = [0-9]+ ("." [0-9]+)?
NL  = \n | \r | \r\n

%%

/* operators */
"+" | 
"-" | 
"*" | 
"/" | 
"^" | 
"!" | 
"(" | 
")" | 
"="    { return (int) yycharat(0); }

/* functions */
"sin"  { return Parser.SIN; }
"cos"  { return Parser.COS; }
"tan"  { return Parser.TAN; }
"sqrt" { return Parser.SQRT; }

/* newline */
{NL}   { return Parser.NL; }

/* float */
{NUM}  { yyparser.yylval = new ParserVal(Double.parseDouble(yytext()));
         return Parser.NUM; }

/* whitespace */
[ \t]+ { }

\b     { System.err.println("Sorry, backspace doesn't work"); }

/* error fallback */
[^]    { System.err.println("Error: unexpected character '"+yytext()+"'"); return -1; }
