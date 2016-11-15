/* $Id: tan.c,v 1.8 2014/12/15 20:58:11 phil Exp $ */

/*
 * SPARC SPITBOL compatibility;
 * LOAD("TAN(REAL)REAL")
 * tangent of angle in radians
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <math.h>

#include "h.h"
#include "snotypes.h"
#include "macros.h"
#include "load.h"
#include "equ.h"

int
TAN( LA_ALIST ) LA_DCL
{
    real_t ret = tan(LA_REAL(0));
    if (!REAL_ISFINITE(ret))
	RETFAIL;
    RETREAL(ret);
}
