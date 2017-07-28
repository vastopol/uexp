package jsnobol3;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.StringTokenizer;
import java.io.PrintStream;

import tools.*;

import static jsnobol3.Constants.*;

public class Parser
{	
    Lexer tokens = null;

    public Parser()
    {
    }

    public void setLexer(Lexer lex) {this.tokens = lex;}

    //////////////////////////////////////////////////
    // Invariant: every non-terminal leaves the next token as the current one.
    // if non-term proc returns false, then current token is the one
    // in place at the beginning of the proc.
    //////////////////////////////////////////////////

    Token consumed = null;

    Token consumed() {Token t = consumed; return t;}
    Pos pos() {return (consumed != null?consumed.pos:tokens.token().pos);}

    //////////////////////////////////////////////////
    // program: (statement)+ EOF
    boolean program(Ref<AST> astp) throws Error
    {
	parseEnter("program");
	tokens.start();
	AST prev = null;
	AST node = new AST(AstType.PROGRAM,pos());
	Ref<AST> subtree = new Ref<AST>();
	if(!statement(subtree)) return parseExit(false);
	AST stmt = subtree.deref();
	node.add(stmt);
	for(;;) {
	    if(match(TokenType.EOF)) break;
	    if(!statement(subtree))
		throw new Error(parseMsg(node.getPos(),expect("<statement>")));
	    prev = stmt;
	    stmt = subtree.deref();
	    node.add(stmt);
	    // do a check to see that we are not flowing thru to
	    // a function label
	    if(prev != null && stmt.get(1) != null
		&& Main.parms.getBoolean("lint")) {
		// check to see if the label on this statement
		lint1(prev,stmt);
	    }
	}
	astp.set(node);
	return parseExit(true,astp);
    }

    // statement: (LABEL)? (body)? EOL;
    boolean statement(Ref<AST> astp) throws Error
    {
	parseEnter("statement");
	AST stmt = new AST(AstType.STATEMENT,pos());
	Ref<AST> subtree = new Ref<AST>();
	// Add a line number to all STATEMENT instances
	stmt.add(AST.makeleaf(AstType.LINENO,new Integer(tokens.pos.getLine())));
	AST label = null;
	if(match(TokenType.LABEL))
	    label = AST.makeleaf(AstType.LABEL,consumed().text);
	stmt.add(label);
	if(body(subtree,stmt)) {}
	else {
	    // empty statement
	    stmt.add((AST)null); // no pattern part
	    stmt.add((AST)null); // no replacement part
	    stmt.add((AST)null); // no branch part
	}
	if(!match(TokenType.EOL))
	    throw new Error(parseMsg(pos(),expect("EOL")));
	astp.set(stmt);
	return parseExit(true,astp);
    }

    // body: subject (pattern)? (replacement)? (branch)?;
    boolean body(Ref<AST> astp, AST stmt) throws Error
    {
	parseEnter("body");
	Ref<AST> subtree = new Ref<AST>();
	if(!subject(subtree)) return parseExit(false);
	AST subj = subtree.deref();
	boolean replaceable = (subj.atype == AstType.NAME
				|| subj.atype == AstType.REFERENCE);
	stmt.add(subtree); subtree.clear();
	pattern(subtree);
	stmt.add(subtree); subtree.clear();
	if(replacement(subtree) && !replaceable)
	    throw new Error(parseMsg(subj.findpos(),"Subject cannot be replaced"));
	stmt.add(subtree); subtree.clear();
	branch(subtree);
	stmt.add(subtree); subtree.clear();
	astp.set(stmt);
//System.out.println("body="+stmt);
	return parseExit(true,astp);
    }

    // subject: primary
    boolean subject(Ref<AST> astp) throws Error
    {
	parseEnter("subject");
	// Suppress subject nodes from the tree
	astp.clear();
	if(!primary(astp)) return parseExit(false);
	return parseExit(true,astp);
    }

    // replacement: EQUAL (concat)?
    boolean replacement(Ref<AST> astp) throws Error
    {
	parseEnter("replacement");
	// Suppress replacement nodes from the tree
	astp.clear();
	if(!match(TokenType.EQUAL)) return parseExit(false);
	if(!concat(astp)) {
	    astp.set(AST.makeleaf(""));
	}
	return parseExit(true,astp);
    }

    //////////////////////////////////////////////////

    // concat: (expr)+
    boolean concat(Ref<AST> astp) throws Error
    {
	parseEnter("concat");
	Ref<AST> subtree = new Ref<AST>();
	Pos pos = pos();
	if(!expr(subtree)) return parseExit(false);
	// collect all the cat exprs
	AST cats = new AST(AstType.CONCAT,pos);
	cats.add(subtree.deref());
	subtree.clear();
	while(expr(subtree)) {
	    cats.add(subtree.deref());
	    subtree.clear();	    
	}
	// make it so that we drop the concat node if there is only
	// one expression in the concatentation.
	if(cats.size() == 1) {
	    astp.set(cats.get(0)); // only one expr
	} else {
	    astp.set(cats);
	}
	return parseExit(true,astp);
    }

    // expr: term ((PLUS|MINUS) term)*;
    boolean expr(Ref<AST> astp) throws Error
    {
	parseEnter("expr");
	Ref<AST> subtree = new Ref<AST>();
	if(!term(subtree)) return parseExit(false);
	AST exp = subtree.deref();
	Pos pos = null;
	for(;;) {
	    pos = pos();
	    if(match(TokenType.PLUS)) {
	        if(!term(subtree))
		    throw new Error(parseMsg(pos,expect("<term>")));
		exp = new AST(AstType.ADD,pos)
				.add(exp).add(subtree);
	    } else if(match(TokenType.MINUS)) {
	        if(!term(subtree))
		    throw new Error(parseMsg(pos,expect("<term>")));
		exp = new AST(AstType.SUBTRACT,pos)
				.add(exp).add(subtree);
	    } else
		break;
	}
	astp.set(exp);
	return parseExit(true,astp);
    }

    //term: primary ((MULT|DIV) primary)* ;
    boolean term(Ref<AST> astp) throws Error
    {
	parseEnter("term");
	Ref<AST> subtree = new Ref<AST>();
	if(!primary(subtree)) return parseExit(false);
	AST term = subtree.deref();
	Pos pos = null;
	for(;;) {
	    pos = pos();
	    if(match(TokenType.MULT)) {
	        if(!primary(subtree))
		    throw new Error(parseMsg(pos,expect("<primary>")));
		term = new AST(AstType.MULT,pos).add(term).add(subtree);
	    } else if(match(TokenType.DIV)) {
	        if(!primary(subtree))
		    throw new Error(parseMsg(pos,expect("<primary>")));
		term = new AST(AstType.DIV,pos).add(term).add(subtree);
	    } else
		break;
	}
	astp.set(term);
	return parseExit(true,astp);
    }

    // var: reference | NAME;
    boolean var(Ref<AST> astp) throws Error
    {
	parseEnter("var");
	if(match(TokenType.NAME)) {
	    astp.set(AST.makeleaf(AstType.NAME,consumed().text));
	} else if(reference(astp)) {}
	else
	    return parseExit(false);
	return parseExit(true,astp);
    }

    // reference: DOLLAR primary
    boolean reference(Ref<AST> astp) throws Error
    {
	parseEnter("reference");
	Ref<AST> subtree = new Ref<AST>();
	AST node;
	if(!match(TokenType.DOLLAR)) return parseExit(false);
	node = new AST(AstType.REFERENCE,pos());
	if(!primary(subtree))
	    throw new Error(parseMsg(subtree.deref().findpos(),expect("<primary>")));
	node.add(subtree);
	astp.set(node);
	return parseExit(true,astp);
    }

    //primary: atom | LPAREN unary RPAREN
    boolean primary(Ref<AST> astp) throws Error
    {
	parseEnter("primary");
	Ref<AST> subtree = new Ref<AST>();
	AST node;
	if(match(TokenType.LPAREN)) {
	    if(!unary(subtree))
		throw new Error(parseMsg(pos(),expect("<unary>")));
	    if(!match(TokenType.RPAREN))
		throw new Error(parseMsg(pos(),expect("RPAREN")));
	    node = subtree.deref();
	} else if(atom(subtree)) {
	    node = subtree.deref();	    
	} else
	    return parseExit(false);
	astp.set(node);
	return parseExit(true,astp);
    }

    //unary: (PLUS|MINUS)? concat
    boolean unary(Ref<AST> astp) throws Error
    {
	parseEnter("atom");
	Ref<AST> subtree = new Ref<AST>();
	AST node;
	Pos pos = pos();
	if(match(TokenType.PLUS)) {
	    pos = pos();
	    if(!concat(subtree))
		throw new Error(parseMsg(pos,expect("<concat>")));
	    node = subtree.deref();
	} else if(match(TokenType.MINUS)) {
	    pos = pos();
	    if(!concat(subtree))
		throw new Error(parseMsg(pos,expect("<concat>")));
	    node = new AST(AstType.NEGATE,pos).add(subtree);
	} else if(concat(subtree)) {
	    node = subtree.deref();
	} else
	    return parseExit(false);
	astp.set(node);
	return parseExit(true,astp);
    }

    // atom: STRING | var | fcncall ;
    boolean atom(Ref<AST> astp) throws Error
    {
	parseEnter("atom");
	Ref<AST> subtree = new Ref<AST>();
	AST node;
	if(match(TokenType.STRING)) {
	    node = AST.makeleaf(consumed().text).setPos(pos());
	} else if(var(subtree)) {
	    node = subtree.deref();
	} else if(fcncall(subtree)) {
	    node = subtree.deref();
	} else
	    return parseExit(false);
	astp.set(node);
	return parseExit(true,astp);
    }

    // fcncall: FCNNAME LPAREN (args)? RPAREN
    boolean fcncall(Ref<AST> astp)
	throws Error
    {
	parseEnter("fcncall");
	Ref<AST> subtree = new Ref<AST>();
	if(!match(TokenType.FCNNAME)) return parseExit(false);
	Token fcnname = consumed();
	Pos pos = pos();
	if(!match(TokenType.LPAREN))
	    throw new Error(parseMsg(pos,expect("LPAREN")));
	AST node = new AST(AstType.FCNCALL,pos);
	node.add(AST.makeleaf(AstType.NAME,fcnname.text));
	AST args;
	pos = pos();
	if(arglist(subtree))
	    args = subtree.deref();
	else
	    args = new AST(AstType.ARGLIST);
	node.add(args);
	if(!match(TokenType.RPAREN))
	    throw new Error(parseMsg(pos(),expect("RPAREN")));
	// Do some context sensitive checks
	Function f = Snobol3.functions.get(fcnname.text);
	if(f == null)
	    throw new Error(parseMsg(node.getPos(),"Undefined function:"+fcnname.text));
	// check for proper number of args: too many is error, too few ok
	if(f.nargs() < args.size())
	    throw new Error(parseMsg(fcnname.getPos(),"Too many args: "+fcnname));
	// Add extra arguments to match nargs
	for(int n=args.size();n<f.nargs();n++) {
	    args.add(AST.makeleaf(""));
	}
	// Now, if this is the DEFINE function, partially process it now
	// rather than at runtime.
	if(f.name.equalsIgnoreCase("define")) {
	    PrimFunction pdef = (PrimFunction)Snobol3.functions.get("define");
	    Define deff = (Define)pdef.prim;
	    deff.pass2(node);
	}
	astp.set(node);
	return parseExit(true,astp);
    }

    // arglist: concat (COMMA concat)*
    boolean arglist(Ref<AST> astp) throws Error
    {
	parseEnter("arglist");
	Ref<AST> subtree = new Ref<AST>();
	AST node = new AST(AstType.ARGLIST);
	if(!concat(subtree)) return parseExit(false);
	node.add(subtree);
	while(match(TokenType.COMMA)) {
	    Pos pos = pos();
	    subtree.clear();
	    if(!concat(subtree))
		throw new Error(parseMsg(pos,expect("<concat>")));
	    node.add(subtree);	    
	}
	astp.set(node);
	return parseExit(true,astp);
    }

    // pattern: (pattest)+
    boolean pattern(Ref<AST> astp) throws Error
    {
	parseEnter("pattern");
	Ref<AST> subtree = new Ref<AST>();
	AST node = new AST(AstType.PATTERN);
	if(!pattest(subtree)) return parseExit(false);
	do { node.add(subtree); subtree.clear();} while(pattest(subtree));
	astp.set(node);
	return parseExit(true,astp);
    }

    // pattest: patvar | expr ;
    boolean pattest(Ref<AST> astp) throws Error
    {
	parseEnter("pattest");
	Ref<AST> subtree = new Ref<AST>();
	if(patvar(astp)) return parseExit(true,astp);
	if(expr(astp)) return parseExit(true,astp);
	astp.clear();
	return parseExit(false);
    }

    // patvar: STAR patmatch STAR
    boolean patvar(Ref<AST> astp) throws Error
    {
	parseEnter("patvar");
	Ref<AST> subtree = new Ref<AST>();
	if(!match(TokenType.STAR)) return parseExit(false);
	astp.clear();
	if(!patmatch(astp))
	    throw new Error(parseMsg(pos(),expect("<patmatch>")));
	if(!match(TokenType.STAR))
	    throw new Error(parseMsg(pos(),expect("STAR")));
	return parseExit(true,astp);
    }

    //patmatch:   (var)? (SLASH primary | BAR fcncall)?
    //          | LPAREN (var)? RPAREN ;
    boolean patmatch(Ref<AST> astp) throws Error
    {
	parseEnter("patmatch");
	Ref<AST> subtree = new Ref<AST>();
	AST node = null;
	AST var = null;
	if(match(TokenType.LPAREN)) {
	    var(subtree);
	    var = subtree.deref();
	    node = new AST(AstType.BALANCE);
	    node.add(var);
	    if(!match(TokenType.RPAREN))
		throw new Error(parseMsg(pos(),expect("RPAREN")));
	} else {
	    var(subtree);
	    var = subtree.deref();
	    if(match(TokenType.SLASH)) {
		node = new AST(AstType.LEN);
		node.add(var);
		subtree.clear();
		if(!primary(subtree))
		    throw new Error(parseMsg(pos(),expect("<primary>")));
		node.add(subtree);
	    } else if(match(TokenType.BAR)) {
		node = new AST(AstType.PATFCN);
		node.add(var);
		subtree.clear();
		if(!patfcn(subtree))
		    throw new Error(parseMsg(pos(),expect("<pattern function>")));
		node.add(subtree);
	    } else {
		node = new AST(AstType.ARB);
		node.add(var);
	    }
	}
	astp.set(node);
	return parseExit(true,astp);
    }

    // patfcn: FCNNAME LPAREN (args)? RPAREN
    boolean patfcn(Ref<AST> astp)
	throws Error
    {
	parseEnter("patfcn");
	Ref<AST> subtree = new Ref<AST>();
	if(!match(TokenType.PATFCNNAME)) return parseExit(false);
	Token fcnname = consumed();
	if(!match(TokenType.LPAREN))
	    throw new Error(parseMsg(pos(),expect("LPAREN")));
	AST node = new AST(AstType.FCNCALL,fcnname.getPos());
	node.add(AST.makeleaf(AstType.NAME,fcnname.text));
	AST args;
	if(arglist(subtree))
	    args = subtree.deref();
	else
	    args = new AST(AstType.ARGLIST);
	node.add(args);
	if(!match(TokenType.RPAREN))
	    throw new Error(parseMsg(pos(),expect("RPAREN")));
	// Do some context sensitive checks
	PatternOp pop = Snobol3.patterns.get(fcnname.text.toLowerCase());
	// check for proper number of args: too many is error, too few ok
	if(pop.nargs() < args.size())
	    throw new Error(parseMsg(pos(),"Too many actuals: "+fcnname));
	// Add extra arguments to match nargs
	for(int n=args.size();n<pop.nargs();n++) {
	    args.add(AST.makeleaf(""));
	}
	astp.set(node);
	return parseExit(true,astp);
    }

    // Unclear from primer exactly what combinations
    // are allowed; I assume the following
    // branch: SLASH (s (f)? | f (s)? | go);
    boolean branch(Ref<AST> astp) throws Error
    {
	parseEnter("branch");
	if(!match(TokenType.SLASH)) return parseExit(false);
	Pos pos = pos();
	Ref<AST> gotree = new Ref<AST>();
	Ref<AST> stree = new Ref<AST>();
	Ref<AST> ftree = new Ref<AST>();
	if(s(stree)) {f(ftree);}
	else if(f(ftree)) {s(stree);}
	else if(go(gotree)) {}
	else
	    throw new Error(parseMsg(pos,expect("<branch>")));
	AST node = new AST(AstType.BRANCH,pos);
	node.add(gotree.deref());
	node.add(stree.deref());
	node.add(ftree.deref());
	astp.set(node);
	return parseExit(true,astp);
    }

    // s: SUCCESS LPAREN dest RPAREN
    boolean s(Ref<AST> astp) throws Error
    {
	parseEnter("s");
	// We allow s() to also be a function name
	if(!match(TokenType.SUCCESS)) return parseExit(false);
	if(!match(TokenType.LPAREN)) 
	    throw new Error(parseMsg(pos(),expect("LPAREN")));
	astp.clear();
	if(!dest(astp))
	    throw new Error(parseMsg(pos(),expect("<label>")));
	if(!match(TokenType.RPAREN)) 
	    throw new Error(parseMsg(pos(),expect("RPAREN")));
	// just past the dest subtree up
	return parseExit(true,astp);
    }

    // f: FAIL LPAREN dest RPAREN
    boolean f(Ref<AST> astp) throws Error
    {
	parseEnter("f");
	// We allow f() to also be a function name
	if(!match(TokenType.FAIL)) return parseExit(false);
	if(!match(TokenType.LPAREN)) 
	    throw new Error(parseMsg(pos(),expect("RPAREN")));
	astp.clear();
	if(!dest(astp))
	    throw new Error(parseMsg(pos(),expect("<label>")));
	if(!match(TokenType.RPAREN)) 
	    throw new Error(parseMsg(pos(),expect("RPAREN")));
	return parseExit(true,astp);
    }

    // go: LPAREN dest RPAREN
    boolean go(Ref<AST> astp) throws Error
    {
	parseEnter("go");
	if(!match(TokenType.LPAREN)) return parseExit(false);
	astp.clear();
	if(!dest(astp))
	    throw new Error(parseMsg(pos(),expect("<dest>")));
	if(!match(TokenType.RPAREN))
	    throw new Error(parseMsg(pos(),expect("RPAREN")));
	return parseExit(true,astp);
    }


    //dest: label | RETURN | FRETURN | END | reference;
    boolean dest(Ref<AST> astp) throws Error
    {
	parseEnter("dest");
	Ref<AST> subtree = new Ref<AST>();
	if(match(TokenType.RETURN)
	   || match(TokenType.FRETURN)
	   || match(TokenType.END)) {
	    astp.set(AST.makeleaf(AstType.LABEL,consumed().text).setPos(pos()));
	} else if(label(subtree))
	    astp.set(subtree);
	else if(reference(subtree))
	    astp.set(subtree);
	else
	    return parseExit(false);
	return parseExit(true,astp);
    }

    //label: LABEL | NAME;
    boolean label(Ref<AST> astp) throws Error
    {
	parseEnter("label");
	if(match(TokenType.LABEL)
	   || match(TokenType.NAME)) {
	    astp.set(AST.makeleaf(AstType.LABEL,consumed().text).setPos(pos()));
	    return parseExit(true,astp);
	}
	return parseExit(false);
    }

    // Following is used by Define.java and defines
    // the structure of the first argument to the DEFINE function.
    // fcndecl: NAME LPAREN (NAME (COMMA (NAME)?)*)? RPAREN;
    boolean fcndecl(Ref<AST> astp) throws Error
    {
	if(!match(TokenType.NAME)) return parseExit(false);
	Token fname = consumed();
	AST def = new AST(AstType.DEFINITION,pos())
			.add(AST.makeleaf(fname.text));
	if(!match(TokenType.LPAREN))
	    throw new Error(parseMsg(pos(),expect("LPAREN")));
	AST formals = new AST(AstType.NAMELIST);
	if(match(TokenType.NAME)) {
	    for(;;) {
		formals.add(new LeafAST(AstType.NAME).setValue(consumed().text));
	        if(match(TokenType.RPAREN)) break;
	        if(!match(TokenType.COMMA))
	            throw new Error(parseMsg(pos(),expect("COMMA")));
	        if(!match(TokenType.NAME))
	            throw new Error(parseMsg(pos(),expect("NAME(as STRING)")));
	    }
	} else if(!match(TokenType.RPAREN))
	    throw new Error(parseMsg(pos(),expect("RPAREN")));
	def.add(formals);
	astp.set(def);
	return parseExit(true,astp);
    }

    //////////////////////////////////////////////////
    // convenience functions

    Token lasttoken = null;

    // Leave the matched token in consumed()
    boolean match(TokenType test) throws Error
    {
	Token token1 = tokens.token();
	if(token1.ttype != test) return false;
	consumed = token1;
	Token token2 = tokens.advance();
//System.err.println("match: "+token1+" -> "+token2.toString());
	return true;
    }

    String parseMsg(Pos pos,String msg)
    {
	return (pos.toString()+"; "+msg);
    }

    String expect(String expected) {return expect(expected,consumed());}

    String expect(String expected, Token t)
    {
	return ("expected "+expected + "; saw: "+ t);
    }

/*
    String markColumn(String mark, int col)
    {
	if(col <= 0) mark = "^"+mark;
	else if(col >= mark.length()) mark = mark+"^";
	else
	    mark = mark.substring(0,col)
		   +"^"
		   +mark.substring(col+1,mark.length());
	
	return mark;
    }

*/
    //////////////////////////////////////////////////

    public ParseStack parseStack = new ParseStack();

    boolean parseExit(boolean b, Ref<AST> astp)
    {
	parseExit(b);
	if(astp != null && Debug.isWhen(DebugPoint.PARSE))
	    Debug.Println(astp.deref().toString());
	return b;
    }

    void parseEnter(String proc)
    {
	if(Debug.isWhen(DebugPoint.PARSE)) {
	    try {
	    Debug.Print(Debug.indentin(parseStack.depth()));
	    Debug.Print(" "+proc+" "+parseStack);
	    Debug.Println();
	    Debug.Println("token="+tokens.token());
	    } catch(Exception e) {Debug.Println(e);}
	}
	parseStack.push(proc);
    }
 
    public boolean parseExit(boolean b)
    {
	String proc = parseStack.pop();
	if(Debug.isWhen((DebugPoint.PARSE))) {
	    Debug.Print(Debug.indentout(parseStack.depth()));
	    Debug.Print(" "+proc);
	    Debug.Print(b?" [success]":" [fail]");
	    Debug.Print(" "+parseStack);
	    Debug.Println();
	    Debug.Println("token="+tokens.token());
	}
	return b;
    }

    //////////////////////////////////////////////////
    // lint checks in parser

    void lint1(AST stmt1, AST stmt2)
    {
	// Stmt1 is followed by a line with a label that begins a function.
	// See if stmt1 can fall thru into the function body
	AST lineno = stmt1.get(0);
	AST branch = stmt1.get(5);
	AST go = (branch==null?null:branch.get(0));
	AST s = (branch==null?null:branch.get(1));
	AST f = (branch==null?null:branch.get(2));
	if(go == null && (s == null || f == null)) {
	    Label l = Snobol3.labels.get(stmt2.get(1).text());
	    if(l.getFunction() != null)
		Debug.Println("WARNING: line "+ lineno.text()
			   +": may fall thru into function body label "+l.name);
	}
    }

}

