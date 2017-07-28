package tools;

public class Util
{
    static public int memcpy(byte[] src, int srcoffset,
			     byte[] dst, int dstoffset,
			     int len)
    {
	if(srcoffset < 0 || dstoffset < 0
	   || srcoffset+len > src.length
	   || dstoffset+len > dst.length)
	{
	    System.err.printf("Util.memcpy: |src|=%d srcoffset=%d |dst|=%d dstoffset=%d len=%d\n",src.length,srcoffset,dst.length,dstoffset,len);
	    System.exit(1);
	}
	System.arraycopy(src,srcoffset,dst,dstoffset,len);
	return dstoffset+len;
    }

    static public String[] mergeFormals(String[] fnew, String[] fold)
    {
	String[] merge = new String[fnew.length+fold.length];
	System.arraycopy(fnew,0,merge,0,fnew.length);
	int cursor = fnew.length;
loop:	for(int i=0;i<fold.length;i++) {
	    String fo = fold[i];
	    fo = fo.substring(0,fo.length()-1); // drop marker type
	    // add only if not already there
	    for(int j=0;j<cursor;j++) {
		String fm = merge[j];
		if(fo.equals(fm.substring(0,fm.length()-1))) continue loop;
	    }
	    merge[cursor++] = fold[i];
	}
	// truncate merge to proper size
	String[] result = new String[cursor];
	System.arraycopy(merge,0,result,0,cursor);
	return result;
    }

    static public int ceilingDiv(int num, int denom)
	{return (int)((num+denom-1)/denom);}


    static Object[] nullparms = new Object[0];

    // shorten (or lengthen an Object... arg vector
	
    static public Object[] resection(int start, Object... args)
    {
	if(args == null) args = nullparms;
	if(start > args.length) {
	    System.err.println("resection: invalid start argument="+start+"; "+args.length);
	    System.exit(1);
	}
	int arglen = args.length;
	Object[] parms = new Object[arglen-start];
	int src;
	int dst;
	int len;
	if(start < 0) {src=0; dst=-start; len=arglen;}
	else {src=start; dst=0; len=parms.length;}
	System.arraycopy(args,src,parms,dst,len);
	return parms;
    }

    static public String trimRight(String s)
    {
	if(s == null || s.length() == 0) return s;
	int i = s.length()-1;
	while(i >= 0 && s.charAt(i) <= ('\u0020')) i--;
	if(i < 0) return "";
	return s.substring(0,i+1);
    }

    static StringBuilder indents = new StringBuilder();

    static public String indent(int n) {return indent(n,' ');}

    static public String indent(int n, char ch)
    {    
	indents.setLength(0);
	while(n > indents.length()) indents.append(ch);
	return indents.toString();
    }

    static public String simpleClassName(Object o)
    {
	String name = o.getClass().getName();
	if(name.endsWith(".class")) name = name.substring(0,name.length()-6);
	int index = name.lastIndexOf(".");
	if(index > 0) name = name.substring(index+1,name.length());
	return name;
    }
}
