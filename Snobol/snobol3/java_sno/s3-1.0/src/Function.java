package jsnobol3;

import java.util.ArrayList;
import static jsnobol3.Constants.NOADDRESS;

abstract class Function extends Binding
{
    String name;
    boolean trace;
    boolean special = false;
    public Function(String name)
    {
	this.name = name;
	trace = false;
	special = false;
    }
    abstract public String toString();
    abstract public int nargs();
    abstract public boolean isPrimitive();
    abstract public ArgType typeFor(int argi);
}

class DefFunction extends Function
{
    ArrayList<String> formals = null;
    ArrayList<String> locals = null;
    Label label = null;
    int preamble = NOADDRESS; // address of the definition of the function
    
    public DefFunction(String name)
    {
	super(name);
	this.locals = new ArrayList<String>();
	this.formals = new ArrayList<String>();
    }

    public boolean isPrimitive() {return false;}
    public int nargs() {return formals.size();}
    public ArgType typeFor(int argi) {return ArgType.CVS;}

    public void addLocal(String l) {locals.add(l);}
    public void addFormal(String l) {formals.add(l);}

    public void setLabel(Label l) {label = l;}

    public String toString()
    {
	String sformal = "";
	for(int i=0;i<formals.size();i++) {
	    sformal += ((i>0?",":"")+formals.get(i));
	}
	String slocal = "";
	for(int i=0;i<locals.size();i++) {
	    slocal += ((i>0?",":"")+locals.get(i));
	}
	return String.format("Function(%s@[%s/%s](%s),%s)"
				,name,label.name,preamble,sformal,slocal);
    }
}

class PrimFunction extends Function
{
    // Note that same primitive may be in the function map
    // with multiple names
    
    Primitive prim;

    public PrimFunction(String name, Primitive p)
    {
	super(name);
	this.prim = p;
	special = p.isSpecial();
    }

    public boolean isPrimitive() {return true;}
    public int nargs() {return prim.nargs();}
    public ArgType typeFor(int argi) {return prim.typeFor(argi);}

    void execute(VM vm, PrimFunction fcn) throws Failure
	{prim.execute(vm,fcn);}

    public String toString()
    {
	return String.format("Function(%s,nargs=%d)",name,nargs());
    }
}
