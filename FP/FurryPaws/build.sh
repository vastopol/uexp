#!/bin/sh
#
# usage: build.sh

uname=`uname`
exe=""

case "$uname" in
    MINGW*) exe=".exe";;
esac

if test \! -e fpc0$exe; then
    if test \! -e fpc$exe; then
	cp fpcboot$exe fpc0$exe
    else
	cp fpc$exe fpc0$exe
    fi
fi

set -x
./fpc0 -c fpc.fp -DCAREFUL -g -I. -prelude prelude.fp -heap 100m -o fpc$exe
