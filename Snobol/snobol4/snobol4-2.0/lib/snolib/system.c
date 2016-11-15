/* $Id: system.c,v 1.5 2003/07/04 20:01:09 phil Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#ifdef HAVE_STDLIB_H
#include <stdlib.h>			/* system() */
#endif /* HAVE_STDLIB_H defined */

#include "h.h"
#include "equ.h"
#include "snotypes.h"
#include "macros.h"
#include "load.h"

/*
 * LOAD("SYSTEM(STRING)INTEGER")
 *
 * Usage;	SYSTEM("shell command")
 * Returns;	exit status
 */

SYSTEM( LA_ALIST ) LA_DCL
{
    char cmd[256];			/* XXX */

    getstring( LA_PTR(0), cmd, sizeof(cmd) );
    RETINT( system(cmd) );
}
