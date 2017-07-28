package jsnobol3;

import java.util.ArrayList;
import tools.*;

class DebugPoint extends AbstractDebugPoint
{
    static final int NONE = BASE;
    static final int TOKENS = BASE+1;
    static final int PARSE = BASE+2;
    static final int PASS3 = BASE+3;
    static final int EXEC = BASE+4;
    static final int PATTERN = BASE+5;
    static void initializeClass()
    {
	AbstractDebugPoint.names = new String[]{
		"none","tokens","parse","pass3","exec","pattern"
	};
    }
    static public int pointFor(String p)
    {
	int ip = AbstractDebugPoint.pointFor(p);
	return (ip == BASE?NONE:ip);
    }
}
public class Debug extends AbstractDebug
{
    //////////////////////////////////////////////////

    static String quotechar(char c) {return "'"+c+"'";}

    static char inchar = '>';
    static char outchar = '<';

    static public String indentin(int n) {return Util.indent(n,inchar);}
    static public String indentout(int n) {return Util.indent(n,outchar);}

    static public Object format(Object o)
    {
	if(o instanceof String) {
	    o = Constants.SQUOTE + ((String)o) + Constants.SQUOTE;
	}
	return o;
    }
    //////////////////////////////////////////////////

    static public void initializeClass()
    {
	// order is important here
	DebugPoint.initializeClass();
	AbstractDebug.initializeClass();
	AbstractDebug.active = new Debug(true);
	AbstractDebug.inactive = new Debug(false);
    }

    static public Debug when(Object... tests)
	{return (Debug)AbstractDebug.choose(andTest(tests));}
    static public Debug whenAny(Object... tests)
	{return (Debug)AbstractDebug.choose(orTest(tests));}
    static public Debug whenany(Object... tests)
	{return whenAny(tests);}
    static public boolean isWhen(Object... tests)
	{return andTest(tests);}

    //////////////////////////////////////////////////

    public Debug(boolean active) {super(); this.isactive = active;}

} // Debug

class ParseStack
{
    ArrayList<String> stack;
    public ParseStack() {stack = new ArrayList<String>();}
    public void push(String proc) {stack.add(proc);}
    public String pop()
    {
	if(stack.size() == 0) return null;
	String s = stack.remove(stack.size()-1);
	return s;
    }
    public String top() {return this.get(0);}

    // this returns from the top down
    public String get(int i)
    {
	if(stack == null || stack.size() == 0) return "empty";
	return stack.get(stack.size()-(i+1));
    }
    public int depth() {return stack.size();}
    public String toString()
    {
	String s = "["; 
	for(int i=0;i<depth();i++) {
	    if(i > 0) s += " ";
	    s += get(i);
	}
	s += "]";
	return s;
    }

}
