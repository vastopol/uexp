class S3Parser extends Parser;

program: (statement)+ EOF;
statement:   (LABEL)? (body)?  EOL;
body: subject (pattern)? (replacement)? (branch)?;
subject: primary;
replacement: EQUAL (concat)?;

concat: (expr)+;
expr: term ((PLUS|MINUS) term)*;
term: primary ((MULT|DIV) primary)*;
primary: atom | LPAREN unary RPAREN ;
unary: concat | (PLUS|MINUS) concat ;
atom: var | STRING | fcncall;

var: reference | NAME;
reference: DOLLAR primary;

fcncall: FCNNAME args ;
args: LPAREN (arglist)? RPAREN ;
arglist: concat (COMMA concat)* ;

pattern: (pattest)+;
pattest: patvar | expr;
patvar:   STAR patmatch STAR ;
patmatch:   (var)? (SLASH primary | BAR patfcn)? | LPAREN (var)? RPAREN ;
patfcn: PATFCNNAME args ;

branch: SLASH (s (f)? | f (s)? | go);

s: SUCCESS LPAREN dest RPAREN ;
f: FAIL LPAREN dest RPAREN ;
go: LPAREN dest RPAREN ;
dest: label| RETURN | FRETURN | END | reference;
label: LABEL | NAME;

fcndecl:  NAME LPAREN (formals)? RPAREN;
formals:  namelist;
locals:   namelist;
namelist: NAME (COMMA (NAME)?)*;
