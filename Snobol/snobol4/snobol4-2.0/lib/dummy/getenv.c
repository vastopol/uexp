/* $Id: getenv.c,v 1.1 1996/10/05 19:09:31 phil Exp $ */

/* dummy getenv() function for snolib/host.c */

char *
getenv( str )
    char *str;
{
    return (char *)0;			/* NULL */
}
