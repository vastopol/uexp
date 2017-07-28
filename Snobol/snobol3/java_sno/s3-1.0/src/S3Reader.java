/*
 * @(#)StringReader.java	1.24 04/02/19
 *
 * Copyright 2004 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package jsnobol3;

import tools.*;
/**
 * A character stream whose source is an sequence of strings
 *
 * @version 	1.24, 04/02/19
 * @author	Mark Reinhold
 * @since	JDK1.1
 */

import java.util.ArrayList;
import java.io.Reader;
import java.io.IOException;

public class S3Reader extends tools.StringBufferReader
{
    static final int EOF = -1;
    static final int EOLCHAR = '\n';

    int nextline = 0;
    ArrayList<Statement> stmts = null;

    public S3Reader() {super();}

    /**
     * Read a single character.
     *
     * @return     The character read, or -1 if the end of the stream has been
     *             reached
     *
     * @exception  IOException  If an I/O error occurs
     */
    public int read() throws IOException
    {
	int ch;
	for(;;) {
	    ch = super.read();
	    if(ch != -1) break;
	    if(!readline()) {
		return EOF;
	    }
	}
	return ch;	
    }

    boolean readline()
    {
	Statement stmt;
	do {
	    if(nextline >= stmts.size()) return false; // EOF
	    stmt = stmts.get(nextline);
	    nextline++;
	} while(stmt == null);
	super.open(stmt.text);
//System.err.println("line=|"+stmt+"|");
	super.setLine(stmt.lineno);
	return true;
    }

    public void open(ArrayList<Statement> stmts)
    {
	this.stmts = stmts;
	nextline = 0;
    }
}
