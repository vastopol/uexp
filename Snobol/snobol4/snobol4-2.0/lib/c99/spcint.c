/* $Id: spcint.c,v 1.7 2004/02/03 05:38:08 phil Exp $ */

/*
 * convert from strings to integer using strtoll (in ISO C99)
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdlib.h>			/* strtoll() */
#include <stdio.h>
#include <errno.h>

#include "h.h"
#include "snotypes.h"
#include "macros.h"
#include "lib.h"
#include "str.h"

#include "equ.h"			/* for I and R */
#include "res.h"
#include "data.h"

#ifdef NO_STATIC_VARS
#include "vars.h"
#endif /* NO_STATIC_VARS defined */

int
spcint(dp, sp)
    struct descr *dp;
    struct spec *sp;
{
    char buffer[64];			/* ??? */
    int_t len;
    char *cp;
    int_t temp;

    len = S_L(sp);
    cp = S_SP(sp);

    if (D_A(SPITCL)) {			/* SPITBOL features? */
	/* strip leading whitespace */
	while (len > 0 && (*cp == ' ' || *cp == '\t')) {
	    cp++;
	    len--;
	}
    }

    if (len > sizeof(buffer)-1)
	len = sizeof(buffer)-1;
    bcopy( cp, buffer, (long)len );	/* XXX SIZE_T */
    buffer[len] = '\0';

    errno = 0;
    temp = strtoll( buffer, &cp, 10);	/* always decimal */
    if (*cp)
	return FALSE;			/* failure */
#ifdef ERANGE
    if (errno == ERANGE)
	return FALSE;
#endif /* ERANGE defined */

    D_A(dp) = temp;
    D_F(dp) = 0;			/* clear flags */
    D_V(dp) = I;			/* set type */

    return TRUE;			/* success */
}
