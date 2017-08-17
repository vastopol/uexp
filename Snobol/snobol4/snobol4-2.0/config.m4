# config generated Thu Aug 17 02:27:38 GMT 2017
# on jessie (Linux)
# by $Id: configure,v 1.278 2015/01/02 04:54:27 phil Exp $
undefine([include])
ADD_CPPFLAGS([-DHAVE_CONFIG_H])
# see config.h for more defines
VERS=2.0
BINDIR=$(DESTDIR)/usr/local/bin
MANDIR=$(DESTDIR)/usr/local/share/man
MAN1DIR=$(DESTDIR)/usr/local/share/man/man1
MAN3DIR=$(DESTDIR)/usr/local/share/man/man3
MAN7DIR=$(DESTDIR)/usr/local/share/man/man7
SNOLIB=$(DESTDIR)/usr/local/lib/snobol4
SNOLIB_LIB=$(DESTDIR)/usr/local/lib/snobol4/2.0/lib
SNOLIB_DOC=$(DESTDIR)/usr/local/lib/snobol4/2.0
INCLUDE_DIR=$(DESTDIR)/usr/local/lib/snobol4/2.0/include
DOC_DIR=$(DESTDIR)/usr/local/share/doc/snobol4-2.0
EXTRA_OBJS=getstring.o retstring.o handle.o
ADD_SRCS([$(GETSTRING_C) $(RETSTRING_C) $(HANDLE_C)])
CCM=$(CC) -M
ADD_CFLAGS([-Wall])
SNOBOL4_C_CFLAGS=-Wno-return-type -Wno-switch
CC=gcc
################
# C Compiler:
# gcc -v
# Using built-in specs.
# COLLECT_GCC=gcc
# COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/4.9/lto-wrapper
# Target: x86_64-linux-gnu
# Configured with: ../src/configure -v --with-pkgversion='Debian 4.9.2-10' --with-bugurl=file:///usr/share/doc/gcc-4.9/README.Bugs --enable-languages=c,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.9 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.9 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-4.9-amd64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-4.9-amd64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-4.9-amd64 --with-arch-directory=amd64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-multiarch --with-arch-32=i586 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
# Thread model: posix
# gcc version 4.9.2 (Debian 4.9.2-10) 
################
# sizeof(int) = 4
# sizeof(void *) = 8
# sizeof(long) = 8
# sizeof(float) = 4
# sizeof(double) = 8
# alignment(double) = 8
OPT=-O3
MSTIME_C=$(SRCDIR)lib/posix2001/mstime.c
INET_O=inet6.o
ADD_OBJS([bindresvport.o])
ADD_SRCS([$(BINDRESVPORT_C)])
ADD_OBJS([popen.o])
ADD_SRCS([$(POPEN_C)])
ADD_LDFLAGS([-lutil])
TTY_C=$(SRCDIR)lib/posix/tty.c
LOAD_C=$(SRCDIR)lib/unix98/load.c
ADD_LDFLAGS([-ldl])
ADD_LDFLAGS([-rdynamic])
ADD_OBJS([loadx.o])
# NOTE: SNOLIB_FILE in config.h too!
SNOLIB_FILE=snolib.so
DYNAMIC_C=$(SRCDIR)lib/posix/dynamic.c
SLEEP_C=$(SRCDIR)posix/sleep.c
# Shared Object Libraries
SO_EXT=.so
SO_CFLAGS=-fPIC
SO_LD=gcc
SO_LDFLAGS=-shared -fPIC
# Dynamicly Loaded Extensions
DL_EXT=.so
DL_CFLAGS=-fPIC
DL_LD=gcc
DL_LDFLAGS=-shared -fPIC
INSTALL=/usr/bin/install
BLOCKS=1
RUNSETUP=SNOLIB=$(SNOLIB) SNOPATH=../..:../../snolib ../../xsnobol4 -N setup.sno

modules/dirs/dirs.so modules/dirs/dirs.sno modules/dirs/snobol4dirs.3 modules/dirs/snobol4dirs.3.html: xsnobol4 snolib/setuputil.sno modules/dirs/setup.sno
	cd modules/dirs; $(RUNSETUP) build

modules/logic/logic.so modules/logic/logic.sno modules/logic/snobol4logic.3 modules/logic/snobol4logic.3.html: xsnobol4 snolib/setuputil.sno modules/logic/setup.sno
	cd modules/logic; $(RUNSETUP) build

modules/random/random.so modules/random/random.sno modules/random/snobol4random.3 modules/random/snobol4random.3.html: xsnobol4 snolib/setuputil.sno modules/random/setup.sno
	cd modules/random; $(RUNSETUP) build

modules/stat/stat.so modules/stat/stat.sno modules/stat/snobol4stat.3 modules/stat/snobol4stat.3.html: xsnobol4 snolib/setuputil.sno modules/stat/setup.sno
	cd modules/stat; $(RUNSETUP) build

modules/time/time.so modules/time/time.sno modules/time/snobol4time.3 modules/time/snobol4time.3.html: xsnobol4 snolib/setuputil.sno modules/time/setup.sno
	cd modules/time; $(RUNSETUP) build

modules/sprintf/sprintf.so modules/sprintf/sprintf.sno modules/sprintf/snobol4sprintf.3 modules/sprintf/snobol4sprintf.3.html: xsnobol4 snolib/setuputil.sno modules/sprintf/setup.sno
	cd modules/sprintf; $(RUNSETUP) build

modules/fork/fork.so modules/fork/fork.sno modules/fork/snobol4fork.3 modules/fork/snobol4fork.3.html: xsnobol4 snolib/setuputil.sno modules/fork/setup.sno
	cd modules/fork; $(RUNSETUP) build

modules/ndbm/ndbm.so modules/ndbm/ndbm.sno modules/ndbm/snobol4ndbm.3 modules/ndbm/snobol4ndbm.3.html: xsnobol4 snolib/setuputil.sno modules/ndbm/setup.sno
	cd modules/ndbm; $(RUNSETUP) build

modules/ffi/ffi.so modules/ffi/ffi.sno modules/ffi/snobol4ffi.3 modules/ffi/snobol4ffi.3.html: xsnobol4 snolib/setuputil.sno modules/ffi/setup.sno
	cd modules/ffi; $(RUNSETUP) build

MODULES=dirs logic random stat time sprintf fork ndbm ffi
MODULES_LOADABLE= modules/dirs/dirs.so modules/logic/logic.so modules/random/random.so modules/stat/stat.so modules/time/time.so modules/sprintf/sprintf.so modules/fork/fork.so modules/ndbm/ndbm.so modules/ffi/ffi.so
MODULES_INCLUDE= modules/dirs/dirs.sno modules/logic/logic.sno modules/random/random.sno modules/stat/stat.sno modules/time/time.sno modules/sprintf/sprintf.sno modules/fork/fork.sno modules/ndbm/ndbm.sno modules/ffi/ffi.sno
MODULES_MAN= modules/dirs/snobol4dirs.3 modules/logic/snobol4logic.3 modules/random/snobol4random.3 modules/stat/snobol4stat.3 modules/time/snobol4time.3 modules/sprintf/snobol4sprintf.3 modules/fork/snobol4fork.3 modules/ndbm/snobol4ndbm.3 modules/ffi/snobol4ffi.3
MODULES_HTML= modules/dirs/snobol4dirs.3.html modules/logic/snobol4logic.3.html modules/random/snobol4random.3.html modules/stat/snobol4stat.3.html modules/time/snobol4time.3.html modules/sprintf/snobol4sprintf.3.html modules/fork/snobol4fork.3.html modules/ndbm/snobol4ndbm.3.html modules/ffi/snobol4ffi.3.html
MODULES_GENERATED=$(MODULES_LOADABLE) $(MODULES_INCLUDE)
TEST_SNOPATH=..:../modules/dirs:../modules/logic:../modules/random:../modules/stat:../modules/time:../modules/sprintf:../modules/fork:../modules/ndbm:../modules/ffi:../snolib:.

clean_modules:
	rm -f $(MODULES_GENERATED)

build_modules: $(MODULES_GENERATED)

# if the file local-config existed it would have been incorporated here
