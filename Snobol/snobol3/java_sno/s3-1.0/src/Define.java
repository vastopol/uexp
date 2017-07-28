package jsnobol3;

import java.util.ArrayList;

import tools.*;

class Define extends Primitive
{
    static final int NLOCALS = 4;
    FunctionCompiler fc;

    public Define(FunctionCompiler fc) {super(); this.fc = fc;}
    public int nargs() {return 2+NLOCALS;}
    void execute(VM vm, PrimFunction fcn) throws Failure
    {
	vm.cc = false;
	setReturn(vm); // return null and always succeed at execution time
    }

    // Do the initial processing of the function definition
    // Return a revised tree
    public void pass2(AST deftree) throws Error
    {
	AST args = deftree.get(1);
	// Make sure that all the arguments are string constants
	for(int i=0;i<args.size();i++) {
	    AST arg = args.get(i);
	    if(arg.atype != AstType.STRING)
		throw new Error(deftree.findpos()+"; DEFINE arguments must be string constants");
	}
	// Construct a list of the local variables, if any
	ArrayList<String> locallist = new ArrayList<String>();
	if(args.size() > 2) {
	    for(int i=2;i<args.size();i++) {
		AST name = args.get(i);
		String text = name.text();
	        if(text != null && text.length() > 0)
		    locallist.add(text);
	    }
	}
	// Extract a Label from the label argument
	AST l = args.get(1);
	// see if this is a legal label
	Label label = Snobol3.labels.get(l.text());
	if(label == null)
	    throw new Error(deftree.findpos()+"; DEFINE: undefined label:"+l.text());

	// Finally, pull out the form definition and process it to
	// get three things: function name, number of args,
	// and the name of the formals
	AST form = args.get(0);
	String fcndef = form.text();
	// indent the form with blanks
	// to make the position reports correct
	fcndef = (Util.indent(form.findpos().getColumn())) + fcndef;
	Ref<AST> tree = new Ref<AST>();
	fc.open(fcndef,deftree.findpos().getLine());
	if(!fc.parser.fcndecl(tree))
	    throw new Error(deftree.findpos()+"; DEFINE: malformed function definition; expected NAME; found: "+fc.parser.consumed());
	AST decltree = tree.deref();
	// extract the function name
	AST fname = decltree.get(0);
	// extract the formal arguments
	AST f = decltree.get(1);
	ArrayList<AST> formals = f.subtrees;
	// Define the function, if not already defined
	if(Snobol3.functions.get(fname.text()) != null)
	    throw new Error(deftree.findpos()+"; DEFINE: multiply defined function:"+fname);
	DefFunction deff = new DefFunction(fname.text());
	// Link label and the function bi-directionally
	deff.setLabel(label);
	label.setFunction(deff);
	// Add the formals to the definition
	for(int i=0;i<formals.size();i++) {
	    deff.addFormal(formals.get(i).text());		
	}	    
	// Add the locals to the definition
	for(int i=0;i<locallist.size();i++) {
	    deff.addLocal(locallist.get(i));
	}	    
	Snobol3.functions.put(deff.name,deff);
    }

}
