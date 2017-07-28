package jsnobol3;

import java.util.HashMap;
import java.util.ArrayList;

import tools.*;
/*
import static jsnobol3.Constants.*;
*/

class Pass2
{
    ArrayList<Statement> stmts;

    Lexer lex;
    Parser parser;
    S3Reader s3r;

    public Pass2(Parser parser, Lexer lex, S3Reader s3r)
    {
	this.parser = parser;
	this.lex = lex;
	this.s3r = s3r;
    }

     public AST pass2(ArrayList<Statement> stmts) throws Error
    {
	this.stmts = stmts;
	// Add special labels
	Label endl = new Label("end",stmts.size());
	Snobol3.labels.put("end",endl);
	s3r.open(stmts);
	Ref<AST> astp = new Ref<AST>();
	parser.program(astp);
	return astp.deref();
    }

}
