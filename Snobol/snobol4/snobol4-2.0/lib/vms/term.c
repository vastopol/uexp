/* $Id: term.c,v 1.7 2003/04/21 23:52:43 phil Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>

#include "h.h"
#include "snotypes.h"
#include "lib.h"

/*
 * return a stdio stream for TERMINAL input variable
 * VMS version.
 */

FILE *
term_input() {
    return fopen("SYS$COMMAND:", "r");
}
