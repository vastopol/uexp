package jsnobol3;

class Label extends Binding
{
    String name;
    int lineno;
    int address;
    DefFunction function;

    public Label(String name, int lineno)
    {
	this.name = name;
	this.lineno = lineno;
	this.address = 0;
	this.function = null;
    }

    public void setFunction(DefFunction f) {function=f;}
    public Function getFunction() {return function;}

    public String toString()
    {
	return String.format("Label(%s @ %d/%d)",name,lineno,address);
    }
}
