/* $Id: cos.c,v 1.8 2005/12/11 16:55:54 phil Exp $ */

/*
 * SPARC SPITBOL compatibility;
 * LOAD("COS(REAL)REAL")
 * cosine of angle in radians
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
COS( LA_ALIST ) LA_DCL
{
    RETREAL( cos( LA_REAL(0) ) );
}
