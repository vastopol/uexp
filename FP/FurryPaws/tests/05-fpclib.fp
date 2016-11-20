% 05-fpclib.fp - use fpc as library


"fpclib.fp"


main = exit.[~0, y].show.[fetch.[~`start`, x.toplevel:parse.[~"examples/hello.fp", []]], y]
