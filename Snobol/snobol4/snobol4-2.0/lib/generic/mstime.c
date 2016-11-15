/* $Id: mstime.c,v 1.6 2003/04/21 22:57:39 phil Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <sys/types.h>
#include <sys/times.h>			/* appeared post-v7?! */
#include <stdio.h>

#include "h.h"
#include "snotypes.h"
#include "lib.h"

/*
 * times() is venerable (it was in v6)!!
 */

#ifndef HZ
/* HZ define is likely to be in some sys include file,
 * but this is supposed to be the ultra-generic version!!!
 */
#define HZ 60				/* sigh */
#endif /* HZ not defined */

real_t
mstime() {
    struct tms tms;

    times(&tms);
    return(tms.tms_utime*1000.0/HZ);	/* just user time? */
}
