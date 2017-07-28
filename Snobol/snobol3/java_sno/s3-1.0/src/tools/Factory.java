package tools;

import java.lang.reflect.Method;
import java.lang.reflect.Field;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

public class Factory
{
    static Object[] nullargs = (Object[])null;

    static String[] packagelist = null;

    static public void initializeClass(String[] pl) {packagelist = pl;}

    static public Class getClass(String cname) throws Error
	{return getClass(cname,null);}

    static public Class getClass(String clname, String defalt) throws Error
    {
	if(clname == null) clname = defalt;
	if(clname == null)
	    throw new Error("Factory: null class specified");
	Class cl = null;
	for(String prefix : packagelist) {
	    try {
	        if(prefix.length()==0) prefix = null;
		String fullname = (prefix==null?clname:(prefix+"."+clname));
	        cl = Class.forName(fullname);
		break; // only reached if class found
	    } catch (ClassNotFoundException cnfe) {cl=null;}
	}	
	if(cl == null)
	    throw new Error("Factory.getClass: class not found: "+clname);
	return cl;
    }

    static public Object newInstance(String cname) throws Error
	{return newInstance(cname,nullargs);}

    static public Object newInstance(String cname, Object... args) throws Error
    {
	Class cl = getClass(cname);
	return Factory.newInstance(cl,args);
    }

    static public Object newInstance(Class cl) throws Error
	{return newInstance(cl,nullargs);}

    static public Object newInstance(Class cl, Object... args) throws Error
    {
	try {
	    if(args == null) {
		return cl.newInstance();
	    } else {
		Object arg;
		Constructor cons = null;
		try {
		    cons = cl.getConstructor(Object.class);
		    arg = args[0];
		}
	        catch (NoSuchMethodException nsme) {
		    cons = cl.getConstructor(Object[].class);
		    arg = args;
		}
		return cons.newInstance(new Object[]{arg});
	    }
	} catch (Exception e) {
	    throw new Error("Factory.newinstance: "+cl,e);
	}
    }

    static public Object getProperty(String cname, String pname)
	throws Exception
	{return Factory.getProperty(Factory.getClass(cname),pname);}

    static public Object getProperty(Class cl, String pname) throws Exception
    {
	try {
	    Method m = cl.getMethod(pname,(Class[])null);
	    return m.invoke(null);
	} catch (Exception e) {
	    throw new Exception(e.toString());
	}
    }

    static public Object getField(String cname, String pname) throws Exception
	{return Factory.getField(Factory.getClass(cname),pname);}

    static public Object getField(Class cl, String pname) throws Exception
    {
	try {
	    Field f = cl.getField(pname);
	    return f.get(null);
	} catch (Exception e) {
	    throw new Exception(e.toString());
	}
    }

    static String rootClassName(Class cl)
    {
	String s = cl.getName();
	int i = s.lastIndexOf('.');
	if(i >= 0) {
	    s = s.substring(i+1,s.length());
	}
	return s;
    } 


/*
    // classInitialize method may or may not have a single argument

    static public void classInitialize(Class cl) throws Error
    {
	try {
	    java.lang.reflect.Method m
		= cl.getMethod(Constants.DEFAULTCLASSINIT,
				(Class[])null);
	    m.invoke(null);
	    return;
	} catch (Exception e) {
	    throw new Error(e.toString());
	}
    }

    static public void classInitialize(Class cl, Object arg) throws Error
    {
	try {
	    java.lang.reflect.Method m
		= cl.getMethod(Constants.DEFAULTCLASSINIT,Object.class);
	    m.invoke(null,arg);
	    return;
	} catch (Exception e) {
	    throw new Error(e.toString());
	}
    }
*/    

}
