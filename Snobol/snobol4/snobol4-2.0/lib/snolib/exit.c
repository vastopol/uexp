/* $Id: exit.c,v 1.8 2014/10/17 22:00:50 phil Exp $ */

/*
 * SPARC SPITBOL compatibility;
 * LOAD("EXIT()")
 *
 * Usage;	EXIT("command")
 * Returns;	fails, or passes execution to command string
 *
 * partial simulation of SPITBOL EXIT()
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>			/* for lib.h */

#ifdef HAVE_STDLIB_H
#include <stdlib.h>			/* for free() */
#endif

#include "h.h"
#include "snotypes.h"
#include "macros.h"

#include "load.h"			/* LA_xxx macros */
#include "equ.h"			/* datatypes I/S */
#include "lib.h"			/* io_flushall(),execute() */

int
EXIT( LA_ALIST ) LA_DCL
{
    char *str;

    if (LA_TYPE(0) == S) {		/* EXIT("command") */
	str = mgetstring(LA_PTR(0));
	io_flushall(0);			/* flush output buffers */
	execute(str);			/* should not return */
	/* ~sigh~ */
	free(str);
	RETFAIL;
    }

    /* save files not supported */
    RETFAIL;
}
