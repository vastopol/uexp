#!/bin/sh
# bootstrap.sh - perform a 3-stage bootstrap build of "fpc"

set -e

heapsize=50m
debug="-debug"

if test -n "$NODEBUG"; then
    debug=""
fi

case "`uname -m`" in
    *64*) heapsize=100m;;
esac

set -x

rm -f fpc1 fpc2 fpcboot *.exe

gcc fpcboot.c -g -I. -o fpcboot

CC=gcc ./fpcboot -c -g -prelude prelude.fp -DCAREFUL -:h$heapsize fpc.fp -o fpc1 -I.
./fpc1 -c -g -prelude prelude.fp -DCAREFUL -:h$heapsize fpc.fp -o fpc2 -I.
./fpc2 -c -prelude prelude.fp -DNDEBUG -:h$heapsize -heap $heapsize fpc.fp \
  -O3 -fomit-frame-pointer -o fpc -I. -debug

set +x

echo "comparing output of stage 1 and 2 compilers ..."

if cmp fpc2.c fpc.c; then
  echo "looks good"
else
  echo "output of fpc2 and fpc differs"
  exit 1
fi
