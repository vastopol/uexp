rem bootstrap.bat - perform a 3-stage bootstrap build of "fpc"

del fpc1 fpc2 fpcboot

gcc fpcboot.c -g -I. -o fpcboot

fpcboot -prelude prelude.fp -:h100m fpc.fp -o fpc1.c
gcc -g fpc1.c -I. -DCAREFUL -o fpc1
fpc1 -c -g -prelude prelude.fp -DCAREFUL -:h100m fpc.fp -o fpc2 -I.
fpc2 -c -prelude prelude.fp -DNDEBUG -:h100m -heap 100m fpc.fp -o fpc -O3 -fomit-frame-pointer -I. -debug

@echo off
echo comparing output of stage 1 and 2 compilers (should not list any differences) ...

fc fpc2.c fpc.c
