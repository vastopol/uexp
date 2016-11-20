% fpc.fp - FP->C compiler - main driver 


"fpclib.fp"

%XXX doesn't use I/O state in the moment

main = toplevel:options.x +
       (id -> toplevel:run; toplevel:usage)
