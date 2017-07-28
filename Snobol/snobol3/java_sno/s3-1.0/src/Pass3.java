package jsnobol3;

import java.util.Iterator;
import java.util.HashMap;
import java.util.ArrayList;
import static jsnobol3.Constants.NOADDRESS;
import tools.Pos;

class Pass3
{
    ArrayList<Binding> relocList;

    public Pass3()
    {
	relocList = new ArrayList<Binding>();
    }

    Program pass3(AST ast, int nstmts) throws Error
    {
	Program code = new Program();
	translate(ast,code);

	// go thru each defined function and generate the preamble;
	generatePreambles(code);

	// bind any unbound label references
	bind(code);

	if(Debug.isWhen(DebugPoint.PASS3))
		dumpCode(code,"|code|="+code.size());
	return code;
    }

    void bind(Program prog)
    {
	Operator[] code = prog.getCode();
	// finish up by doing binding
	for(int i=0;i<relocList.size();i++) {
	    Binding b = relocList.get(i);
	    // Only do relocation once
	    relocList.set(i,null);
	    if(b == null) continue;
	    RelocOp op = (RelocOp)code[i];
	    if(b instanceof Label)
		op.dest = ((Label)b).address;
	    else 
		op.dest = ((DefFunction)b).preamble;
	}
    }

    void generatePreambles(Program prog) throws Error
    {
	for(Iterator it=Snobol3.functions.keySet().iterator();it.hasNext();) {
	    Function f = (Function)Snobol3.functions.get(it.next());
	    if(!(f instanceof DefFunction)) continue;
	    translateDefinition((DefFunction)f,prog);
	}
    }

    void translate(AST prog, Program code) throws Error
    {
	code.add(new BeginOp());
	for(AST stmt: prog.subtrees) {
	    translateStatement(stmt,code);
	}
	int endaddr = point(code);
	Label endl = Snobol3.labels.get("end");
	endl.address = endaddr;
	code.add(new EndOp());
    }

    void translateStatement(AST stmt, Program code)
	throws Error
    {
	AST line = stmt.get(0);
	int beginpoint = point(code);
//	code.add(new LinenoOp((Integer)(line.getValue())));
	BeginStmtOp stmtbegin = new BeginStmtOp((Integer)(line.getValue()));
	code.add(stmtbegin);

	AST label = stmt.get(1); 
	if(label != null) {
	    Label l = getLabelFor(label.text());
	    l.address = beginpoint;
	}
	AST subj = stmt.get(2);
	AST pat = stmt.get(3);
	AST repl = stmt.get(4);
	AST br = stmt.get(5);
	if(subj != null) {
	    boolean isvarsubj = translateSubject(subj,code);
	    boolean nullpat = (pat == null);
	    if(!nullpat) translatePattern(pat,code,isvarsubj);
	    if(repl != null) translateReplacement(repl,code,nullpat);
	}
	int base = point(code);
	stmtbegin.dest = base;
	base = translateBranch(br,code);
    }

    // return true if the subject compiles to a name (direct/indirect)
    boolean translateSubject(AST subj, Program code)
	throws Error
    {
	boolean isvar = false;
	// handle var subjects specially
	if(subj.atype == AstType.NAME) {
	    code.add(new PushOp(new VRef(subj.text())));
	    isvar = true;
	} else if(subj.atype == AstType.REFERENCE) {
	    translateReference(subj,code);
	    isvar = true;
	} else {
	    translateExpr(subj,code);
	}
	return isvar;
    }

    void translatePattern(AST pat, Program code, boolean varsubj)
	throws Error
    {
	// if the subject is a var, compute its value, but
        // leave VRef also on stack
	if(varsubj) {
	    // stack: <vref>
	    code.add(new DupOp());
	    code.add(new DerefOp());
	    // stack: <value> <vref>
	    code.add(new SwapOp());
	    // stack: <vref> <value>
	} else {
	    // stack: <value>
	    code.add(new PushOp(null));
	    // stack: <vref=null> <value>
	}
	// stack: <vref|null> <value>
	// track the beginning of the code for each pattern element
	int prev = point(code);
	code.add(new BeginPatternOp());
	for(int i=0;i<pat.size();i++) {
	    PatternOp pop;
	    AST var;
	    AST patarg;
	    int failpoint; // dest for failures for this element 
	    AST pattern = pat.get(i);
	    // now compile the pattern itself
	    switch (pattern.atype) {
		case ARB:
		    var = pattern.get(0);
		    translatePatVar(var,code);
		    failpoint = point(code);
		    pop = new ArbOp();
		    pop.faildest = prev;
		    code.add(pop);
		    break;
		case LEN:
		    var = pattern.get(0);
		    patarg = pattern.get(1);
		    translatePatVar(var,code);
		    translateExpr(patarg,code);
		    code.add(new CVIOp());
		    failpoint = point(code);
		    pop = new LenOp();
		    pop.faildest = prev;
		    code.add(pop);
		    break;
		case BALANCE:	
		    var = pattern.get(0);
		    translatePatVar(var,code);
		    failpoint = point(code);
		    pop = new BalanceOp();
		    pop.faildest = prev;
		    code.add(pop);
		    break;
		case PATFCN:
		    var = pattern.get(0);
		    patarg = pattern.get(1);
		    translatePatVar(var,code);
		    AST fcn = patarg.get(0);
		    String fname = fcn.text();
		    pop = Snobol3.patterns.get(fname.toLowerCase());
		    if(pop == null)
			throw new Error(transMsg(pattern,"Undefined pattern function: "+fname));
		    pop = pop.clone();
		    AST actuals = patarg.get(1);
		    for(int j=0;j<actuals.size();j++) {
			AST arg = actuals.get(j);
			translateExpr(arg,code);
			switch (pop.typeFor(j)) {
			case CVA: break;
			case CVI: code.add(new CVIOp()); break;
			case CVS: code.add(new CVSOp()); break;
			case CVV: code.add(new CVVOp()); break;
			}
		    }
		    failpoint = point(code);
		    pop.faildest = prev;
		    code.add(pop);
		    break;
		default:
		    translateExpr(pattern,code);
		    code.add(new CVSOp());
		    failpoint = point(code);
		    pop = new StringPatternOp();
		    pop.faildest = prev;
		    code.add(pop);
		    break;
	    }
	    prev = failpoint;
	}
	code.add(new EndPatternOp());
    }

    void translatePatVar(AST var, Program code) throws Error
    {
	// generate pattern var code
	if(var == null) {
	    code.add(new PushOp(null));
	} else if(var.atype == AstType.NAME) {
	    code.add(new PushOp(new VRef(var.text())));
	} else { // its .REFERENCE
	    translateReference(var,code);
	}
    }

    void translateReplacement(AST tree, Program code,
			      boolean direct)
	throws Error
    {
	translateExpr(tree,code);
	// stack = <repl value>, <pat range>, <subj var>
	// If the pattern is null, then do a direct assignment
	if(direct) {
	    code.add(new SwapOp());
	    code.add(new AssignOp());
	} else
	    code.add(new ReplaceOp());
    }

    int translateBranch(AST branch, Program code)
	throws Error
    {
	AST go = null;
	AST s = null;
	AST f = null;
	boolean isUnconditional = false;
	if(branch != null) {
	    go = branch.get(0);
	    s = branch.get(1);
	    f = branch.get(2);
	}
	int base = point(code);
	EndStmtOp eso = new EndStmtOp();
	code.add(eso);
	// generate the code for each case and back fill pointers
	// If gotree != null, then it overrides the other two
	eso.dest = NOADDRESS;
	eso.fdest = NOADDRESS;
	if(go != null) {
	    eso.dest = point(code);
	    eso.fdest = point(code);
	    translateDest(go,code);
	} else {
	    if(s != null) {
	        eso.dest = point(code);
	        translateDest(s,code);
	    }
	    if(f != null) {
	        eso.fdest = point(code);
	        translateDest(f,code);
	    }
	}
	int stmtend = point(code);
	// backfill the cases not present
	if(eso.dest == NOADDRESS) eso.dest = stmtend;
	if(eso.fdest == NOADDRESS) eso.fdest = stmtend;
	return base;
    }

    void translateDest(AST dest, Program code)
	throws Error
    {
	if(dest.atype == AstType.LABEL) {
	    String label = dest.text();
	    if(label.equalsIgnoreCase("return")) {
	        code.add(new RetValOp());
	        code.add(new UnframeOp());
	        code.add(new ReturnOp());
	    } else if(label.equalsIgnoreCase("freturn")) {
	        code.add(new UnframeOp());
	        code.add(new FReturnOp());
	    } else {
		Label l = getLabelFor(label);
		if(l == null)
	 	    throw new Error(transMsg(dest,"Undefined Label:"+dest));
		setBinding(point(code),l);
	        code.add(new JumpOp());
	    }
	} else if(dest.atype == AstType.REFERENCE) {
	    translateReference(dest,code);
	    code.add(new IJumpOp());
	} else
	    throw new Error("Illegal Dest root: "+dest);
    }

    void translateReference(AST ref, Program code)
	throws Error
    {
	translateExpr(ref.get(0),code);
	code.add(new CVVOp());
    }

    void translateExpr(AST expr, Program code)
	throws Error
    {
	switch (expr.atype) {
	case NAME:
	    code.add(new PushOp(new VRef(expr.text())));
	    code.add(new DerefOp());
	    break;
	case STRING:
	    code.add(new PushOp(expr.text()));
	    break;
	case REFERENCE:
	    translateReference(expr,code);
	    code.add(new DerefOp());
	    break;
	case ADD:
	    translateExpr(expr.get(0),code);
	    code.add(new CVIOp());
	    translateExpr(expr.get(1),code);
	    code.add(new CVIOp());
	    code.add(new AddOp());
	    break;
	case SUBTRACT:
	    translateExpr(expr.get(0),code);
	    code.add(new CVIOp());
	    translateExpr(expr.get(1),code);
	    code.add(new CVIOp());
	    code.add(new SubtractOp());
	    break;
	case NEGATE:
	    translateExpr(expr.get(0),code);
	    code.add(new CVIOp());
	    code.add(new NegateOp());
	    break;
	case MULT:
	    translateExpr(expr.get(0),code);
	    code.add(new CVIOp());
	    translateExpr(expr.get(1),code);
	    code.add(new CVIOp());
	    code.add(new MultOp());
	    break;
	case DIV:
	    translateExpr(expr.get(0),code);
	    code.add(new CVIOp());
	    translateExpr(expr.get(1),code);
	    code.add(new CVIOp());
	    code.add(new DivOp());
	    break;
	case CONCAT:
	    for(AST term: expr.subtrees) {
		translateExpr(term,code);
		code.add(new CVSOp());
	    }
	    code.add(new ConcatOp(expr.size()));
	    break;
	case DEFINITION:
	    break; // ignore
	case FCNCALL:
	    AST fname = expr.get(0);
	    Function f = Snobol3.functions.get(fname.text());
	    if(f == null)
		throw new Error(transMsg(expr,"Undefined function: "+fname));
	    AST actuals = expr.get(1);
	    for(int i=0;i<actuals.size();i++) {
		AST arg = actuals.get(i);
		translateExpr(arg,code);
		switch (f.typeFor(i)) {
		case CVA: break;
		case CVI: code.add(new CVIOp()); break;
		case CVS: code.add(new CVSOp()); break;
		case CVV: code.add(new CVVOp()); break;
		}
	    }
	    if(f.isPrimitive()) {
		// we call primitives with a new frame, but no local space
		code.add(new PrimitiveOp((PrimFunction)f));
	    } else { // defined function; establish relocation
	        DefFunction df = (DefFunction)f;
		setBinding(point(code),df);
		code.add(new JSROp());
	    }
	    break;
	default:
	    throw new Error(transMsg(expr,"Illegal expression node: "+expr));
	}
    }

    // Walk an expression subtree to see if it is composed
    // only of operators that return null values; that is,
    // success/fail operators like .EQ, etc.
    boolean isNull(AST expr)
	throws Error
    {
	boolean isnull = false;
	switch (expr.atype) {
	case NAME:
	case STRING:
	case REFERENCE:
	case ADD:
	case SUBTRACT:
	case NEGATE:
	case MULT:
	case DIV:
	    isnull = false;
	    break;
	case CONCAT:
	    for(AST term: expr.subtrees) {
	    }
	    break;
	case DEFINITION:
	    isnull = true;
	    break; // ignore
	case FCNCALL:
	    AST fname = expr.get(0);
	    Function f = Snobol3.functions.get(fname.text());
	    if(f.isPrimitive()) {
		// we call primitives with a new frame, but no local space
	    } else { // defined function: assume it returns a value
		isnull = false;		
	    }
	    break;
	default:
	    throw new Error(transMsg(expr,"Illegal expression node: "+expr));
	}
	return isnull;
    }

    // translateDefinition is invoked only by Define.java
    // to translate the parse of argument 1 of the DEFINE()
    // function. There is one tricky point; one or more
    // arguments may have the same name as either a local variable
    // or the name of the function. In order not to wipe out
    // the actual value, check for this case and suppress the
    // non-actual assignment

    int translateDefinition(DefFunction fcn, Program code)
	throws Error
    {
	ArrayList<String> formals = fcn.formals;
	ArrayList<String> locals = fcn.locals;
	Label label = fcn.label;
	// save address of the def code itself
	int defbegin = point(code);
	// Establish the new local scope
	code.add(new BeginFcnOp());
	// Now assign the actuals to formals.
	// This must be done in reverse order
	// because of the stack order
	for(int i=formals.size()-1;i>=0;i--) {
	    String f = formals.get(i);
	    code.add(new PushOp(new VRef(f)));
	    code.add(new AssignLocalOp());
	}	
	// Similarly, init the locals (except for overlaps)
	boolean found;
	for(int i=0;i<locals.size();i++) {
	    String l = locals.get(i);
	    if(l == null || l.length() == 0) continue;
	    found = false;
	    for(int j=0;j<formals.size();j++) {
		if(l.equals(formals.get(j))) {found = true; break;}
	    }
	    if(found) continue;
	    code.add(new PushOp("")); // initialize with empty string
	    code.add(new PushOp(new VRef(l)));
	    code.add(new AssignLocalOp());
	}	
	// treat the function name as a local variable; again
	// checking for overlap
	found = false;
	for(int j=0;j<formals.size();j++) {
	    if(fcn.name.equals(formals.get(j))) {found = true; break;}
	}
	if(!found) {
	    code.add(new PushOp("")); // initialize with empty string
	    code.add(new PushOp(new VRef(fcn.name)));
	    code.add(new AssignLocalOp());
	}
	// Now save the frame data
	code.add(new FrameOp(fcn));
	// jump to the first statement (relocatable)
	setBinding(point(code),label);
	code.add(new JumpOp());
	// now save the address of the define in the function map
	fcn.preamble = defbegin;
	return defbegin;
    }

    //////////////////////////////////////////////////
    // Utility functions
    void setBinding(int opaddr, Binding b)
    {
	if(b != null) {
	    while(relocList.size() <= opaddr) relocList.add(null);
	    relocList.set(opaddr,b);
	}
    }

    Label getLabelFor(String label)
    {
	Label l = Snobol3.labels.get(label);
	return l;
    }

    int point(Program code) {return code.size();}

    String transMsg(AST ast, String msg)
	{return transMsg(ast.findpos(),msg);}

    String transMsg(Pos pos, String msg)
    {
	return (pos.toString()+"; "+msg);
    }

    //////////////////////////////////////////////////

    static public void dumpCode(Program code, String title)
    {
	Debug.Println(title);	
	for(int addr=0;addr<code.size();addr++) {
	    Debug.Println("[%3d]: %s",addr,code.get(addr));
	}
    }
}

