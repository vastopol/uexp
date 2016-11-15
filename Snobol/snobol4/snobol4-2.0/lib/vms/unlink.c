/*
 * $Id: unlink.c,v 1.1 1998/02/13 07:38:51 phil Exp $
 * Needed under VAXC v3
 */

int
unlink(fname)
    char *fname;
{
    return delete(fname);
}
