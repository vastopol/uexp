package jsnobol3;

class Program
{
    Operator[] code;
    int length;

    public Program() {this(new Operator[Constants.INITIALCODESIZE]);}
    public Program(Operator[] p) {code = p; length=0;}

    public Program setCode(Operator[] code) {this.code=code; return this;}
    public Operator[] getCode()
    {
        return code;
    }

    public int size() {return (code==null?0:length);}
    public Operator get(int i) {return code[i];}
    public Operator set(int i,Operator op)
	{Operator o=code[i]; code[i]=op; return o;}
    public Program add(Operator op)
    {
	// extend if necessary
	if(length  >= code.length) {
	    Operator[] newcode = new Operator[length*2];
	    System.arraycopy(code,0,newcode,0,length);
	    code = newcode;
	}
	code[length++] = op;
	return this;
    }
}
