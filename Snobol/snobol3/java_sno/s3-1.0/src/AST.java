package jsnobol3;

import java.util.ArrayList;

import tools.*;

class AST
{
    boolean isLeaf;
    AstType atype;
    Pos pos = null;
    ArrayList<AST> subtrees = null;

    public AST() {}
    public AST(AstType atype, Pos pos) {this(atype); setPos(pos);}
    public AST(AstType atype)
	{this.atype = atype; subtrees = new ArrayList<AST>(); isLeaf = false;}
    public AST setPos(Pos p) {pos = p.dup(); return this;}
    public Pos getPos() {return pos;}
    public Pos findpos()
    {
	// walk tree in preorder looking for a defined position
	if(pos != null) return pos;
	if(subtrees != null) {
	    for(int i=0;i<subtrees.size();i++) {
	        if((pos = subtrees.get(i).findpos()) != null) return pos;
	    }
	}
	return null;
    }

    public int size() {return subtrees.size();}
    public AST get(int i)
    {
if(isLeaf) {System.err.println("leafed");}
	if(subtrees.size() <= i) return null;
	return subtrees.get(i);
    }
    public AST add(Ref<AST> child1) {
if(isLeaf) {System.err.println("leafed");}
add(child1.deref()); return this;}
    public AST add(AST child1) {
if(isLeaf) {System.err.println("leafed");}
subtrees.add(child1); return this;}

    // overridden in LeafAST
    public Object getValue() {return null;}
    public AST setValue(Object v) {return this;}
    public String text() {return null;}

    public String toString()
    {
	String s = "";
	s += "(";	
	s += atype;
	s += (pos==null?"":" "+pos);
	for(int i=0;i<subtrees.size();i++) {
	    s += (" "+subtrees.get(i));
	}
	s += ")";
	return s;
    }

    //////////////////////////////////////////////////
    static final int ppindent=2;

    public String prettyPrint() {return AST.prettyPrint(this,0);}

    static String prettyPrint(AST ast, int indent)
    {
	String ind = Util.indent(indent);
	if(ast == null) {return ind+"()";}
	String s = ind;
	s += ast.atype;
	if(ast.isLeaf) {
	    s += "["+ast.getValue()+"]";
	} else {
	    s += Constants.LPARENCHAR;	
	    int size = ast.subtrees.size();
	    if(size == 0) {s += Constants.RPARENCHAR; return s;}
	    s += " ["+size+"]";
	    s += "\n";
	    for(int i=0;i<size;i++) {
	        int lind = (indent+ppindent);
	        AST t = ast.subtrees.get(i);
	        s += prettyPrint(t,lind);
	        s += "\n";
	    }
	    s += (ind+Constants.RPARENCHAR);
	}
	return s;
    }

    static AST makeleaf(Object o) {return makeleaf(AstType.STRING,o);}
    static AST makeleaf(AstType atype, Object o)
    {
	return new LeafAST(atype).setValue(o);
    }
}

class LeafAST extends AST
{
    Object value; 
    public LeafAST() {};
    // cause subtree access to fail
    public LeafAST(AstType atype, Pos pos) {this(atype); setPos(pos);}
    public LeafAST(AstType atype)
	{this.atype=atype; subtrees=null; isLeaf=true;}

    public Object getValue() {return value;}
    public AST setValue(Object v) {value=v; return this;}
    public String text() {return (value==null?null:value.toString());}

    public String toString()
    {
	String s = "[";
	s += atype;
	s += (pos==null?"":" "+pos);
	s += (" "+getValue());
	s+= "]";
	return s;
    }
}
