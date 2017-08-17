/* $Id: popen.c,v 1.2 2001/12/03 01:14:25 phil Exp $ */
 
/*
 * dummy popen()/pclose()
 * September 24, 1997
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>

FILE *
popen(file, mode)
    char *file, *mode;
{
    return NULL;
}

int
pclose(f)
    FILE *f;
{
    return -1;
}
