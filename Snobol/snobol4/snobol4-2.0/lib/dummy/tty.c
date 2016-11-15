/* $Id: tty.c,v 1.7 2003/05/20 17:05:16 phil Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>

#include "h.h"
#include "snotypes.h"
#include "lib.h"

/*
 * tty mode, echo
 * dummy version
 */

int
fisatty(f, fname)
    FILE *f;
    char *fname;
{
    return FALSE;			/* why not? */
}

void
tty_mode( fp, cbreak, noecho, recl )
    FILE *fp;
    int cbreak, noecho, recl;
{
}

/* advisory notice */
void
tty_close(f)
    FILE *f;
{
    /* should not be called (fisatty returns FALSE) */
}

void
tty_suspend()
{
}
