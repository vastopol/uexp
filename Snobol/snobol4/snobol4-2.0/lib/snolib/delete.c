/* $Id: delete.c,v 1.7 2014/10/17 22:00:50 phil Exp $ */

/*
 * SITBOL compatibility;
 * LOAD("DELETE(STRING)STRING")
 *
 * Usage;	DELETE("filename")
 * Returns;	null string or failure
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#ifdef HAVE_UNISTD_H
#include <unistd.h>			/* unlink() */
#endif /* HAVE_UNISTD_H defined */

#ifdef HAVE_STDLIB_H
#include <stdlib.h>			/* for free() */
#endif

#include "h.h"
#include "equ.h"
#include "snotypes.h"
#include "macros.h"
#include "load.h"

int
DELETE( LA_ALIST ) LA_DCL
{
    char *path;
    int ret;

    path = mgetstring(LA_PTR(0));
    ret = unlink(path);
    free(path);
    if (ret < 0)
	RETFAIL;
    RETNULL;
}
