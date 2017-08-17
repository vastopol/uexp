/* $Id: system.c,v 1.1 1996/10/05 19:09:31 phil Exp $ */

/* dummy system() function for snolib/host.c */

int
system( str )
    char *str;
{
    return -1;
}
