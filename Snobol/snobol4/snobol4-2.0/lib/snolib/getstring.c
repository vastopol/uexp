/* $Id: getstring.c,v 1.11 2004/12/14 19:59:22 phil Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#ifdef HAVE_STDLIB_H			/* before stdio, h.h */
#include <stdlib.h>			/* for malloc */
#else  /* HAVE_STDLIB_H not defined */
extern void *malloc();
#endif /* HAVE_STDLIB_H not defined */

#include "str.h"			/* before h.h to get system __P */
#include "h.h"
#include "equ.h"			/* BCDFLD, etc */
#include "snotypes.h"			/* DESCR, etc */
#include "macros.h"			/* D_A() etc */
#include "load.h"			/* prototypes */

EXPORT(void)
getstring( vp, dp, len )
    const void *vp;			/* pointer to "natural variable" */
    char *dp;
    int len;
{
    int dlen;
    char *sp;

    if (!vp) {				/* null string? */
	*dp = '\0';
	return;
    }
    dlen = D_V(vp);			/* get length from title */
    sp = (char *) vp + BCDFLD;		/* get pointer to string */

    if (dlen > len-1)
	dlen = len-1;

    bcopy( sp, dp, dlen );
    dp[dlen] = '\0';
}

/* perform malloc, getstring */
EXPORT(char *)
mgetstring( vp )
    const void *vp;			/* pointer to "natural variable" */
{
    char *cp;
    int len;

    if (vp)
	len = D_V(vp);
    else
	len = 0;

    len++;
    cp = malloc(len);
    if (!cp)
	return NULL;

    getstring(vp, cp, len);

    return cp;
}
