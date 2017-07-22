#!/bin/sh
#
# test.sh - Run test programs

set -e

# run tests
rm -f check.log
for x in `ls tests/0*-*.fp`; do
    echo "-- `basename $x .fp` --"
    if ./fpc -g -I. -prelude prelude.fp -DCAREFUL -c $x -o a.out -debug >$x.log; then
	if ./a.out >>check.log 2>&1; then
	    : 
	else
	    tail -n30 check.log; exit 1
	fi
    else
	tail -n30 $x.log
	exit 1
    fi
done

# run benchmark
echo "sqrt benchmark ..."
./fpc -c -prelude prelude.fp -I. -O3 -fomit-frame-pointer tests/sqrt-bench.fp
tests/sqrt-bench -:d

# self-test of compiler
echo "compiler self-test ..."
./fpc -c -I. -g -DCAREFUL -:h50m -heap 50m fpc.fp -o fpc1.exe -debug >debug.log
./fpc1.exe -c examples/hello.fp
./examples/hello
./fpc1.exe -I. -g -DCAREFUL -:h50m -heap 50m fpc.fp -o fpc2.c

echo "comparing output of stage 1 and 2 compilers ..."
if cmp fpc1.c fpc2.c; then
  echo "looks good"
else
  echo "output of fpc1 and fpc differs"
  exit 1
fi
