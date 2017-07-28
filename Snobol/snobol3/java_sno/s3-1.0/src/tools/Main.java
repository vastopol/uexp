package tools;

import java.io.InputStreamReader;
import java.io.FileReader;
import java.io.BufferedReader;
import java.util.HashMap;
import java.util.ArrayList;

import tools.*;

abstract public class Main
{
    // Following are static because they are referenced
    // in other classes
    static public Parameters parms;

    // This is set by our subclass in a static block
    static Main main;

    // Define default legal cmdline and envfile arguments
    static String[] defaultFormals = new String[] {
	"envfile=",
	"env=",
	"v?",
	"debug?",
	"debugn#",
	"debugclasses*",
	"debugpoints*",
	"stacktrace?"
    };

    // should only be called from subclass main
    static protected void main(String[] argv, Main main)
    {
	Main.main = main;
        try {

	String[] formals = main.getFormals();

	// Load in order, with later overriding earlier
	// 1. this class and subclass defaults
	// 2. env file
	// 3. command-line args

	Parameters subparms = new Parameters(formals); // case 1
	Parameters envparms = new Parameters(formals); // case 2
	Parameters cmdparms = new Parameters(formals); // case 3

	main.setDefaults(subparms); // case 1: this class and subclass defaults

	tools.ParseArgs.parse(argv,(HashMap)cmdparms,formals);// 2

	// Use cmdparms to find any env or envfile parameters

	// load the env file, if any
	String envf = cmdparms.getString("envfile");
	if(envf != null) {envparms.load(envf,false);}

	// load any  env args to override env file
	ArrayList envargs = (ArrayList)cmdparms.get("env");
	if(envargs != null) {
	    for(int i=0;i<envargs.size();i++)
		envparms.load1((String)envargs.get(i),false);
	}

	// Now merge all the parameter sets
	parms = new Parameters();
	parms.putAll(subparms);
	parms.putAll(envparms);
	parms.putAll(cmdparms);
	
	// now finalize the parameters
	main.finalizeParameters();	

	if(parms.getBoolean("v"))
	    System.err.println("parameters=("+parms.toString()+")");

	// Now let the main do all the initialization
	main.initialize();

	// start the interpreter
	main.start();
	
        } catch(Exception e) {
	    System.err.println(e.toString());
	    if(Main.parms.getBoolean("v")) e.printStackTrace();
	    System.exit(1);
        };
    }

    //////////////////////////////////////////////////

     static void usage(String msg) {usage(msg,null);}
     static void usage(Exception e) {usage(e.toString(),e);}

     static void usage(String msg, Exception e)
     {
	System.err.println(msg);
	if(e != null && Main.parms.getBoolean("v"))
	    e.printStackTrace(System.err);
	System.exit(1);
     }


    //////////////////////////////////////////////////

    public Main()
    {
    }

    protected void initialize() throws Exception
    {
    }

    protected String[] getFormals() {return defaultFormals;}

    abstract protected void setDefaults(Parameters parms);
    abstract protected void finalizeParameters() throws Exception;
    abstract protected void start() throws Exception;
}
