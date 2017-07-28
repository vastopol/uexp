package jsnobol3;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.io.PrintWriter;
import java.io.PrintStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;


import tools.*;


/*
The stack frame looks like the following:

N+3: Function name     <- FP (Frame Pointer)
N+2: Previous Context
N+1: Previous FP
N:   Return Address
*/

class VM
{
    Operator[] code;
    Program program;

    // Following are global state
    boolean cc; // condition code: success=false;fail=true
    int endprog; // address of the end of the program
    boolean stop;
    int pc; // program counter
    Object returnvalue;
    EvalStack eval; // eval and frame stack
    HashMap<String,Var> globals;
    ArrayList <String> argv;

    PrintWriter stdout;
    PrintWriter stderr;
    BufferedReader stdin;

    Frame frame; // function frame
    HashMap<String,Var> locals;
    MatchState match; // pattern matching data

    public VM()
    {
	eval = new EvalStack(this);
	stop = false;
	globals = new HashMap<String,Var>();
	cc = false;
	argv = null;
	stdout = new PrintWriter(System.out);
	stderr = new PrintWriter(System.err);
	stdin = new BufferedReader(new InputStreamReader(System.in));
	frame = new Frame();
	locals = null;
	match = new MatchState(this);
	returnvalue = null;
	predefined(); // predefine some variables
    }

    void predefined()
    {
	Var v = new Var("SYSPOT");
	v.stream = Constants.STDOUT;
	globals.put("SYSPOT",v);
	v = new Var("SYSPPT");
	v.stream = Constants.STDIN;
	globals.put("SYSPPT",v);
	// predefine some alternative IO names
	v = new Var("stdout");
	v.stream = Constants.STDOUT;
	globals.put("stdout",v);
	v = new Var("stdin");
	v.stream = Constants.STDIN;
	globals.put("stdin",v);
	v = new Var("stderr");
	v.stream = Constants.STDERR;
	globals.put("stderr",v);
	v = new Var("squote");
	v.value = "'";
	globals.put("QUOTE",v);
	    globals.put("squote",v);
	v = new Var("dquote");
	v.value = "\"";
	globals.put("dquote",v);
    }

    void execute(Program prog, ArrayList<String> argv)
	throws Failure
    {

	this.program = prog;
	this.code = program.getCode();
	this.argv = argv;
	endprog = prog.size() - 1;

	Debug.when(DebugPoint.EXEC).println("labels:\n"+dumpMap(Snobol3.labels));
	Debug.when(DebugPoint.EXEC).println("functions:\n"+dumpMap(Snobol3.functions));
	Debug.when(DebugPoint.EXEC).println("patterns:\n"+dumpMap(Snobol3.patterns));

	pc = 0;
	try {
	    while(!stop) {
		Operator op = code[pc];
		traceBefore(op);
		pc++; // default increment
		op.execute(this);
		traceAfter();
		if(cc && !frame.infailure) {
		    // initiate failure processing
		    pc = frame.endstmt;
		}
	    }
	} catch (Failure ferr) {
	    Snobol3.modes.dump = Snobol3.modes.dumper;
ferr.printStackTrace();
	}
	if(Snobol3.modes.dump) {
	    System.out.println("Final State:");
	    System.out.println("stack="+dumpStack());
	    if(locals != null) System.out.println("locals:\n"+dumpMap(locals));
	    System.out.println("globals:\n"+dumpMap(globals));
	}
    }

    boolean localchange = false;
    boolean globalchange = false;
    boolean stackchange = false;

    void traceBefore(Operator op)
    {
	if(!Debug.isWhen(DebugPoint.EXEC)) return;
	Debug.Println("%2d: %s %s",pc,op,(cc?"[F]":""));
    }

    void traceAfter()
    {
	if(!Debug.isWhen(DebugPoint.EXEC)) return;
	String retval = (returnvalue==null?"null"
			    : Debug.format(returnvalue).toString());
	Debug.Println("    %s (%s) %s",
		(cc?"[F]":("{"+retval+"}")),
		(stop?"stop":""),
		frame.trace()
		);
	if(stackchange) Debug.Println("    stack="+dumpStack());
	if(localchange && locals.size() > 0)
	    Debug.Println("locals:\n"+dumpMap(locals));
	if(globalchange) Debug.Println("globals:\n"+dumpMap(globals));
	localchange = false;
	globalchange = false;
	stackchange = false;
//	Debug.Println(".....");
    }

    // passthru
    int depth() {return eval.depth();}    
    int mark() {return eval.mark();}
    void unmark(int i) {eval.unmark(i);}
    Object pop() throws Failure {stackchange=true; return eval.pop();}
    Object top() throws Failure {return eval.top();}
    Object nth(int n) throws Failure {return eval.nth(n);}
    void push(Object o) {stackchange=true; eval.push(o);}

    // utilities

    // find the current var matching a given name and using
    // current scopes; create if required.
    // This mimics a two level name space like S3 or C.

    Var lookup(String name)
    {
	// Rule: take from local if defined
	//       otherwise take from global
	Var var;
	if(locals != null && (var = locals.get(name)) != null) return var;
	var = globals.get(name);
	return var;
    }

    Var createVar(String name, Scope scope)
    {
	Var var = null;
	switch (scope) {
	case SCOPED:
	    var = new Var(name);
	    if(locals != null) {
		var.isLocal = true;
		locals.put(name,var);
	    } else {		
		var.isLocal = false;
		globals.put(name,var);
	    }	
	case LOCAL:
	    if(locals != null) {
	        var = new Var(name);
		var.isLocal = true;
		locals.put(name,var);
	    } 
	    break;
	case GLOBAL:
	    var = new Var(name);
	    var.isLocal = false;
	    globals.put(name,var);
	}
	return var;
    }

    Var assign(String name, Object value)
    {
	Var var = lookup(name);	
	if(var == null) var = createVar(name,Scope.SCOPED);
	var.value = value;
	if(var.trace) traceVar(var);
	if(var.stream != Constants.STDNULL) outputTest(var);
	if(var.isLocal) localchange = true; else globalchange = true;
	return var;
    }

    Var assignLocal(String name, Object value) throws Failure
    {
	Var var = lookup(name);	
	if(var == null) var = createVar(name,Scope.LOCAL);
	if(var == null) return null;
	var.value = value;
	if(var.trace) traceVar(var);
	if(var.stream != Constants.STDNULL) outputTest(var);
	if(var.isLocal) localchange = true; else globalchange = true;
	return var;
    }

    void outputTest(Var var) throws Failure
    {
	PrintWriter stream = stdout;
	switch (var.stream) {	
	case Constants.STDERR: stream = stderr; // fall thru
	case Constants.STDOUT:
	    // dump the var's current value as a line of text
	    Object value = (var.value == null?"":var.value);
	    if(var.nonewline)
		stream.print(value.toString());
	    else
		stream.println(value.toString());
	    stream.flush();
	    break;
	case Constants.STDIN:
	    throw new Failure(this,"Assignment to input variable:"+var.name);
	default: break; // ignore
	}
    }

    // deref a variable, taking IO into account
    Object inputTest(Var var) throws Failure
    {
	Object value = var.value;
	switch (var.stream) {	
	case Constants.STDIN:
	    try {value = stdin.readLine();} catch (IOException ioe) {
		throw new Failure(this,ioe);
	    }
	    break;
	case Constants.STDERR:
	case Constants.STDOUT:
	    throw new Failure(this,"Read from output variable:"+var.name);
	default: break;
	}
	var.value = value;
	return value;
    }

    void traceVar(Var var) throws Failure
    {
	// dump the var's current value as a line of text
	Object value = (var.value == null?"":var.value);
	stdout.println("ASSIGN: "+var.name+"="+value.toString());
	stdout.flush();
    }

    void dump(PrintStream out)
    {
	out.printf("VM: line=%d pc=%d fp=%d rp=%d endStmt=%d cc=%b stop=%b\n");
	out.println("stack:");
	printStackVerbose(out);
	out.println("locals:\n"+dumpMap(locals));
	out.println("globals:\n"+dumpMap(globals));
    }


    void printStackVerbose(PrintStream out)
    {
	int maxwidth=16;
	String extra = Util.indent(maxwidth);
	int mark = eval.getmark();
	int depth = eval.depth();
	out.printf("[%2d]:  %s  <- sp\n",depth,extra);
	for(int i=depth-1;i>=0;i--) {
	    Object o = eval.ith(i);	
	    String s = o.toString() + extra;
	    out.printf("[%2d]: |%s|",i,s.substring(0,maxwidth));
	    if(i == frame.fp) out.print("<- fp");
	    if(i == mark) out.print("<- mark");
	    out.println();	
	}
    }

    String dumpStack()
    {
	int mark = eval.getmark();
	int depth = eval.depth();
	String s = "["+depth+"/"+mark+"]:";
	for(int i=depth-1;i>=0;i--) {
	    Object o = eval.ith(i);	
	    o = (o == null ? "null"
		           : Debug.format(o).toString());
	    s += (' '+o.toString());
	    if(i > 0) {
		char sep = ',';
		if(i == frame.fp) sep = '^';
		if(i == mark) sep = '|';
		s += sep;
	    }
	}
	return s;
    }

    // used by dump/dumper modes and by trace
    String dumpMap(HashMap map)
    {
	String s = "";
	for(Iterator it=map.keySet().iterator();it.hasNext();) {
	    String key = (String)it.next();
	    s += ("\t" + key + "='"+map.get(key)+"'"+Constants.EOLCHAR);
	}
	return s;
    }

}
