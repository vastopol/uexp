package tools;

import java.util.Iterator;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Arrays;
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;

public class Parameters extends HashMap
{
    static final String EOL = "\n";

    String[] formals;

    public Parameters() {this(null);}

    public Parameters(String[] formals) {super(); this.formals=formals;}

    // Typed gets: string is default

    public String getEnv(String name)
    {
	Object o = get(name);
	return (o == null?null:o.toString());
    }

    public ArrayList<String> getSet(String name)
    {
	ArrayList<String> aset = (ArrayList<String>)get(name);
	if(aset == null) aset = new ArrayList<String>();
	return aset;
    }

    public String getString(String name) {return getEnv(name);}

    public int getInt(String name) {return getInt(name,-1);}

    public int getInt(String name, int dfalt)
    {
	Object o = get(name);
	if(o == null) return dfalt;
	int n = dfalt;
	if(o instanceof Number) n = ((Number)o).intValue();
	try {
	    n = Integer.parseInt(o.toString());
	} catch (NumberFormatException nfe) {
	    n = dfalt;
	}
	return n;
    }

    public boolean getBoolean(String name) {return getBoolean(name,false);}

    public boolean getBoolean(String name, boolean dfalt)
    {
	Object o = get(name);
	if(o == null) return dfalt;
	boolean b = dfalt;
	if(o instanceof Boolean) b = ((Boolean)o).booleanValue();
	else if(o instanceof Number) b = !(((Number)o).intValue()==0);
	else {
	    String s = o.toString();
	    if(s.equalsIgnoreCase("true")) b = true;
	    else if(s.equalsIgnoreCase("false")) b = false;
	}
	return b;
    }

    public Object putEnv(String name, Object val) {return put(name,val);}

    public String toString()
    {
	String s= null;
	for(Iterator it=super.keySet().iterator();it.hasNext();) {
	    String key = (String)it.next();
	    if(s == null) s = ""; else s = (s+" ");
	    s += (key+"=");
	    Object o = super.get(key);
	    if(o instanceof ArrayList) {
		ArrayList a = (ArrayList)o;
		s += ("[");	    
		for(int i=0;i<a.size();i++) s += (i==0?"":" ")+a.get(i);
		s+= ("]");
	    } else if(o instanceof Integer) {
		s += (o);
	    } else
		s += ("|"+o+"|");
	}
	return s;
    }

    public String dump()
    {
	// sort by property name
	String[] keyset = new String[size()];
 	int i=0;
	for(Iterator it=super.keySet().iterator();it.hasNext();)
	    keyset[i++] = (String)it.next();
	Arrays.sort(keyset);

	String s = "";
	for(String key : keyset) {
	    s += ("\t"+key+"=");
	    Object o = super.get(key);
	    if(o instanceof ArrayList) {
		ArrayList a = (ArrayList)o;
		s += ("[");	    
		for(i=0;i<a.size();i++) s += (i==0?"":" ")+a.get(i);
		s+= ("]");
	    } else if(o instanceof Integer) {
		s += (o);
	    } else
		s += ("|"+o+"|");
	    s += EOL;
	}
	return s;
    }

    public void load(String fname, boolean override) throws Error
    {
	if(fname == null) return;
	BufferedReader in = null;
	try {
	    in = new BufferedReader(new FileReader(fname));
	} catch(FileNotFoundException fnfe) {
	    throw new Error("File cannot be read: "+fname);
	}
	String line;
	for(;;) {	    
	    try {
		line=in.readLine();
	    } catch (IOException ioe) {
		throw new Error(ioe.toString());
	    }
	    if(line == null) break;
	    load1(line,override);
	}
    }

    // load a single name=value entry
    // use this.formals to to type checking.
    // if name is not in this.formals, then treat line "name="

    public void load1(String entry, boolean override) throws Error
    {
	// there are 3 cases:
	// 1. x=y   action: add pair(x,y)
	// 2. x=    action: remove x
	// 3. x	    action add pair(x,Boolean.TRUE)

	String name = null;
	Object value = null;
	int sep = entry.indexOf("=");
	if(sep < 0) { // case 3
	    name = entry;
	    value = Boolean.TRUE;
	} else { // case 1 or 2
	    name = entry.substring(0,sep);
	    value = entry.substring(sep+1,entry.length());
	}
	// canonicalize value
	if(value != null && value.toString().length() == 0) value = null;// 2
	// do override check and process value against formals
	if(override || get(name) == null) {
	    // check against the formals list
	    String formal = ParseArgs.match(formals,name);
	    if(formal == null) formal = name+"=";
	    char tag = (formal.charAt(formal.length()-1));
	    switch (tag) {
		case '#': // single valued integer
		    String n = ((String)value).trim();
		    put(name,new Integer(Integer.parseInt(n)));
		    break;
		case '*': // list valued		
		    ArrayList a = (ArrayList)get(name);
		    if(a == null) {a = new ArrayList(); put(name,a);}
		    ParseArgs.collectWords(a,value.toString());
		    break;
		case '+': // list valued integers
		    a = (ArrayList)get(name);
		    if(a == null) {a = new ArrayList(); put(name,a);}
		    try{ParseArgs.collectInts(a,value.toString());}
		    catch (Exception e) {throw new Error("Parameters.load",e);}
		    break;
		case '=': // single valued
		default:
		    if(value == null) remove(name); else put(name,value);
		    break;
	    }
	}
    }
}


