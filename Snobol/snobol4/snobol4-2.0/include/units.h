/* $Id: units.h,v 1.5 2005/05/27 05:46:02 phil Exp $ */

/* NOTE: must be .GE. 1 and .LE. 99!! */
#define UNITI	5		/* INPUT: stdin */
#define UNITO	6		/* OUTPUT: stdout */
#define UNITP	7		/* TERMINAL output: stderr */
#define UNITT	8		/* TERMINAL input */

/* names associated with UNITI, UNITO, UNITP(!), UNITT */
#define STDIN_NAME  "stdin"		/* XXX "-" ? */
#define STDOUT_NAME "stdout"		/* XXX "-" ? */
#define STDERR_NAME "stderr"
#define TERMIN_NAME "termin"		/* terminal input */
