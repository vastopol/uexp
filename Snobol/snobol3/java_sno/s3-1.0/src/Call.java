package jsnobol3;

// Implement the call() function

import java.util.ArrayList;
import tools.Ref;

class Call extends Primitive
{
    FunctionCompiler fc;
 
    public Call(FunctionCompiler d) {super(); this.fc = d;}
    public boolean isSpecial() {return true;}
    public int nargs() {return 1;}

    public void execute(VM vm, PrimFunction fcn) throws Failure
    {
	// Construct a statement for the function call
	String callstring = (String)vm.pop();
	//nolabel,no branch,eol
	callstring = " "+callstring;
	AST calltree = null;
	try {
	    Ref<AST> tree = new Ref<AST>();
	    fc.open(callstring,vm.frame.line);
	    fc.parser.fcncall(tree);
	    calltree = tree.deref();
	    Debug.when(DebugPoint.EXEC).println("CALL: pass2:");
	    Debug.when(DebugPoint.EXEC).println(calltree.prettyPrint());
	} catch(Error p2err) {
	    throw new Failure(vm,"CALL: "+p2err);
	}
	boolean isprim = false;
	Program prog = null;
	int base = Constants.NOADDRESS;
	try {
	    prog = vm.program;
	    base = prog.size();
	    // construct the code
	    fc.pass3.translateExpr(calltree,prog);
	    // do any necessary bindings
	    fc.pass3.bind(prog);
	    // now we need to modify the generated code
	    // get the last op generated.
	    int last = prog.size()-1;
	    Operator op = prog.get(last);
	    isprim = (op instanceof PrimitiveOp);	
	    if(isprim) {
		// make the primitive return to the right place
		prog.add(new CallRetOp());
	    } else if(op instanceof JSROp) {
		// defined code call; make it return to the right place
		JSROp jsrop = (JSROp)op;
		int dest = jsrop.dest;
		JumpOp jop  = new JumpOp();
		jop.dest = dest;
		prog.set(last,jop);
	    } else
		throw new Failure(vm,"CALL: unexpected code generated: notify developer");
	    if(Debug.isWhen(DebugPoint.EXEC))
		Pass3.dumpCode(prog,"CALL: |code|="+prog.size());
	} catch(Error p3err) {
	    throw new Failure(vm,"CALL: "+p3err);
	}
	// now execute the code
	// set the rp to return to the right place
	vm.frame.rp = vm.pc; // return to after the CALL
	vm.pc = base;
    }

}