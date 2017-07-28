package jsnobol3;

import tools.*;

class Error extends Exception
{
    public Error(String arg) {super(arg);}
    public Error(String arg, Throwable cause) {super(arg,cause);}
    public Error(Throwable cause) {super(cause);}
}

class Failure extends RuntimeException
{
    public Failure(VM vm, String...argv) {super(makeMessage(vm,argv));}
    public Failure(VM vm, Throwable cause, String...argv)
	{super(makeMessage(vm,argv),cause);}
    static String makeMessage(VM vm, String[] argv)
    {
	String Message = "";
	if(argv.length == 0) {
	    Message = "";
	} else if(argv.length == 1) {
	    Message = argv[0];
	} else {
	    Message = String.format(argv[0],Util.resection(1,(Object[])argv));
	}
	return "line "+vm.frame.line+": "+Message;
    }
}
