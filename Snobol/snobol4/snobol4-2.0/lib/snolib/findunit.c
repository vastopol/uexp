/* $Id: findunit.c,v 1.5 2003/04/21 18:31:40 phil Exp $ */

/*
 * LOAD("IO_FINDUNIT()INTEGER")
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>			/* for lib.h */

#include "h.h"
#include "snotypes.h"
#include "macros.h"
#include "load.h"
#include "equ.h"
#include "lib.h"

int
IO_FINDUNIT( LA_ALIST ) LA_DCL
{
    int i;

    i = io_findunit();			/* from lib/io.c */
    if (i < 0) {
	RETFAIL;
    }
    RETINT( i );
}
