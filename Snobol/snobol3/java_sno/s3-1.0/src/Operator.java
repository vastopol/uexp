package jsnobol3;

import java.io.IOException;
import java.util.HashMap;

import tools.*;
import static jsnobol3.Constants.*;

abstract class Operator
{
    public Operator() {}
    public void execute(VM vm) throws Failure {}
    public String toString()
    {
	String scn = Util.simpleClassName(this);
	if(scn.endsWith("Op")) scn = scn.substring(0,scn.length()-2);
	return scn;
    }
}
// Following class of operators may need relocation
abstract class RelocOp extends Operator
{
    int dest=NOADDRESS;
    public RelocOp(int dest) {this(); this.dest=dest;}
    public RelocOp() {super();}
    public String toString()
	{return String.format("%s(%d)",super.toString(),dest);}
}
//////////////////////////////////////////////////
class AddOp extends Operator
{
    public AddOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o2 = vm.pop();
	Object o1 = vm.pop();
	if(o1 instanceof Integer && o2 instanceof Integer) {
	    int i1 = ((Integer)o1).intValue();
	    int i2 = ((Integer)o2).intValue();
	    int result = i1 + i2;
	    vm.push((Integer)result);
	} else
	    throw new Failure(vm,"Add applied to non-integer(s): "+o1+","+o2);
    }
}
class AssignOp extends Operator
{
    public AssignOp() {super();}
    public void execute(VM vm) throws Failure
    {
	VRef vref = (VRef)vm.pop();
	Object value = vm.pop();
	// Assign the value, taking output flags into account
	Var var = vm.assign(vref.name,value);
	vm.cc = false;
    }
}
class AssignLocalOp extends Operator
{
    public AssignLocalOp() {super();}
    public void execute(VM vm) throws Failure
    {
	VRef vref = (VRef)vm.pop();
	Object value = vm.pop();
	// Assign the value, taking output flags into account
	Var var = vm.assignLocal(vref.name,value);
	vm.cc = false;
    }
}
class BeginOp extends Operator
{
    public BeginOp() {super();}
    public void execute(VM vm) throws Failure
    {
    }
}
class BeginFcnOp extends Operator
{
    // Prepare the local space for the function
    public BeginFcnOp() {super();}
    public void execute(VM vm) throws Failure
    {
	vm.frame.locals = vm.locals;
	vm.locals = new HashMap<String,Var>();
	vm.localchange = true;
    }
}
class BeginStmtOp extends RelocOp
{
    int lineno=0;
    public BeginStmtOp(int lineno) {super();this.lineno=lineno;}
    public String toString()
	{return String.format("%s(%d)",super.toString(),lineno);}
    public void execute(VM vm) throws Failure
    {
	vm.frame.line = lineno;
	vm.frame.infailure = false;
	vm.cc = false;
	vm.frame.stp = vm.depth();
	vm.frame.endstmt = dest;
	vm.match.reset();
    }
}
class CVIOp extends Operator
{
    public CVIOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o = vm.pop();
	if(o instanceof Integer) {
	    // do nothing
	} else if(o instanceof String && ((String)o).length() == 0)
	    o = ZERO;
	else {
	    try {
		o = Integer.decode(o.toString());
	    } catch (NumberFormatException nfe) {
		throw new Failure(vm,"Object cannot be converted to integer:"+o);
	    }
	}
	vm.push(o);		
    }
}
class CVSOp extends Operator
{
    public CVSOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o = vm.pop();
	vm.push(o.toString());		
    }
}
class CVVOp extends Operator
{
    public CVVOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o = vm.pop();
	if(!(o instanceof VRef)) {
	    o = new VRef(o.toString());
	}
	vm.push(o);		
    }
}
class CallRetOp extends Operator // CALL support
{
    public CallRetOp() {}
    public void execute(VM vm) throws Failure
    {
	if(vm.frame.rp == NOADDRESS) {
	    vm.pc = vm.endprog;
	} else
	    vm.pc = vm.frame.rp;
	vm.frame.rp = NOADDRESS;
    }
}
class ConcatOp extends Operator
{
    int count;
    public ConcatOp(int n) {super(); this.count=n;}
    public String toString()
	{return String.format("%s(%d)",super.toString(),count);}
    public void execute(VM vm) throws Failure
    {
	if(count == 0)
	    throw new Failure(vm,"Concat with no arguments");
	String result = "";
	for(int i=0;i<count;i++) {
	    Object o = vm.pop();
	    result = (o.toString() + result);
	}
	vm.push(result);
    }
}
class DerefOp extends Operator
{
    public DerefOp() {super();}
    public void execute(VM vm) throws Failure
    {
	VRef vref = (VRef)vm.pop();
	Var var = vm.lookup(vref.name);
	if(var == null)
	    throw new Failure(vm,"Undefined variable: "+vref.name);
	Object value = var.value;
	if(var.stream != Constants.STDNULL) value = vm.inputTest(var);
	if(value == null)
	    vm.cc = true; // flag failure
	else {
	    vm.push(value);
	    vm.cc = false;
	}
    }
}
class DivOp extends Operator
{
    public DivOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o2 = vm.pop();
	Object o1 = vm.pop();
	if(o1 instanceof Integer && o2 instanceof Integer) {
	    int i1 = ((Integer)o1).intValue();
	    int i2 = ((Integer)o2).intValue();
	    int result = (int)(i1 / i2);
	    vm.push((Integer)result);
	} else
	    throw new Failure(vm,"Division applied to non-integer(s): "+o1+","+o2);
    }
}
class DupOp extends Operator
{
    public DupOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o1 = vm.top();
	vm.push(o1);
    }
}
class EndOp extends RelocOp
{
    public void execute(VM vm) throws Failure {vm.stop = true;}
}
class EndStmtOp extends RelocOp
{
    int fdest;
    public void execute(VM vm) throws Failure
    {
	// clear stack
	while(vm.depth() > vm.frame.stp) vm.pop();
	if(vm.cc) {
	    vm.frame.infailure = true;
	    vm.pc = fdest;
	} else {
	    vm.pc = dest;
	}
	vm.cc = false;
    }
    public String toString()
	{return String.format("%s(%d)",super.toString(),fdest);}
}
class FrameOp extends Operator
{
    DefFunction def; // the function being called
    // Save the VM frame info
    public FrameOp(DefFunction f) {super();this.def = f;}
    public String toString()
	{return String.format("%s(%s)",super.toString(),def);}
    public void execute(VM vm) throws Failure
    {
	int fp = vm.mark();
	Frame oldframe = vm.frame;
	oldframe.state = vm.match;
	vm.push(oldframe);
	Frame frame = new Frame();
	vm.frame = frame;
	frame.fp = fp;
	frame.fcn = def;
	vm.match = new MatchState(vm);
	// do tracing
	if(def.trace) {
	    // compute argument string
	    String s = "";	    	    
	    for(String formal: def.formals) {
		Var v = vm.lookup(formal);
		s += ("," + Debug.format(v.value));
	    }
	    s = s.substring(1); // remove the leading comma
	    vm.stdout.println(String.format("CALL: %s(%s)",def.name,s));
	}
    }
}
class FReturnOp extends Operator
{
    public FReturnOp() {super();}
    public void execute(VM vm) throws Failure
    {
	vm.cc = true;
	vm.pc = vm.frame.rp;
    }
}
class IJumpOp extends Operator
{
    public IJumpOp() {super();}
    public void execute(VM vm) throws Failure
    {
	VRef var = (VRef)vm.pop();
	// lookup the label dynamically
	Label dest = Snobol3.labels.get(var.name);
	if(dest == null)
	    throw new Failure(vm,"$ label expression: no such label: "+var.name);
	vm.pc = dest.address;	
    }
}
class JSROp extends RelocOp
{
    public JSROp() {super();}
    public JSROp(int dest){super(dest);}
    public void execute(VM vm) throws Failure
    {
	vm.frame.rp = vm.pc;
	vm.pc = dest;
    }
}
class JumpOp extends RelocOp
{
    public JumpOp() {super();}
    public JumpOp(int dest) {super(dest);}
    public void execute(VM vm) throws Failure {vm.pc = dest;}
}
/*
class LinenoOp extends Operator
{
    int lineno=0;
    public LinenoOp(int lineno) {super();this.lineno=lineno;}
    public String toString()
	{return String.format("%s(%d)",super.toString(),lineno);}
    public void execute(VM vm) throws Failure {vm.frame.line=lineno;}
}
*/
class MultOp extends Operator
{
    public MultOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o2 = vm.pop();
	Object o1 = vm.pop();
	if(o1 instanceof Integer && o2 instanceof Integer) {
	    int i1 = ((Integer)o1).intValue();
	    int i2 = ((Integer)o2).intValue();
	    int result = i1 * i2;
	    vm.push((Integer)result);
	} else
	    throw new Failure(vm,"Multiply applied to non-integer(s): "+o1+","+o2);
    }
}
class NegateOp extends Operator
{
    public NegateOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o1 = vm.pop();
	if(o1 instanceof Integer) {
	    int i1 = ((Integer)o1).intValue();
	    int result = 0 - i1;
	    vm.push((Integer)result);
	} else
	    throw new Failure(vm,"Negate applied to non-integer(s): "+o1);
    }
}
class NthOp extends Operator
{
    int n;
    public NthOp(int n) {super(); this.n=n;}
    public void execute(VM vm) throws Failure
    {
	Object o1 = vm.nth(n);
	vm.push(o1);
    }
}
class PrimitiveOp extends Operator
{
    PrimFunction fcn;
    public PrimitiveOp(PrimFunction fcn) {this.fcn = fcn;}
    public String toString()
	{return String.format("%s(%s)",super.toString(),fcn);}
    public void execute(VM vm) throws Failure
    {
	if(fcn.trace) { // call trace
	    // assume the arguments are on the stack
	    int n = fcn.nargs();
	    String s = "";	    	    
	    for(int i=0;i<n;i++) {
		Object o = vm.nth(i);
		s += ("," + Debug.format(o));
	    }
	    s = s.substring(1); // remove the leading comma
	    vm.stdout.println(String.format("CALL: %s(%s)",fcn.name,s));
	}
	vm.returnvalue = ""; // default return value
	fcn.execute(vm,fcn);
	if(fcn.trace) { // return trace
	    vm.stdout.println(String.format("RET: %s()=%s",
				fcn.name,Debug.format(vm.returnvalue)));
	}
	// Allow certain functions to avoid returning a value.
	// Currently only Call does this.
	if(!fcn.special) vm.push(vm.returnvalue);
    }
}
class PushOp extends Operator
{
    Object value;
    public PushOp(Object o) {super(); this.value=o;}
    public String toString()
	{return String.format("%s(%s)",super.toString(),Debug.format(value));}
    public void execute(VM vm) throws Failure {vm.push(value);}
}
class ReplaceOp extends Operator
{
    public ReplaceOp() {super();}
    public void execute(VM vm) throws Failure
    {
	// stack = <repl value>, <pat range>
	Object repl = vm.pop();
	Range range = (Range)vm.pop();
	VRef vref = vm.match.subjectvar;
	if(vref == null)
	    throw new Failure(vm,"ReplaceOp: subject is not replaceable");
	// get existing value of the var
	Var var = vm.lookup(vref.name);
	String sval = (String)(var==null?null:var.value);
	String result;
	if(range != null) {
	    if(sval == null)
		throw new Failure(vm,"Subject variable has no previous value: "+var);
	    // r0, rn are like String.substring arguments
	    // r0 is first char, rn is last+1
	    if(range.r0 >= sval.length() || range.rn > sval.length())
	        throw new Failure(vm,"Pattern match range too long: "+sval+range);
	    result = sval.substring(0,range.r0)
			  + repl.toString()
			  + sval.substring(range.rn,sval.length());
	} else {
	    result = repl.toString();
	}
	// assign the result, take output into account
	vm.assign(var.name,result);
    }
}
class RetValOp extends Operator
{
    public RetValOp() {super();}
    public void execute(VM vm) throws Failure
    {
	// extract the return result (= fcn name)
	Frame current = vm.frame;
	Var result = vm.lookup(current.fcn.name);
	if(result.value == null) result.value = "";
	vm.cc = false;
	vm.returnvalue = result.value;
    }
}
class ReturnOp extends Operator
{
    public ReturnOp() {super();}
    public void execute(VM vm) throws Failure
    {
	vm.push(vm.returnvalue);
	vm.cc = false;
	vm.pc = vm.frame.rp;
    }
}
class SubtractOp extends Operator
{
    public SubtractOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o2 = vm.pop();
	Object o1 = vm.pop();
	if(o1 instanceof Integer && o2 instanceof Integer) {
	    int i1 = ((Integer)o1).intValue();
	    int i2 = ((Integer)o2).intValue();
	    int result = i1 - i2;
	    vm.push((Integer)result);
	} else
	    throw new Failure(vm,"Subtract applied to non-integer(s): "+o1+","+o2);
    }
}
class SwapOp extends Operator
{
    public SwapOp() {super();}
    public void execute(VM vm) throws Failure
    {
	Object o1 = vm.pop();
	Object o2 = vm.pop();
	vm.push(o1);
	vm.push(o2);
    }
}
class UnframeOp extends Operator
{
    public UnframeOp() {}
    public void execute(VM vm) throws Failure
    {
	// Do return tracing first
	if(vm.frame.fcn.trace) { // return trace
	    vm.stdout.println(String.format("RET: %s()=%s",
				vm.frame.fcn.name,Debug.format(vm.returnvalue)));
	}
	Frame current = vm.frame;
	Frame oldframe = (Frame)vm.pop();
	vm.unmark(oldframe.fp);
	vm.frame = oldframe;
	vm.match = oldframe.state;
	vm.locals = oldframe.locals;
    }
}




