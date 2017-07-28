package tools;

public class CharStreamSequence implements CharStream
{
    protected class Push{int ch; Push next;}
    protected Push pushed;
    protected CharSequence seq;
    protected int index;

    public CharStreamSequence() {reset();}
    public CharStreamSequence(CharSequence seq) {open(seq);}

    public void pushback(int ch)
    {
	Push p = new Push();
	p.ch = ch;
	p.next = pushed;
	pushed = p;	
    }

    protected boolean pushed() {return pushed != null;}
    protected int unpush()
    {
	int ch = 0;
	if(pushed != null) {
	    ch = pushed.ch;
	    pushed = pushed.next;
	}
	return ch;
    }

    protected int nextch()
    {
	if(index == seq.length()) return -1; // EOF
	return seq.charAt(index++);
    }

    public CharStream open(CharSequence seq)
    {
	reset();
	this.seq = seq;
	index = 0;
	return this;
    }

    // Allow for extraction of the string and the current cursor
    public CharSequence getSequence() {return seq;}
    public int getCursor() {return index;}

    // CharStream interface
    public int getch() {return pushed()?unpush():nextch();}
    public int peek() {int ch = getch(); pushback(ch); return ch;}
    protected  void reset() {pushed = null;}

}; //CharStream
