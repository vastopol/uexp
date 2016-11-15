/* $Id: lexcmp.c,v 1.7 2003/04/21 23:20:08 phil Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>

#include "h.h"
#include "snotypes.h"
#include "macros.h"
#include "lib.h"

int
lexcmp(a,b)
    struct spec *a, *b;
{
    char *x, *y;
    int i, j;

    i = S_L(a);
    j = S_L(b);
    x = S_SP(a);
    y = S_SP(b);

    while (i > 0 && j > 0) {
	if (*x != *y)
	    return *x - *y;
	i--;
	j--;
	x++;
	y++;
    }
    return i - j;
}
