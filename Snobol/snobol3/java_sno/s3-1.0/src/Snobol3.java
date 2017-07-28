package jsnobol3;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import tools.*;
import static jsnobol3.Constants.*;

public class Snobol3 extends Main
{

    static final Class thisclass = Snobol3.class;

    //////////////////////////////////////////////////
    // Define legal cmdline arguments

    static String[] formals = new String[] {
	"startup?",
	"exec?",
	"trace*",
	"mode*",
	"lint?",
    };

    //////////////////////////////////////////////////

    static public void main(String[] argv) throws Exception
    {
	Main thismain = (Main)Factory.newInstance(thisclass);
	Main.main(argv,thismain);
    }
 
    //////////////////////////////////////////////////

    // These are shared by all parts of the compiler

    static HashMap<String,Label> labels = new HashMap<String,Label>();
    static HashMap<String,Function> functions = new HashMap<String,Function>();
    static HashMap<String,PatternOp> patterns = new HashMap<String,PatternOp>();
    static Modes modes = new Modes();

    //////////////////////////////////////////////////

    Compiler compiler = null;
    VM vm = null;

    public Snobol3() {super();}

    @abstractbody protected void setDefaults(Parameters parms)
    {
	parms.put("debug",0);
	parms.put("startup",false);
	parms.put("exec",true);
    }

    // set some Main. flags

    @abstractbody protected void finalizeParameters() throws Exception
    {
    }

    @override protected String[] getFormals()
    {
	return Util.mergeFormals(this.formals,super.getFormals());
    }

    @override protected void initialize() throws Exception
    {
	super.initialize();
	// Initialize static class values
	// (must be a better way to do this)
	Debug.initializeClass();
	Primitive.initializeClass();
	PatternOp.initializeClass();
	// process the command line mode arguments, if any
	modes.setModeList(Main.parms.getSet("mode"));
	// Create the components
	compiler = new Compiler();
	vm = new VM();

    }

    @abstractbody protected void start()
    {
    // Compilation operates in multiple passes:
    // pass 1: read the whole program and process labels,
    //         continuations, comments, and control words
    // pass 2: compile each program to a tree form
    // pass 2: compile tree to a sequence of operator objects

//	System.out.println("start parameters:\n"+parms.dump());
	ArrayList<String> argv = Main.parms.getSet("--");
	// Execute the first argument
	if(argv.size() == 0) {
	    System.out.println("exit");
	    System.exit(0);
	}
	String fname = argv.get(0);
	try {
	    Program program = compiler.compile(fname);
	    if(Main.parms.getBoolean("exec")) {
		vm.execute(program,argv);
	    }	    
	} catch (Error er) {
	    Debug.Println(er.toString());
	    Debug.PrintStackTrace(er);
	    System.exit(1);
	} catch (Failure ef) {
	    Debug.Println("Execution failure: "+ef.toString());
	    Debug.PrintStackTrace(ef);
	    System.exit(1);
	} catch (Exception ex) {
	    Debug.Println(ex.toString());
	    Debug.PrintStackTrace(ex);
	    System.exit(1);
	}
    }

}
