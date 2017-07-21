# (c) copyright 1987 jim frost
# all rights reserved
#
# this program is copyrighted material.  the author gives permission
# to duplicate and redistribute this program provided the following
# conditions are met:
#   - this copyright notice is not removed.
#   - all duplicate copies or distributions contain full source
#     and documentation, including copyright notices.
#   - duplicate copies or distributions outside a single site are
#     original distributions without modifications.  (this is to keep
#     bastardized versions from showing up all over thie place.)
#
# this program source may be modified provided the following
# conditions are met:
#   - modified source is not distributed to other sites.
#   - modifications (including, but not limited to, bug fixes) are
#     sent to the author if the modifications are to be distributed.
#     no modified source is to be distributed unless done so by the
#     author.
#
# no warranty, either express or implied, is given for this program.
# the author makes no guarantees of fitness for any use of this
# program.  the author is not responsible for damages resulting from
# the use of this program for any purpose.
#
# 'site' refers to one or more computers under a single management.
# 'author' refers to the copyright holder, jim frost.
# 'source' refers to all files related to this program.
# 'documentation' refers to non-compilable files in the distribution.
#
# basically this notice is to keep me out of trouble should anything
# go wrong (i really *do* test these things though) and to make sure
# that the distribution of code is centralized.  makes bug fixes and
# enhancements much easier.
#
# thank you for your attention to this copyright notice.  if everyone
# follows this, you may find this a useful tool that is pretty well
# supported.
#
# author information:
#   jim frost                    permanent usnail address:
#   madd@bucsb.bu.edu            75 washington street
#   ..!harvard!bu-cs!bucsb!madd  laconia, nh  03246.
#
CFLAGS= -O

all:		bool

bool:		alloc.o bool.o build.o eval_file.o eval_func.o misc.o token.o
		cc $(CFLAGS) -o bool alloc.o bool.o build.o eval_file.o eval_func.o misc.o token.o

clean:
		rm -f *.o

*.c.o:
		cc -c $(CFLAGS) $*

alloc.o:	bool.h alloc.c
bool.o:		bool.h bool.c
build.o:	bool.h build.c
eval_file.o:	bool.h eval_file.c
eval_func.o:	bool.h eval_func.c
misc.o:		bool.h misc.c
token.o:	bool.h token.c
