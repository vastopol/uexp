package jsnobol3;

import tools.*;
import static jsnobol3.Constants.*;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.Iterator;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.File;
import java.io.FileReader;

class Pass1
{
    ArrayList<Statement> stmts;

    public Pass1()
    {
	stmts = new ArrayList<Statement>();
    }

    public ArrayList<Statement> pass1(String fname) throws Error
    {
	File file = null;
	FileReader fr = null;
	BufferedReader r = null;
	try {
	    file = new File(fname);
	    if(!file.exists() || !file.canRead())
		throw new Error("Cannot read file: "+fname);
	    fr = new FileReader(file);
	    r = new BufferedReader(fr);
	} catch (Exception e) {
	    throw new Error(e);
	}
	try {
	    // Read in the whole program
	    String line = r.readLine();
	    while(line != null) {
		Statement m = new Statement();
		stmts.add(m);
		m.lineno = stmts.size();
		m.text = line + ((char)EOLCHAR);
		line = r.readLine();
	    }		
	} catch (IOException ioe) {
	    throw new Error(ioe);
	}
	if(stmts.size() == 0) return stmts;
	// process out comments, labels, continuations, and control words
	for(int index=0;index < stmts.size();index++) {
	    Statement m = stmts.get(index);
	    if(m == null) continue;
	    char ch = m.text.charAt(0);
	    switch (ch) {
		case CONTROLWORDCHAR:// [-]name([= ]value])? is expected
		    // remove trailing EOL and leading dash.
		    String text = m.text.substring(1,m.text.length()-1);
		    // capture name and value
		    String name;
		    if(text.contains(" =")) {
			int ieq = Math.min(text.indexOf("="),
					    text.indexOf(" "));
			name=text.substring(0,ieq);
			String value=text.substring(ieq+1,text.length());
			if(name.length() == 0)
			    Debug.Println("Pass1: illegal option: "+text+"; ignored");
		        Main.parms.put(name,value);
		    } else {// name only control word
			name=text.substring(0,text.length());
			if(name.startsWith("no")) {
			    name = name.substring(2);
			    Main.parms.remove(name);
			} else {
			    Main.parms.put(name,Boolean.TRUE);
			}
		    }
		    // after processing, treat line like a comment; fall thru
		case EOLCHAR: // empty line; treat like comment
		case COMMENTCHAR:
		    stmts.set(index,null);
		    break;
		case CONTINUECHAR:
		    if(index == 0)
			throw new Error(new Pos().set(index+1)+"; Program starts with continuation line");
		    Statement m0 = stmts.get(index-1);
		    m0.text = (m0.text + m.text.substring(1));
		    stmts.set(index,null);
		    break;
		default: 
		    if(Lexer.isWS(ch)) { // pass thru
		    } else if(Lexer.isLabelChar1(ch)) {
			// collect and store the label
			int w = 1;
		        while(Lexer.isLabelChar2(m.text.charAt(w))) w++;
			m.label = m.text.substring(0,w);
			// Check for duplicates
			Label l = Snobol3.labels.get(m.label);
			if(l == null) {
			    l = new Label(m.label,index+1);
			    Snobol3.labels.put(m.label,l);
			} else
			    throw new Error(new Pos().set(index+1)+"; Multiply defined label: "+m.label+"; previous at line "+(l.lineno));
		    } else {
			// some kind of unexpected character
			throw new Error(new Pos().set(index+1)+"; Unexpected character in column 1: '"+ch+"'");
		    }
	    }
	}
if(false) {
for(int i=0;i<stmts.size();i++)
System.out.println(i+": "+stmts.get(i));
System.out.println("----------");
System.out.println("|stmts|="+stmts.size());
for(Iterator it=Snobol3.labels.keySet().iterator();it.hasNext();) {
String key = (String)it.next();
Label l = Snobol3.labels.get(key);
Statement stmt = stmts.get(l.lineno - 1);
System.out.println(key+"-> "+l+": "+stmt);
}
}
	return stmts;
    }
}
