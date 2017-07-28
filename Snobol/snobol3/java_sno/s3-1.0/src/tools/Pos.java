package tools;

public class Pos
{
    static final int NP = -1;
    int col=NP;
    int line=NP;
    public Pos() {};
    public Pos set(int l) {return set(l,NP);}
    public Pos set(int l, int c) {col=c;line=l; return this;}
    public Pos set(Pos p) {return set(p.getLine(),p.getColumn());}
    public Pos dup() {return new Pos().set(this);}
    public int getLine() {return line;}
    public int getColumn() {return col;}
    public String toString()
    {
	if(line == NP) return "pos(?:?)";
	String s = "pos("+line;
	if(col != NP) s += (":"+col);
	s += ")";
	return s;
    }
}