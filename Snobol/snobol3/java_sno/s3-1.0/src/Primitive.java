package jsnobol3;

import java.util.StringTokenizer;
import java.util.regex.PatternSyntaxException;
	
abstract class Primitive
{
    static void initializeClass() {Primitive.definePrimitives();}

    static void definePrimitives() 
    {
	// define some primitive functions
	Primitive p;
	fcnDef("EQUALS",(p=new $Equals()));
	    fcnDef("equals",p);
	    fcnDef("equal",p);
	fcnDef("UNEQL",(p=new $Uneql()));
	    fcnDef("uneql",p);
	    fcnDef("unequal",p);
	fcnDef("SIZE",(p=new $Size()));
	    fcnDef("size",p);
	fcnDef("ANCHOR",(p=new $Anchor()));
	    fcnDef("anchor",p);
	fcnDef("UNANCH",(p=new $Unanch()));
	    fcnDef("unanch",p);
	    fcnDef("unanchor",(p=new $Unanch()));
	// Arithmetics
	fcnDef(".EQ",(p=new $Eq()));
	    fcnDef(".eq",p);
	fcnDef(".NE",(p=new $Ne()));
	    fcnDef(".ne",p);
	fcnDef(".LT",(p=new $Lt()));
	    fcnDef(".lt",p);
	fcnDef(".LE",(p=new $Le()));
	    fcnDef(".le",p);
	fcnDef(".GT",(p=new $Gt()));
	    fcnDef(".gt",p);
	fcnDef(".GE",(p=new $Ge()));
	    fcnDef(".ge",p);
	fcnDef(".NUM",(p=new $Num()));
	    fcnDef(".num",p);
	fcnDef(".REMDR",(p=new $Remdr()));
	    fcnDef(".remdr",p);
	// following are upper case only
	fcnDef("READ",(p=new $Read()));
	fcnDef("PRINT",(p=new $Print()));
	fcnDef("MODE",(p=new $Mode()));
	fcnDef("TRACE",(p=new $Trace()));
	fcnDef("STRACE",(p=new $Strace()));
	// special cases
	FunctionCompiler fc = new FunctionCompiler();
	fcnDef("CALL",(p=new Call(fc)));
	fcnDef("DEFINE",(p=new Define(fc)));
	    fcnDef("define",p);
	// example from the ref manual
	fcnDef("rematch",(p=new $REmatch()));
    }

    static void fcnDef(String name,Primitive p)
    {
	Function f = new PrimFunction(name,p);
	Snobol3.functions.put(name,f);
    }

    //////////////////////////////////////////////////
    public Primitive() {}
    public int nargs() {return 2;} // default
    public ArgType typeFor(int argi) {return ArgType.CVS;} // default
    public void setReturn(VM vm) {setReturn(vm,"");}
    public void setReturn(VM vm, Object r) {vm.returnvalue=r;}
    public void setReturn(VM vm, int r) {vm.returnvalue=r;}
    public void setReturn(VM vm, boolean r) {vm.returnvalue=r;}
    public boolean isSpecial() {return false;}

    abstract void execute(VM vm, PrimFunction fcn) throws Failure;

}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
class $Equals extends Primitive
{
    public $Equals() {super();}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String s1 = (String)vm.pop();	
	String s2 = (String)vm.pop();	
	vm.cc = !(s1.equals(s2));
	setReturn(vm);
    }
}
class $Uneql extends Primitive
{
    public $Uneql() {super();}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String s1 = (String)vm.pop();	
	String s2 = (String)vm.pop();	
	vm.cc = !(s1.equals(s2));
	setReturn(vm);
    }
}
class $Size extends Primitive
{
    public $Size() {super();}
    public int nargs() {return 1;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String s1 = (String)vm.pop();
	int len = Integer.valueOf(s1==null?0:s1.length());
	vm.cc = false;
	setReturn(vm,len);
    }
}
class $Anchor extends Primitive
{
    public $Anchor() {super();}
    public int nargs() {return 0;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	vm.match.anchored = true;
	vm.cc = false;
	setReturn(vm);
    }
}
class $Unanch extends Primitive
{
    public $Unanch() {super();}
    public int nargs() {return 0;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	vm.match.anchored = false;
	vm.cc = false;
	setReturn(vm);
    }
}
class $Mode extends Primitive
{
    public $Mode() {super();}
    public int nargs() {return 1;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String modes = (String)vm.pop();
	try {Snobol3.modes.setModeList(modes);}
	catch (Error err) {throw new Failure(vm,err.toString());}
System.err.println("modes="+modes);
	vm.cc = false;
	setReturn(vm);
    }
}
class $Strace extends Primitive
{
    public $Strace() {super();}
    public int nargs() {return 1;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String vnames = (String)vm.pop();
	if(vnames != null && vnames.length() > 0) {
	    StringTokenizer tok = new StringTokenizer(vnames,",");
	    while(tok.hasMoreTokens()) {
	        String vname = tok.nextToken();
	        Var var = vm.lookup(vname);
		if(var == null) { // create it in global space
		    var = vm.createVar(vname,Scope.GLOBAL);
		}
		var.trace = true;
	    }
	}
	vm.cc = false;
	setReturn(vm);
    }
}

class $Trace extends Primitive
{
    public $Trace() {super();}
    public int nargs() {return 1;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String fnames = (String)vm.pop();
	if(fnames != null && fnames.length() > 0) {
	    StringTokenizer tok = new StringTokenizer(fnames,",");
	    while(tok.hasMoreTokens()) {
	        String fname = tok.nextToken();
	        Function f = Snobol3.functions.get(fname);
		if(f == null)
		    throw new Failure(vm,"TRACE: undefined function: "+fname);
		f.trace = true;
	    }
	}
	vm.cc = false;
	setReturn(vm);
    }
}

class $Read extends Primitive
{
    public $Read() {super();}
    public int nargs() {return 1;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String name = (String)vm.pop();
	Var var = vm.lookup(name);
	if(var == null) { // create it in global space
	    var = vm.createVar(name,Scope.GLOBAL);
	}
	var.stream = Constants.STDIN;
	vm.cc = false;
	setReturn(vm);
    }
}
class $Print extends Primitive
{
    public $Print() {super();}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String flags = (String)vm.pop();
	String name = (String)vm.pop();
	Var var = vm.lookup(name);
	if(var == null) { // create it in global space
	    var = vm.createVar(name,Scope.GLOBAL);
	}
	var.stream = Constants.STDOUT;// default
	if(flags != null && flags.length() > 0) {
	    StringTokenizer tok = new StringTokenizer(flags,",");
	    while(tok.hasMoreTokens()) {
	        String flag = tok.nextToken();
Debug.Println("flag="+flag);
		if(flag.equals("nonewline"))
		    var.nonewline = true;
		else if(flag.equals("stderr"))
		    var.stream = Constants.STDERR;
		else
		    throw new Failure(vm,"PRINT(): unknown flag: "+flag);
	    }
	}
	vm.cc = false;
	setReturn(vm);
    }
}
class $Eq extends Primitive
{
    public $Eq() {super();}
    public ArgType typeFor(int argi) {return ArgType.CVI;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	int i1 = ((Integer)vm.pop()).intValue();
	int i2 = ((Integer)vm.pop()).intValue();
	vm.cc = !(i1 == i2);
	setReturn(vm);
    }
}
class $Ne extends Primitive
{
    public $Ne() {super();}
    public ArgType typeFor(int argi) {return ArgType.CVI;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	int i1 = ((Integer)vm.pop()).intValue();
	int i2 = ((Integer)vm.pop()).intValue();
	vm.cc = !(i1 != i2);
	setReturn(vm);
    }
}
class $Lt extends Primitive
{
    public $Lt() {super();}
    public ArgType typeFor(int argi) {return ArgType.CVI;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	int i2 = ((Integer)vm.pop()).intValue();
	int i1 = ((Integer)vm.pop()).intValue();
	vm.cc = !(i1 < i2);
	setReturn(vm);
    }
}
class $Le extends Primitive
{
    public $Le() {super();}
    public ArgType typeFor(int argi) {return ArgType.CVI;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	int i2 = ((Integer)vm.pop()).intValue();
	int i1 = ((Integer)vm.pop()).intValue();
	vm.cc = !(i1 <= i2);
	setReturn(vm);
    }
}
class $Gt extends Primitive
{
    public $Gt() {super();}
    public ArgType typeFor(int argi) {return ArgType.CVI;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	int i2 = ((Integer)vm.pop()).intValue();
	int i1 = ((Integer)vm.pop()).intValue();
	vm.cc = !(i1 > i2);
	setReturn(vm);
    }
}
class $Ge extends Primitive
{
    public $Ge() {super();}
    public ArgType typeFor(int argi) {return ArgType.CVI;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	int i2 = ((Integer)vm.pop()).intValue();
	int i1 = ((Integer)vm.pop()).intValue();
	vm.cc = !(i1 >= i2);
	setReturn(vm);
    }
}
class $Num extends Primitive
{
    public $Num() {super();}
    public int nargs() {return 1;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String i1 = (String)vm.pop();
	try {Integer.decode(i1); vm.cc = false;}
	catch (NumberFormatException nfe) {vm.cc = true;}
	setReturn(vm);
    }
}
class $Remdr extends Primitive
{
    public $Remdr() {super();}
    public ArgType typeFor(int argi) {return ArgType.CVI;}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	int i2 = ((Integer)vm.pop()).intValue();
	int i1 = ((Integer)vm.pop()).intValue();
	int r = Integer.valueOf(i1 % i2);
	vm.cc = false;
	setReturn(vm,r);
    }
}

class $Define extends Primitive
{
    public $Define() {super();}
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	// It is illegal to try to call this function
	// dynamically.
	throw new Failure(vm,"DEFINE(): cannot be called dynamically");
    }
}

// Example from the reference manual
class $REmatch // The primitive class names begin with $ as a convention.
      extends Primitive
{
    public $REmatch() {super();} // This function <= 2 arguments
    public ArgType typeFor(int argi)
	{return ArgType.CVS;} // All arguments are required to be a string
			      // The compiler will insert appropriate CVS
			      // operators to ensure this.
    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	String re = (String)vm.pop(); // pop regular expression off the stack.
	String s = (String)vm.pop(); // pop target string off the stack.
	if(re.length() == 0)
	    throw new Failure(vm,"REMatch: empty regular expression");
	// See if the string can be matched by the RE
	boolean match = false;
	try {
	    match = s.matches(re);
	} catch (PatternSyntaxException pse) {
	    throw new Failure(vm,"rematch: illegal regular expression: "+re);
	}
	vm.cc = !match; // if the match failed, then set the condition code.
	setReturn(vm); // return empty string on success
    }
}
