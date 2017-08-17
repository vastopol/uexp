/* $Id: term.c,v 1.1 2003/06/17 01:29:33 phil Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>

#include "h.h"
#include "snotypes.h"
#include "lib.h"

/*
 * return a stdio stream for TERMINAL input variable
 * Win32 version.
 */

FILE *
term_input() {
    return fopen("CONIN$", "r");
}
