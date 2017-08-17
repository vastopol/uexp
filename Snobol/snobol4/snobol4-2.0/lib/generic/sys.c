/* $Id: sys.c,v 1.6 2003/06/06 16:11:10 phil Exp $ */

/* generic support for HOST() on systems with no uname(2) */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>

#include "h.h"
#include "snotypes.h"
#include "lib.h"
#include "str.h"

void
hwname(cp)
    char *cp;
{
    strcpy(cp, HWNAME);
}

void
osname(cp)
    char *cp;
{
    strcpy(cp, OSNAME);
}
