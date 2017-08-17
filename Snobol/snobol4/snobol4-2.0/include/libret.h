/* $Id: libret.h,v 1.6 2003/04/21 19:44:50 phil Exp $ */

/* return values for library routines */

enum io_read_ret {
    IO_OK,
    IO_EOF,
    IO_ERR
};

enum io_include_ret {
    INC_FAIL,
    INC_SKIP,
    INC_OK
};

enum io_read_ret io_read __P((struct descr *,struct spec *sp));
enum io_include_ret io_include __P((struct descr *,struct spec *));
