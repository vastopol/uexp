/* $Id: expops.c,v 1.2 1994/01/02 21:25:20 budd Exp $ */

/* dummy functions for EXPINT/EXREAL
 * (attempts to execute code using exponentiation will
 * cause fatal errors (as specified in SIL reference))
 */

int
expint(res,x,y)
    struct descr *res, *x, *y;
{
    UNDF();
}

int
exreal(res,x,y)
    struct descr *res, *x, *y;
{
    INTR10();
}
