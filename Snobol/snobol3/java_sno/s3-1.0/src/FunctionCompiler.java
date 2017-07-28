package jsnobol3;

import tools.*;

class FunctionCompiler
{
    StringBufferReader rdr;
    Lexer lex;
    Parser parser;
    Pass3 pass3;

    public FunctionCompiler()
    {
	rdr = new StringBufferReader();
	lex = new Lexer();
	lex.setSource(rdr);
	lex.labelsok = false;
	parser = new Parser();
	parser.setLexer(lex);
	pass3 = new Pass3();
    }

    public void open(String text, int lineno)
	throws Error
    {
	rdr.open(text);
	rdr.setLine(lineno);
	lex.reset();
	lex.advance(); // prime token stream
    }
}
