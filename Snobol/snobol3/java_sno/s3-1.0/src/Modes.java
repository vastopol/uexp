package jsnobol3;

import java.util.StringTokenizer;
import java.util.ArrayList;

class Modes
{
    boolean anchored = false;
    boolean dump = false;
    boolean dumper = false;

    void setModeList(ArrayList<String> modelist) throws Error
    {
	for(String mode: modelist) Snobol3.modes.setMode(mode);
    }

    void setModeList(String modelist) throws Error
    {
	StringTokenizer tok = new StringTokenizer(modelist,",");
	while(tok.hasMoreTokens()) {
	    String mode = tok.nextToken();
	    setMode(mode);
	}
    }

    void setMode(String mode0) throws Error
    {
	boolean on = true;
	String mode = mode0;
	if(mode.startsWith("no")) {
	    on = false;
	    mode = mode.substring(2);	
	}
	if(mode.equalsIgnoreCase("anchor")) {
	    anchored = on;
	} else if(mode.equalsIgnoreCase("unanchor")) {
	    anchored = !on;
	} else if(mode.equalsIgnoreCase("dump")) {
	    dump = on;
	} else if(mode.equalsIgnoreCase("dumper")) {
	    dumper = on;
	} else {
	    throw new Error("Unknown MODE argument:"+mode0);
	}
    }

    public String toString()
    {
	return String.format(
	    "anchored=%b dump=%b dumper=%b",
	    anchored,dump,dumper);
    }
}

