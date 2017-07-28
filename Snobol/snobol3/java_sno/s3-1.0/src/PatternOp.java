package jsnobol3;

import static jsnobol3.Constants.*;

abstract class PatternOp extends Operator
{
    static void initializeClass() {PatternOp.definePatterns();}

    static void definePatterns()
    {
	// define some primitive pattern function
	// (excluding ARB, LEN, BAL)
	Snobol3.patterns.put("bracket",new BracketOp());
    }

    //////////////////////////////////////////////////
    int faildest = 0;
    VRef opvar = null; // not always used
    MatchState state = null;
    VM vm = null;

    public PatternOp() {super();}
    public String toString()
    {
	String s = "";
	if(opvar != null) {
	    s = String.format("*%s|%s(%d)*",super.toString(),
			      opvar.name,faildest);
	} else {
	    s = String.format("%s(%d)",super.toString(),faildest);
	}
	return s;
    }
    public int nargs() {return 0;} // default
    public ArgType typeFor(int argi) {return ArgType.CVS;} // default
    public PatternOp clone()
    {
	PatternOp pop = null;
	try {
	    pop = (PatternOp)this.getClass().newInstance();
	} catch (Exception e) {pop=null;}
	return pop;
    }	

    public void execute(VM vm) throws Failure
    {
	this.vm = vm;
	this.state = vm.match;
	if(state.failed) retry(); else initial();
    }
    // Utilities for subclasses
    void fail(int cursor)
    {
	state.failed = true;
	state.cursor = cursor;
	vm.pc = faildest;
	Debug.whenany(DebugPoint.PATTERN,DebugPoint.EXEC).println(state.toString());
    }

    void succeed(Range r)
    {
	state.failed = false;
	state.cursor = r.rn;
	vm.push(r);
	Debug.whenany(DebugPoint.PATTERN,DebugPoint.EXEC).println(state.toString());
    }

    void assign(Range r)
    {
	if(opvar != null) {
	    String rangestring = state.subject.substring(r.r0,r.rn);
	    vm.assign(opvar.name,rangestring);
	}
    }

    abstract void initial() throws Failure;
    abstract void retry() throws Failure;
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

class ArbOp extends PatternOp
{
    public ArbOp() {super();}

    public void initial() throws Failure
    {
	// get name of our variable from stack and keep until failure
	opvar = (VRef)vm.pop();
	// get current match point
	int r0 = state.cursor;
	int rn = r0; // initially match zero characters
	Range r = new Range(r0,rn);
	assign(r);
	succeed(r);
    }

    public void retry() throws Failure
    {
	// get previous range from the stack
	Range r = (Range)vm.pop();
	// see if there is room to extend the match
	if(r.rn < state.subject.length()) {
	    r.rn++;
	    assign(r);
	    succeed(r);
	} else { // report failure
	    // remove range
	    vm.pop();
	    fail(r.r0);
	}
    }
}

class BalanceOp extends PatternOp
{
    public BalanceOp() {super();}

    public void initial() throws Failure
    {
	// get name of our variable from stack and keep until failure
	opvar = (VRef)vm.pop();
	Range r = new Range(state.cursor,state.cursor);
	if(!balance(r)) fail(r.r0); else {assign(r); succeed(r);}
    }

    public void retry() throws Failure
    {
	Range r = (Range)vm.pop();
	if(!balance(r)) fail(r.r0); else {assign(r); succeed(r);}
    }

    boolean balance(Range r)
    {
	String subject = state.subject;
	int len = subject.length();
	if(r.rn >= len) return false;
	char ch = subject.charAt(r.rn);
	if(ch == RPARENCHAR) return false;
	if(ch != LPARENCHAR) {
	    r.rn++;
	    return true;
	}
	// Only case left is that we are at an LPAREN, need to scan
	// forward to find a matching RPAREN, taking nesting into account
	int rn = r.rn+1;
	int depth = 1;
	while(rn < len && depth > 0) {
	    ch = subject.charAt(rn++);
	    switch (ch) {
	    case LPARENCHAR: depth++;
	    case RPARENCHAR: depth--; break;
	    default: break;
	    }	    
	}
	if(depth == 0) {
	    r.rn = rn;
	    return true;
	}
	return false;
    }
}

class BeginPatternOp extends Operator
{
    public BeginPatternOp() {super();}
    public void execute(VM vm) throws Failure
    {
	MatchState state = vm.match;
	// check for failure entry
	if(state.failed) {
	    // try to move the anchor point
	    if(state.anchored || state.anchor >= state.subject.length()) {
		// total pattern failure
	        vm.cc = true;
		// clear stack
		while(vm.depth() > state.pp) vm.pop();
		Debug.when(DebugPoint.EXEC).println(state.toString());
	    } else { // move forward with new starting point
		state.anchor++;
		state.cursor = state.anchor;
		state.failed = false;
		Debug.when(DebugPoint.EXEC).println(state.toString());
	    }
	} else {
	    // initial entry
	    // stack: <vref|null> <value>	
	    VRef subjvar = (VRef)vm.pop();
	    Object subj = vm.pop();
	    state.pp = vm.depth();
	    state.subject = subj.toString();
	    state.subjectvar = subjvar;
	    state.cursor = (state.anchor = 0);
	    state.failed = false;
	    Debug.when(DebugPoint.EXEC).println(state.toString());
	}
    }
}

class LenOp extends PatternOp
{
    int len = 0;
    public LenOp() {super();}
    public int nargs() {return 1;}

    public void initial() throws Failure
    {
	// get length from the stack and keep until fail
	len = ((Integer)vm.pop()).intValue();
	// get name of our variable from stack and keep until failure
	opvar = (VRef)vm.pop();
	// get current match point
	int r0 = state.cursor;
	int rn = r0 + len;
	// see is we can match n characters
	if(rn > state.subject.length()) {
	    fail(r0);
	} else {
	    Range r = new Range(r0,rn);
	    assign(r);
	    succeed(r);
	}		
    }

    public void retry() throws Failure
    {
	// we are not retryable; so we fail too
	Range r = (Range)vm.pop();
	fail(r.r0);
    }
}

class StringPatternOp extends PatternOp
{
    public StringPatternOp() {super();}

    void initial() throws Failure
    {
	// get string to match from stack
	String str = (String)vm.pop();
	// see is we can match n characters
	int r0 = state.cursor;
	int rn = r0 + str.length();
	if(rn > state.subject.length()
	   || !state.subject.substring(r0,rn).equals(str)) {
	    fail(r0);
	} else {
	    Range r = new Range(r0,rn);
	    succeed(r);
	}		
    }

    void retry() throws Failure
    {
	// we are not retryable; so we fail too
	Range r = (Range)vm.pop();
 	fail(r.r0);
    }
}

class EndPatternOp extends Operator
{
    public EndPatternOp() {super();}
    public void execute(VM vm) throws Failure
    {
	MatchState state = vm.match;
	Debug.when(DebugPoint.EXEC).println(state.toString());
	// if we get here, then we have success, so
	// compute the range that was matched and leave on stack
	// (after clearing the other rubbish)
	Range result = new Range(state.anchor,state.cursor);
	while(vm.depth() > state.pp) vm.pop();
	vm.push(result);
    }
}

class BracketOp extends PatternOp
{
    char lbracket = 0;
    char rbracket = 0;

    public BracketOp() {super();}
    public int nargs() {return 1;}

    public void initial() throws Failure
    {
	// get the bracket pair
	String pair = (String)vm.pop();
	// get name of our variable from stack and keep until failure
	opvar = (VRef)vm.pop();
	if(pair.length() != 2)
	    throw new Failure(vm,"Bracket: illegal bracket pair: "+pair);
	lbracket = pair.charAt(0);
	rbracket = pair.charAt(1);
	Range r = new Range(state.cursor,state.cursor);
	if(!bracket(r)) fail(r.r0); else {assign(r); succeed(r);}
    }

    public void retry() throws Failure
    {
	Range r = (Range)vm.pop();
	if(!bracket(r)) fail(r.r0); else {assign(r); succeed(r);}
    }

    boolean bracket(Range r)
    {
	String subject = state.subject;
	int len = subject.length();
	if(r.rn >= len) return false;
	char ch = subject.charAt(r.rn);
	if(ch == rbracket) return false;
	if(ch != lbracket) {
	    r.rn++;
	    return true;
	}
	// Only case left is that we are at an lbracket, need to scan
	// forward to find a matching rbracket, taking nesting into account
	int rn = r.rn+1;
	int depth = 1;
	while(rn < len && depth > 0) {
	    ch = subject.charAt(rn++);
	    if(ch == lbracket)
		depth++;
	    else if(ch == rbracket)
		depth--;
	    // else do nothing
	}
	if(depth == 0) {
	    r.rn = rn;
	    return true;
	}
	return false;
    }
}

class Range
{
    int r0;
    int rn;
    public Range(int r0, int rn) {this.r0=r0; this.rn=rn;}
    public String toString() {return ("/"+r0+","+rn+"/");}
}

class MatchState
{
    VM vm;
    boolean anchored;
    int cursor; // current location during a match
    int anchor; // current beginning of the whole match;
		// must be 0 if anchored is true
    int pp; // pointer into stack before match begins
    boolean failed;
    VRef subjectvar;
    String subject;

    public MatchState(VM vm) {this.vm = vm;}
    public void reset()
    {
	subjectvar = null;
	subject = null;
	anchored = Snobol3.modes.anchored;
	cursor = 0;
	anchor = 0;
	pp = 0;
	failed = false;
    }
    public String toString()
    {
	String s = "MatchState[";
	s += subject.substring(0,anchor);
	s += ".";
	s += subject.substring(anchor,cursor);
	s += "|";
	s += subject.substring(cursor,subject.length());
//s += (subject+"("+anchor+","+cursor+")");
	if(subjectvar != null) s += ("@"+subjectvar);
	if(failed) s += "*";	
	s += "]";
	return s;
    }

}
