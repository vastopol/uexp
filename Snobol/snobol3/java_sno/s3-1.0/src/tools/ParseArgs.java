package tools;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.StringTokenizer;

public class ParseArgs
{
    static public HashMap parse(String[] argv,
				   HashMap props,
				   String[] formals)
	throws Exception
    {
	int argc = argv.length;
	int i;
parse:	for(i=0;i < argc;i++) {
	    String arg = canonical(argv[i]);
	    if(!arg.startsWith("-")) {
		// file all other args under the prop "--"
		ArrayList<String> a = (ArrayList<String>)props.get("--");
		if(a == null) {a = new ArrayList<String>(); props.put("--",a);}
		a.add(arg);
		continue;
	    }	
	    if(arg.equals("-")) {
		// stop all further argument examination
		ArrayList<String> a = (ArrayList<String>)props.get("--");
		if(a == null) {a = new ArrayList<String>(); props.put("--",a);}
		for(int j=i+1;j<argc;j++) a.add(argv[j]);
		break parse;
	    }
	    arg = arg.substring(1);
	    boolean negate = false;
	    String formal = match(formals,arg);
	    if(formal == null) {
		formal = match(formals,noBase(arg));
		if(formal == null || getTag(formal) != '?')
		    throw new Exception("-"+arg+": unknown option"); // invalid
		negate = true;
		arg = noBase(arg);
	    }
	    // switch on tag char
	    char tag = getTag(formal);
	    if(tag != '?' && i == argv.length)
	        throw new Exception("-"+arg+"requires an argument");
	    switch (tag) {
		case '?': // simple flag
		    props.put(arg,negate?Boolean.FALSE:Boolean.TRUE);
		    break;
		case '#': // single valued integer
		    String n = argv[++i].trim();
		    props.put(arg,new Integer(Integer.parseInt(n)));
		    break;
		case '=': // single valued
		    props.put(arg,argv[++i]);
		    break;
		case '*': // list valued		
		    ArrayList<String> a = (ArrayList<String>)props.get(arg);
		    if(a == null) {a = new ArrayList(); props.put(arg,a);}
		    collectWords(a,argv[++i]);
		    break;
		case '+': // list valued integers
		    ArrayList<Integer> ia = (ArrayList<Integer>)props.get(arg);
		    if(ia == null) {ia = new ArrayList<Integer>(); props.put(arg,ia);}
		    collectInts(ia,argv[++i]);
		    break;
	    }
	}
	return props;
      }

    static public String match(String[] sa, String s)
    {
	if(sa == null) return null;
	for(int i=0;i<sa.length;i++) {
	    String m = sa[i].substring(0,sa[i].length()-1);
	    if(s.equals(m)) return sa[i];
	}
	return null;
    }

    static public String noBase(String s)
    {
	if(s.substring(0,2).equalsIgnoreCase("no"))
	    return s.substring(2,s.length());
	else
	    return s;
    }

    static char getTag(String formal)
	{return formal.charAt(formal.length()-1);}

    static public void collectWords(ArrayList<String> a, String words)
    {
	if(words == null || a == null) return;
	StringTokenizer t = new StringTokenizer(words);
	while(t.hasMoreTokens()) {
	    a.add(t.nextToken(" ,")); // duplicates ok
	}
    }

    static public void collectInts(ArrayList<Integer> a, String words) throws Exception
    {
	if(words == null || a == null) return;
	StringTokenizer t = new StringTokenizer(words);
	while(t.hasMoreTokens()) {
	    String number = t.nextToken(" ,");
	    a.add(new Integer(Integer.parseInt(number.trim())));
	}
    }

    static public String canonical(String arg)
    {
	if (arg.startsWith("++")) arg = "-"+arg.substring(2);
	if (arg.startsWith("--")) arg = "-"+arg.substring(2);
	if (arg.startsWith("+")) arg = "-"+arg.substring(1);
	return arg;
    }


    static public String[] sconcat(String[] sa1, String[] sa2)
    {
	if(sa1 == null) return sa2;
	if(sa2 == null) return sa1;
	String[] cat = new String[sa1.length+sa2.length];
	System.arraycopy(sa1,0,cat,0,sa1.length);
	System.arraycopy(sa2,0,cat,sa1.length,sa2.length);
	return cat;
    }

}


