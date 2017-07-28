package jsnobol3;

// This goes in the scope maps
class Var
{
    String name;
    int stream;
    boolean trace;
    boolean isLocal;
    Object value;

    // io flags
    boolean nonewline;


    public Var(String name)
    {
	this.name=name;
	isLocal = false;
	stream = Constants.STDNULL;
	trace = false;
	value = null;
	nonewline=false;
    }
    public String toString()
    {
	String s = "<"+name;
	if(trace) s += ("*");
	if(isLocal) s += (",local");
	if(stream != Constants.STDNULL) s += (",stream="+stream);
	s += (",value=|"+value+"|");
	s += ">";
	return s;
    }
}

class VRef
{
    String name;
    public VRef(String name) {this.name=name;}
    public String toString() {return "$<"+name+">";}
}

// Parent for labels and function
abstract class Binding {}



