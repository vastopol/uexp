package tools;

abstract public class AbstractDebugPoint
{
    static public final int offset = 0x10000000;
    static public final int BASE = offset;
    static public String[] names = null; // filled in by debug class

    static public boolean member(Integer point)
	{return member(point.intValue());}
    static public boolean member(int point)
	{return (point & offset) == offset;}
    static public String nameOf(Integer point)
	{return nameOf(point.intValue());}
    static public String nameOf(int point)
    {
	point &= (~offset);
	if(point < 0 || point >= names.length) return (""+point);
	return names[point];
    }
    static public int pointFor(String p)
    {
	for(int i=0;i<names.length;i++) {
	    if(names[i].equalsIgnoreCase(p)) return (i|offset);
	}
	return BASE;
    }
}
