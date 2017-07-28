package tools;

import java.io.PrintWriter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;

import tools.Parameters;

abstract public class AbstractDebug
{
    static final int defaultLevel = 0;

    static protected AbstractDebug inactive = null;
    static protected AbstractDebug active = null;

    static protected PrintWriter out = new PrintWriter(System.err);
    static public int debugLevel = 0;
    static protected HashMap<Class,Integer> classes
		= new HashMap<Class,Integer>();
    static public HashSet<Integer> points = new HashSet<Integer>();

    static protected Parameters parms = null;

    static protected AbstractDebug When(Object... tests)
	{return choose(andTest(tests));}
    static protected AbstractDebug WhenAny(Object... tests)
	{return choose(orTest(tests));}

    static protected AbstractDebug choose(boolean b)
	{return b?AbstractDebug.active:AbstractDebug.inactive;}

    static protected boolean andTest(Object... tests)
    {
	if(tests == null) {
	    return (true); // default is true
	}
	boolean ok = true;
	for(Object o : tests) {
	    if(o instanceof Integer
	       && AbstractDebugPoint.member((Integer)o)) {
		Integer flag = (Integer)o;
		ok &= (points.contains(flag));
	    } else if(o instanceof Integer) {
		ok &= (debugLevel >= (Integer)o);
	    } else if(o instanceof Boolean) {
		ok &= ((Boolean)o);
	    } else if(o instanceof Class) {
		ok &= testClass((Class)o);
	    } else {
		ok &= testClass(o.getClass());
	    }
	}
	return (ok);
    }

    static protected boolean orTest(Object... tests)
    {
	if(tests == null) return (true); // default is true
	boolean ok = false;
	for(Object o : tests) {
	    if(o instanceof Integer
	              && AbstractDebugPoint.member((Integer)o)) {
		Integer flag = (Integer)o;
		ok |= (points.contains(flag));
	    } else if(o instanceof Integer) {
		ok |= (debugLevel >= (Integer)o);
	    } else if(o instanceof Boolean) {
		ok |= ((Boolean)o);
	    } else if(o instanceof Class) {
		ok |= testClass((Class)o);
	    } else {
		ok |= testClass(o.getClass());
	    }
	}
	return (ok);
    }

    static public void setWhen(Object... tests)
    {
	try {

	if(tests == null) return;
	for(Object o : tests) {
	    if(o instanceof Integer
	       && AbstractDebugPoint.member((Integer)o)) {
		points.add((Integer)o);
	    } else if(o instanceof Integer) {
		AbstractDebug.debugLevel=(Integer)o;
	    } else if(o instanceof Boolean) {
		AbstractDebug.debugLevel=((Boolean)o)?defaultLevel:0;
	    } else if(o instanceof Class) {
		increment((Class)o);
	    } else {
		throw new Error("Debug.setWhen: unrecognized flag: "+o);
	    }		
	}

	} catch (Exception e) {Fail(e);}
    }

    static public void setWhenNot(Object... tests)
    {
	try {
	if(tests == null) return;
	for(Object o : tests) {
	    if(o instanceof Integer
	       && AbstractDebugPoint.member((Integer)o)) {
		points.remove((Integer)o);
	    } else if(o instanceof Class) {
		decrement((Class)o);
	    } else if(o instanceof Integer || o instanceof Boolean) {
		// ignore
	    } else {
		throw new Error("Debug.setWhen: unrecognized flag: "+o);
	    }		
	}
	} catch (Exception e) {Fail(e);}
    }

    static boolean testClass(Class cl)
    {
	Integer count = 0;
	if(cl!=null) count = classes.get(cl);
	if(count == null) count = 0;
	boolean ok = (count > 0);
	return ok;
    }

    static void increment(Class cl)
    {
	Integer count = classes.get(cl);
	if(count == null) count = 0;
	count++;
	classes.put(cl,(Integer)count);
    }

    static void decrement(Class cl)
    {
	Integer count = classes.get(cl);
	if(count == null) count = 0; else count--;
	if(count == 0) classes.remove(cl); else classes.put(cl,(Integer)count);
    }

    static String report()
    {
	String s = "classes[";
	for(Iterator it=classes.keySet().iterator();it.hasNext();) {
	    Class cl = (Class)it.next();
	    Integer count = classes.get(cl);
	    s += String.format("%s=>%d ",cl,count);
	}
	s += "]";
	return s;
    }

    static public void setWriter(PrintWriter p) {AbstractDebug.out = p;}

    static public void Fail(Object...args)
    {	
	try {
	    System.err.println(String.format("%s",args));
	} catch (Exception ex) {ex.printStackTrace(System.err);}
	System.exit(1);
    }

    static public void Fail(Exception e)
    {	
	e.printStackTrace(System.err);
	System.exit(1);
    }

    //////////////////////////////////////////////////

    static public void Print(Object... args)
    {
	try {
	    String msg = "";
	    if(args != null) {
		switch(args.length) {
		case 0: break;
		case 1: msg = args[0].toString(); break;
		default:
		    String fmt = (String)args[0];
		    Object[] parms = (Object[])Util.resection(1,args);
		    msg = String.format(fmt,parms);
		    break;
		}
	    }
	    System.out.print(msg);
	    System.out.flush();
	} catch (Exception ex) {Fail(ex);}
    }

    static public void Println(Object... args)
    {
	try {
	    String msg = "";
	    if(args != null) {
		switch(args.length) {
		case 0: break;
		case 1: msg = ""+args[0]; break;
		default:
		    String fmt = ""+args[0];
		    Object[] parms = Util.resection(1,args);
		    if(fmt != null && parms.length > 0) {
			msg = String.format(fmt,parms);
		    } else msg = fmt;
		    break;
		}
	    }
	    System.out.println(msg);
	    System.out.flush();
	} catch (Exception ex) {Fail(ex);}
    }

    static public void PrintStackTrace(Exception e)
    {
	if(parms.getBoolean("stacktrace"))
	    e.printStackTrace(System.out);
    }

    //////////////////////////////////////////////////

    static public void initializeClass()
    {
	AbstractDebug.parms = Main.parms;
	int level = 0;
	if(parms.getBoolean("debug")) level = 1;
	level = parms.getInt("debugn",level);
	setWhen(level);
	ArrayList<String> classes = parms.getSet("debugclasses");
	Class cl;
	if(classes != null) {
	    for(Object o : classes) {
		try {cl = Factory.getClass(o.toString());}
		catch (Exception e) {cl = null;}
		if(cl == null) Fail("Unknown debug class: "+o);
		setWhen(cl);		
	    }
	}
	ArrayList<String> pointset = parms.getSet("debugpoints");
	if(pointset != null) {
	    for(String s : pointset) {
		int p = AbstractDebugPoint.pointFor(s);
		if(p == AbstractDebugPoint.BASE)
		    Fail("Unknown debug point: "+s);
		setWhen(p);
	    }
	}
    }

    //////////////////////////////////////////////////

    protected boolean isactive = false;
    public AbstractDebug() {}
    public void print(Object ... args)
	{if(isactive) {AbstractDebug.Print(args);} else {};}
    public void println(Object ... args)
	{if(isactive) {AbstractDebug.Println(args);} else {};}

}

			      
