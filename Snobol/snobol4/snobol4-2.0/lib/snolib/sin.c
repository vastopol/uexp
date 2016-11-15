/* $Id: sin.c,v 1.6 2003/04/21 18:04:19 phil Exp $ */

/*
 * SPARC SPITBOL compatibility;
 * LOAD("SIN(REAL)REAL")
 * sine of angle in radians
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
SIN( LA_ALIST ) LA_DCL
{
    RETREAL( sin( LA_REAL(0) ) );
}
