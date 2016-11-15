/* $Id: dynamic.c,v 1.5 2010/12/04 06:14:10 phil Exp $ */

/* allocate dynamic region */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#ifdef HAVE_STDLIB_H			/* before stdio */
#include <stdlib.h>			/* for malloc */
#else  /* HAVE_STDLIB_H not defined */
extern void *malloc();
#endif /* HAVE_STDLIB_H not defined */

char *
dynamic( size )
    size_t size;
{
    return malloc(size);
}

void
vm_gc_advise(gc)
    int gc;
{
}
