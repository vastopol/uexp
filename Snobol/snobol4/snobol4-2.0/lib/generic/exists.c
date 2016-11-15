/* $Id: exists.c,v 1.4 2012/02/09 23:08:44 phil Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H defined */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "h.h"
#include "snotypes.h"
#include "lib.h"

int
exists(path)
    char *path;
{
    struct stat st;

    return stat(path, &st) >= 0;
}

int
isdir(path)
    char *path;
{
    struct stat st;

    return stat(path, &st) >= 0 && S_ISDIR(st.st_mode);
}

int
abspath(path)
    char *path;
{
#ifdef ABSPATH
    return ABSPATH(path);
#else
    return *path == '/';
#endif
}
