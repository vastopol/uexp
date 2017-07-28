package jsnobol3;

import tools.*;

import java.util.HashMap;
import java.io.IOException;

import static jsnobol3.Constants.*;

class Lexer
{
    Token token = null;
    TokenType prev = null;

    Pos pos = new Pos().set(0,0);
    StringBuilder text = new StringBuilder();

    StringBufferReader rdr;

    boolean pushedback = true;
    boolean labelsok = true;

    boolean dquoteOK;
    boolean escapesOK;

    public Lexer()
    {
	reset();
    }

    void reset()
    {
	token = null;
	prev = null;
	text.setLength(0);
	pos.set(0,0);
	pushedback=false;
    }

    public Token token() {return token;}
    public void pushBack()
    {
	if(pushedback) {
	    System.err.println("double pushback");
	    new Exception().printStackTrace();
	    System.exit(1);
	}
	pushedback = true;
    }

    public void setSource(StringBufferReader rdr) {this.rdr = rdr;}

    public String remainder() {return rdr.remainder();}

    // all start grammar symbols should call this entry
    public void start() throws Error
    {
	dquoteOK = Main.parms.getBoolean("dquotes");
	escapesOK = Main.parms.getBoolean("escapes");
	if(token == null) advance();
    }

    public Token advance() throws Error
    {
	token = nextToken();
Debug.when(DebugPoint.TOKENS).println("token="+token);
	return token;
    }

    public Token nextToken() throws Error
    {
	TokenType ttype = TokenType.NULL;

	if(pushedback) {pushedback = false; return token;}

	try {
	
        // may need to try several times to get a lexeme
	for(;;) {
	    prev = ttype;
	    ttype = TokenType.NULL;
	    text.setLength(0);
	    int ch = rdr.read();
	    pos.set(rdr.getPos()); // get pos of token start
	    if(ch == EOFCHAR) {
		ttype = TokenType.EOF;
	    } else if(ch == EOLCHAR) {
		ttype = TokenType.EOL;
		text.append((char)ch);
	    } else if(isWS(ch)) {
		skipWS(rdr);
		ttype = TokenType.WHITESPACE;
	    } else if(ch == SQUOTE || (ch == DQUOTE && dquoteOK)) {
		// Snobol3 uses single quotes and does not allow
		// escape characters
		// resulting string does not include quotes
		// We provide a flag to extend this.
		int endq = ch;
		if(escapesOK) {
		    QuotedString.removeEscapes(rdr,text,(char)endq);
		} else { // do simple case
		    for(;;) {
		        ch = rdr.read();
		        if(ch == EOFCHAR || ch == EOLCHAR)
			    throw new Error(lexMsg(pos,"Unterminated quoted string: '"+text+"'"));
			if(ch == endq) break;
			text.append((char)ch);
		    }
		}
		ttype=TokenType.STRING;
	    } else if(isNameChar1(ch)) {
		do {
		    text.append((char)ch); ch = rdr.read();
		} while(isNameChar2(ch));
		rdr.pushback();
		// check for: keyword, label, patternfcn, fcnname, or name
		// in that order
		// The primer indicates that a var can have
		// the same name as a label, this is provided for
		// by allowing a name everywhere a label is allowed,
		// and disambiguating in the parser.
		// This means that the lexer will only return a
		// LABEL token when it knows it is one by
		//  checking token position for column one.

		// Function name disambiguation is also an issue.
		// 1. as name for invoking function: e.g. f(x)
		// 2. as the return value within the function: e.g. f = '5'
		// #2 implies we must treat function names as variables
		// when they are not invoking the function.
		// We disambiguate as follows:
		// a. if the name is immediately followed by a
		//    left paren and is in the function map, it is case 1.
		// b. otherwise, it is case 2.

		String txt = text.toString();
		if(isKeyWord(txt))
		    ttype = keywordFor(txt);
		else if(labelsok && isLabel(txt) && pos.getColumn()==1)
		    ttype = TokenType.LABEL;
		else if(isPatternFunction(txt))
		    ttype = TokenType.PATFCNNAME;
		else if(isFunction(txt) && rdr.peek() == LPARENCHAR)
		    ttype = TokenType.FCNNAME;
		else
		    ttype = TokenType.NAME;		
	    } else if(isDelimiter(ch)) {
		text.append((char)ch);
		ttype = TokenType.delimiterFor(text);
		// special case STAR vs MULT and SLASH vs DIV
		ch = rdr.peek();
//System.err.println("ch="+((char)ch)+"current="+token+" prev="+prev);
		if(ttype == TokenType.STAR) {
		    if(prev == TokenType.WHITESPACE && isWS(ch))
			ttype = TokenType.MULT;
		} else if(ttype == TokenType.SLASH) {
		    if(prev == TokenType.WHITESPACE && isWS(ch))
			ttype = TokenType.DIV;
		} else if(ttype == TokenType.PLUS || ttype == TokenType.MINUS) {
		    // check that PLUS and MINUS have surrounding whitespace
		    // (except that we cheat and allow LPAREN on left)
		    if(!isWS(ch)
			|| (prev != TokenType.WHITESPACE
			    && prev != TokenType.LPAREN)) {
			throw new Error(lexMsg(pos,"Arithmetic operators Must be surrounded by whitespace:"+text));
		    }
		}
	    } else
	        throw new Error(lexMsg(pos,"Illegal character sequence: "+text));
	    token = new Token(ttype);
	    token.text = text.toString();
	    token.pos.set(pos);
	    // if whitespace, try for another token
	    if(ttype == TokenType.WHITESPACE) continue;
	    break;
	}
	return token;
	
	} catch (Exception e) {
	    throw new Error(lexMsg(pos),e);
	}
    }

    //////////////////////////////////////////////////

    String lexMsg(Pos pos)
	{return (pos.toString());}

    String lexMsg(Pos pos,String msg)
	{return (pos.toString()+"; "+msg);}



    //////////////////////////////////////////////////

    public void skipWS(StringBufferReader rdr) throws Exception
    {
	int ch;
	while(isWS(ch=rdr.read()));
	if(ch != EOFCHAR) rdr.pushback();	
    }

    // The snobol3 primer claims that
    // a label, after the first character,
    // can contain any "admissible character".
    // This is interpreted to mean any legal NAME character.
    // i.e.: [a-zA-Z0-9][a-zA-Z0-9.]*

    static public boolean isLabelChar1(int ch)
	{return isNameChar1(ch) && (ch != '.');}

    static public boolean isLabelChar2(int ch) {return isNameChar2(ch);}

    static public boolean isNameChar1(int ch)
    {
	return (
	       (ch >= 'a' && ch <= 'z')
	    || (ch >= 'A' && ch <= 'Z')
	    || (ch >= '0' && ch <= '9')
	    || (ch == '.')
	    );
    }

    // ID1 characters can appear after the initial id char
    static public boolean isNameChar2(int ch) {return isNameChar1(ch);}

    static public boolean isDelimiter(int ch)
    {
	return TokenType.DELIMITERS.indexOf(ch) >= 0;
    }

    static public boolean isDigit(int ch)
    {
	return (
	   (ch >= '0' && ch <= '9')
	);
    }

    // Provide context sensitive test to distinguish names and integers
    static public boolean isInteger(StringBuilder text)
    {
	int ch = text.charAt(0);
	if(!isDigit(ch) && (ch != '+')) return false;
	for(int i=1;i<text.length();i++)
	    if(!isDigit(text.charAt(i))) return false;
	return true;
    }

/*
    static public boolean isName(String text)
    {
	if(!isNameChar(text.charAt(0))) return false;
	for(int i=1;i<text.length();i++)
	    if(!isNameChar1(text.charAt(i))) return false;
	return true;
    }
*/

    static public boolean isFunction(String text)
    {
	return (Snobol3.functions.get(text) != null);
    }

    static public boolean isPatternFunction(String text)
    {
	return (Snobol3.patterns.get(text) != null);
    }

    static public boolean isLabel(String text)
    {
	// It is a label iff it is in the label map
	return (Snobol3.labels.get(text) != null);
    }

    static public boolean isWS(int ch)
    {
	boolean b = (
	   (ch >= '\u0000' && ch <= '\u0020')
	   && ch != EOLCHAR
	   && ch != EOFCHAR
	);
	return b;
    }

    static boolean isKeyWord(String txt)
	{return keywordFor(txt) != TokenType.NULL;}

    static TokenType keywordFor(String txt)
    {
	if(txt.equalsIgnoreCase("s")) return TokenType.SUCCESS;
	if(txt.equalsIgnoreCase("f")) return TokenType.FAIL;
	if(txt.equalsIgnoreCase("end")) return TokenType.END;
	if(txt.equalsIgnoreCase("return")) return TokenType.RETURN;
	if(txt.equalsIgnoreCase("freturn")) return TokenType.FRETURN;
//	if(txt.equalsIgnoreCase("define")) return TokenType.DEFINE;
	return TokenType.NULL;
    }

}
