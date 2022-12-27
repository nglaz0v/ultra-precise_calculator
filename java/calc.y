%{
  import java.io.*;
%}

%token <dval> NUM
%token PLUS MINUS MULT DIV POW
// %token ASIGN
%token NL
%token LB RB
%token SIN COS TAN SQRT FACT

%type <dval> exp

%left PLUS MINUS
%left MULT DIV
%left NEG
%right POW

%%

input: /* empty string */
    | input line
    ;

line: NL      { if (interactive) System.out.print("Expression: "); }
    | exp NL  { System.out.println(" = " + $1); 
                if (interactive) System.out.print("Expression: "); }
    ;

exp: NUM            { $$ = $1; }
    | exp PLUS exp  { $$ = $1 + $3; }
    | exp MINUS exp { $$ = $1 - $3; }
    | exp MULT exp  { $$ = $1 * $3; }
    | exp DIV exp   { $$ = $1 / $3; }
    | MINUS exp %prec NEG { $$ = -$2; }
    | exp POW exp   { $$ = Math.pow($1, $3); }
    | LB exp RB     { $$ = $2; }
    | SIN LB exp RB { $$ = Math.sin($3); }
    | COS LB exp RB { $$ = Math.cos($3); }
    | TAN LB exp RB { $$ = Math.tan($3); }
    | SQRT LB exp RB { $$ = Math.sqrt($3); }
    | FACT LB exp RB { $$ = 1; for (long i = 1; i <= (long)$3; ++i) $$ *= i; }
    ;

%%

  private Yylex lexer;


  private int yylex () {
    int yyl_return = -1;
    try {
      yylval = new ParserVal(0);
      yyl_return = lexer.yylex();
    }
    catch (IOException e) {
      System.err.println("IO error :"+e);
    }
    return yyl_return;
  }


  public void yyerror (String error) {
    System.err.println ("Error: " + error);
  }


  public Parser(Reader r) {
    lexer = new Yylex(r, this);
  }


  static boolean interactive;

  public static void main(String args[]) throws IOException {
    System.out.println("BYACC/Java with JFlex Calculator Demo");

    Parser yyparser;
    if ( args.length > 0 ) {
      // parse a file
      yyparser = new Parser(new FileReader(args[0]));
    }
    else {
      // interactive mode
      System.out.println("[Quit with CTRL-D]");
      System.out.print("Expression: ");
      interactive = true;
      yyparser = new Parser(new InputStreamReader(System.in));
    }

    yyparser.yyparse();
    
    if (interactive) {
      System.out.println();
      System.out.println("Have a nice day");
    }
  }
