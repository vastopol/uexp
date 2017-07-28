package jsnobol3;

import tools.Pos;

class Token
{
    TokenType ttype = TokenType.NULL;
    String text = null;
    Pos pos = new Pos().set(0,0);

    public Token() {};
    public Token(TokenType kind) {this(); ttype=kind;};
    public Pos getPos() {return pos;}
    public void setPos(Pos p) {pos.set(p);}
    public void setPos(Token other) {pos.set(other.pos);}
    public String toString()
    {
	return ttype+"("+pos+":"
		+" text=|"+text+"|"
		+")";
    }
}