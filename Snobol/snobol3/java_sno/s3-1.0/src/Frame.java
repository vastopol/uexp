package jsnobol3;

import java.util.HashMap;
import static jsnobol3.Constants.NOADDRESS;

class Frame
{
    // The following items constitute (most of) the state
    // to be saved on a function invocation
    int fp=0; // framepointer
    int rp=NOADDRESS; // return address
    int stp=NOADDRESS;  // stack top at entry to stmt
    int endstmt=NOADDRESS; // pointer to stmt terminator operator
    boolean infailure=false; // to prevent s/f processing in a branch
    int line=-1; // current source line
    DefFunction fcn = null;
    MatchState state = null;
    HashMap<String,Var> locals = null;
    public Frame() {}

    public String toString()
    {
	return "Frame("+trace()+")";
    }

    public String trace()
    {
	String s = "";
	if(line >= 0) s += (" line="+line);
	if(fp != NOADDRESS) s += (" fp="+fp);
	if(rp != NOADDRESS) s += (" rp="+rp);
	if(stp != NOADDRESS) s += (" stp="+stp);
	if(endstmt != NOADDRESS) s += (" endstmt="+endstmt);
	if(infailure) s += (" infailure=true");
	return s.trim();
    }

}
