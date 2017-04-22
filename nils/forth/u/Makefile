PREFIX=/u
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/man
CFLAGS=	-O -g
LIBS=	-lcurses

# WOPS    enables the "w" (windowing/curses) operator
# COLOR   enables color support in "w" (req. WOPS)
# USLEEP  enables the "wu" operator (else it's a nop)
# ATEXIT  signals that we have atexit()
# STRDUP  signals that we have strdup()
# SANDBOX strips directories from paths in f{a,i,o,t,x}

# also set these options in "uc" and "ugo"

OPTS=	-DWOPS -DCOLOR -DUSLEEP -DATEXIT -DSTRDUP

all:	u uc1 u.1
	true

u:	u.c w.c uclib ucwlib
	$(CC) $(CFLAGS) $(OPTS) -o u u.c $(LIBS)

uc1:	u uc1.u
	./u uc1.u <uc1.u >uc1.c
	$(CC) -o uc1 uc1.c
	rm -f uc1.c

u.1:	u.txt
	echo '.nf' >u.1
	echo '.ec $$' >>u.1
	sed -e 's/\$$/ /' <u.txt >>u.1

ue:	prog/ue.u
	(cd prog; uc -w ue.u) && mv prog/ue .

test: u
	./u uc1.u <uc1.u >1.c && cc -o 1 1.c
	./1 <uc1.u >2.c && diff 1.c 2.c && rm -f 1 [12].c

install: all
	chmod +x uc ugo
	cp u uc1 uc ugo $(BINDIR)
	cp ulib uclib ucwlib $(BINDIR)
	if [ -f ue ]; then cp ue $(BINDIR); else true; fi
	if [ -f ue ]; then cp prog/ue.hlp $(BINDIR); else true; fi
	cp u.1 $(MANDIR)/man1

clean:
	rm -f u uc1 ue u.1 prog.shar u.shar u.shar.Z \
		a.out *.o *.core core

csums:
	csum -u <_sums >_nsums && mv -f _nsums _sums

sums: clean
	find . -type f -print | grep -v _ | grep -v '^$$' | csum >_sums

arc:	clean
	(cd prog; u shar.u -d prog * >../prog.shar)
	u prog/shar.u -d u `ls | grep -v prog` prog.shar >u.shar
	rm -f prog.shar
	compress u.shar
