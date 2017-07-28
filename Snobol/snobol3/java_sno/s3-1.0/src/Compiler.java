package jsnobol3;

import java.util.HashMap;
import java.util.ArrayList;
/*
import java.util.Iterator;
import java.io.BufferedReader;
import java.io.IOException;
*/

//import tools.*;

//import static jsnobol3.Constants.*;

public class Compiler
{	
    // Compilation operates in multiple passes:
    // pass 1: read the whole program and process labels,
    //         continuations, comments, and control words.
    // pass 2: compile each program to a tree form
    // pass 2: compile tree to a sequence of operator objects

//    static final int EOF = -1;
//    static final char EOLCHAR = '\n';

    ArrayList<Statement> stmts;
    Lexer lex = null;
    Parser parser = null;
    S3Reader s3r = null;

    Pass1 pass1;
    Pass2 pass2;
    Pass3 pass3;

    public Compiler()
    {
	stmts = null;
	s3r = new S3Reader();
	lex = new Lexer();
	lex.setSource(s3r);
	parser = new Parser();
	parser.setLexer(lex);

	pass1 = new Pass1();
	pass2 = new Pass2(parser,lex,s3r);
	pass3 = new Pass3();
    }

    public Program compile(String fname)
	throws Error
    {
	stmts = pass1.pass1(fname);
	AST prog = pass2.pass2(stmts);
	Debug.when(DebugPoint.PARSE).println("AST: pass2:");
	Debug.when(DebugPoint.PARSE).println(prog.prettyPrint());
	Program code = pass3.pass3(prog,stmts.size());
	return code;	
    }

}

class Statement
{
    int lineno = 0;
    String text = null;
    String label = null;
    public Statement() {}
    public String toString()
	{return String.format("[%d] %s: |%s|",lineno,label,text);}
}
