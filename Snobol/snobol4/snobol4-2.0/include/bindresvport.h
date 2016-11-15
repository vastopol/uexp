/* $Id: bindresvport.h,v 1.2 2003/07/04 20:01:09 phil Exp $ */

#ifdef BINDRESVPORT_IN_RPC_H		/* FreeBSD, NetBSD, but NOT OpenBSD! */
#include <rpc/rpc.h>
#endif /* BINDRESVPORT_IN_RPC_H defined */

/* from bindresvport.c */
/* should be in lib.h, but would require socket.h! */

#ifdef NEED_BINDRESVPORT
extern int bindresvport __P((int, struct sockaddr_in *));
#endif /* NEED_BINDRESVPORT defined */
#ifdef NEED_BINDRESVPORT_SA
extern int bindresvport_sa __P((int, struct sockaddr *));
#endif /* NEED_BINDRESVPORT_SA defined */
