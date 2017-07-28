package jsnobol3;

class EvalStack
{
    VM vm;
    Object[] stack;
    int sp; // stack next
    int sb; // current base of stack

    public EvalStack(VM vm)
    {
	this.vm = vm;
	stack = new Object[Constants.DEFAULTSTACKSIZE];
	sp = 0;
	sb = 0;		
    }

    void push(Object o)
    {
	if(sp == stack.length) {
	    Object[] newstack = new Object[stack.length*2];
	    System.arraycopy(stack,0,newstack,0,stack.length);
	    stack = newstack;
	}
	stack[sp++] = o;
    }
    Object pop() throws Failure
    {
	if(sp == sb) throw new Failure(vm,"Eval Stack pop(): underflow");
	return stack[--sp];
    }

    Object top() throws Failure
    {
	if(sp == sb) throw new Failure(vm,"Eval Stack top(): underflow");
	return stack[sp-1];
    }
 
    Object nth(int i) throws Failure // nth from top (=0)
    {
	if(((sp - sb) - i) <= 0)
	    throw new Failure(vm,"Eval Stack nth(): underflow");
	return stack[sp - (i+1)];
    }
 
    Object ith(int i) throws Failure // ith from bottom (=0), ignore sb
    {
	if(sp <= i) throw new Failure(vm,"Eval Stack ith(): overflow");
	return stack[i];
    }
 
   int depth() {return sp;}
   int getmark() { return sb; }    
   int mark() { sb = sp; return sb; }    
   void unmark(int i) { sb = i; }
}
