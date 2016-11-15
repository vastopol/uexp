/* $Id: osopen.c,v 1.5 2013/08/15 05:10:33 phil Exp $ */

/*
 * Win32 open hook
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>
#include <string.h>			/* strcmp() */

#include "h.h"
#include "snotypes.h"
#include "lib.h"

int
osdep_open(fname, mode, fpp)
    const char *fname, *mode;
    FILE **fpp;
{
    if (strcmp(fname, "/dev/tty") == 0) {
	*fpp = fopen("CON", mode);
#ifdef _IONBF
	if (*fpp) {
	    // no line buffering on windoze, so turn of buffering.
	    // setbuf(*fpp, NULL) should do the same thing as this, and
	    // might be more portable, but is now deprecated.
	    setvbuf(*fpp, (char *)NULL, _IONBF, 0);
	}
#endif
	return TRUE;			/* matched */
    }
    if (strcmp(fname, "/dev/null") == 0) {
	*fpp = fopen("NUL", mode);
	return TRUE;			/* matched */
    }
    return FALSE;			/* no match */
}

