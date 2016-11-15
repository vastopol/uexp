#!/bin/sh
# $Id: sdb.sh,v 1.5 2013/09/26 02:57:32 phil Exp $

SDB=<SNOLIB_LIB>/sdb.sno
SNOBOL4=<BINDIR>/snobol4<VERS>

# handle user args??

# create listing file, and pass filename in environment so sdb.sno can read it.
SDB_LISTFILE=/tmp/sdb$$
export SDB_LISTFILE

# remove listing file on exit (if not already removed by sdb)
trap "rm -f $SDB_LISTFILE" 0

$SNOBOL4 -b -l $SDB_LISTFILE -L $SDB "$@"
