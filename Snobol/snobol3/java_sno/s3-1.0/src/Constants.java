package jsnobol3;

enum ArgType {CVA,CVI,CVS,CVV;}
enum Scope {SCOPED,LOCAL,GLOBAL;}

abstract class Constants
{
    static final char COMMENTCHAR='*'; 
    static final char CONTINUECHAR='.'; 
    static final char CONTROLWORDCHAR='-'; 
    static final char COMMACHAR=','; 
    static final char BLANKCHAR=' '; 
    static final String WHITESPACECHARS =" \t"; 
    static final String NULLSTRING =""; 
    static final char SQUOTE = '\'';
    static final char DQUOTE = '"';
    static final char EOLCHAR = '\n';
    static final int EOFCHAR = -1;
    static final char  LPARENCHAR = '(';
    static final char  RPARENCHAR = ')';
    static final int NOADDRESS = -1;

    static final int DEFAULTSTACKSIZE=128;
    static final int INITIALCODESIZE=1024;

    static final Integer ZERO = new Integer(0);

    // predefined IO streams
    static final int STDNULL = -1;
    static final int STDIN = 0;
    static final int STDOUT = 1;
    static final int STDERR = 2;
}
