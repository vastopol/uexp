/* $Id: inet.c,v 1.5 2003/07/06 04:13:32 phil Exp $ */

/* dummy inet interface */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <stdio.h>
#include "h.h"				/* TRUE */

FILE *
tcp_open( host, service, port, priv )
    char *host, *service;
    int port, priv;
{
    return NULL;
}


FILE *
udp_open( host, service, port, priv )
    char *host, *service;
    int port, priv;
{
    return NULL;
}

void
inet_cleanup() {
}

int
inet_close( f )
    FILE *f;
{
    return TRUE;
}
