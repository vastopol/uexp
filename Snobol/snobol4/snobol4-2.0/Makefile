# $Id: Makefile,v 1.96 2014/12/22 00:42:45 phil Exp $

# TARGETS:
#
# snobol4	make xsnobol4 binary; regression test; link to snobol4
# xsnobol4	make xsnobol4 binary; no regression test
#
# tidy		remove turds (backup, temp files)
# cleanmostly	removes objects, turds; leave generated sources, final binary
# clean		clean as when unpacked (removes binary)
# spotless	removes snobol4 generated files (requires binary to regenerate)
#
# tar		make distribution
# uu		make uuencoded distribution

# m4 macro processor; used to generate Makefile2
# (largely to avoid depending on make '+=' operator)
M4=m4

################
# snobol commands

# bootstrapped using Catspaw SPARC SPITBOL
#SNO=spitbol -i512k -b
SNO=snobol4 -b

################
# machine generated files (final products);

GENERATED=data.c data_init.h proc.h static.h syn.h data.h callgraph \
	equ.h res.h syn.c syn_init.h snobol4.c host.sno isnobol4.c with

################
# SIL source file
SIL=	v311.sil

################
# AIX4 makes all targets, so added this;
# must be named "all" for FreeBSD "ports"?

# force GNU make to run top Makefile serially
.NOTPARALLEL: foo

# tell BSD make to use a single shell
.SINGLESHELL: foo

# files to avoid removing when interrupted
.PRECIOUS: snobol4 xsnobol4 Makefile2 $(GENERATED)

M2TARGETS=all snobol4 xsnobol4 install lint sdb cpuid timing.out build_all \
	clean_modules build_modules printenv
$(M2TARGETS): $(GENERATED) Makefile2 ALWAYS .depend
	$(MAKE) -f Makefile2 $@

# a rule depending on this target will always be run
# since ALWAYS is never created!
ALWAYS:

# for hand generation of sources
generated: $(GENERATED)

################
# run configuration script

config.m4 config.h config.sno version.h version: configure
	./configure

# ~~SIGH~~ adding "with" above would cause regeneration of source when
# configure script changed...  _could_ have configure output with.tmp
# and only rename if it has changed...  But THAT would still cause
# configure to be re-run and necessitate recompilation of all files
# even if it didn't change....
with:
	./configure

################
# make second level makefile

M2TMP=Makefile2.tmp
Makefile2 .depend: config.m4 Makefile2.m4
	echo '# DO NOT EDIT. machine generated from Makefile2.m4' > $(M2TMP)
	echo '# add local changes to local-config'		>> $(M2TMP)
	$(M4) Makefile2.m4 >> $(M2TMP)
	echo '# DO NOT DELETE THIS LINE. make depend uses it.' >> $(M2TMP)
	$(MAKE) -f $(M2TMP) depend MAKEFILE2=$(M2TMP)
	mv -f $(M2TMP) Makefile2
	rm -f .depend
	touch .depend

################
# code

# regular version
snobol4.c proc.h2 static.h2: procs genc.sno globals $(SIL) with
	rm -f snobol4.c2 proc.h2 static.h2
	$(SNO) genc.sno `cat with` $(SIL) > snobol4.c2
	mv -f snobol4.c2 snobol4.c

# generate inline version (functions reordered)

# Uses tsort (topological sort), to order routines for best inlining.
# private copy of FreeBSD version of tsort (topological sort) supplied
# since GNU version used on Linux and Cygwin can't handle cycles

# isnobol4.c can't depend on bsdtsort (which depends on config.h),
# since that would force isnobol4.c to be rebuilt (which requires a
# snobol4 binary) when starting with a distribution kit.

isnobol4.c: procs genc.sno globals $(SIL) with
	rm -rf isnobol4.c2 proc.h2 static.h2 prolog subr
	$(MAKE) bsdtsort
	mkdir subr
	$(SNO) -- genc.sno --inline `cat with` $(SIL) > prolog
	cd subr && ../bsdtsort < ../callgraph > order && \
		cat ../prolog `cat order` > ../isnobol4.c2
	mv -f isnobol4.c2 isnobol4.c
	rm -rf prolog subr

proc.h:	proc.h2
	@cmp proc.h proc.h2 || cp proc.h2 proc.h

static.h: static.h2
	@cmp static.h static.h2 || cp static.h2 static.h

bsdtsort: bsdtsort.c config.h
	$(CC) -o bsdtsort -DHAVE_CONFIG_H bsdtsort.c

################
# syntax tables

# only change syn.h if it has changed from last run!

syn.c syn.h2 syn_init.h2: syntax.tbl gensyn.sno with
	rm -f syn.c2 syn.h2
	$(SNO) gensyn.sno `cat with` syntax.tbl
	mv -f syn.c2 syn.c

syn.h:	syn.h2
	@cmp syn.h syn.h2 || cp syn.h2 syn.h

syn_init.h: syn_init.h2
	@cmp syn_init.h syn_init.h2 || cp syn_init.h2 syn_init.h

################
# resident data

data.h2 data.c2 equ.h2 data_init.h2 res.h2: $(SIL) gendata.sno with
	rm -f data.h2 data.c2 equ.h2 data_init.h2 res.h2
	$(SNO) gendata.sno `cat with` $(SIL)

data.h:	data.h2
	@cmp data.h data.h2 || cp data.h2 data.h

data.c:	data.c2
	@cmp data.c data.c2 || cp data.c2 data.c

equ.h:	equ.h2
	@cmp equ.h equ.h2 || cp equ.h2 equ.h

res.h:	res.h2
	@cmp res.h res.h2 || cp res.h2 res.h

data_init.h: data_init.h2
	@cmp data_init.h data_init.h2 || cp data_init.h2 data_init.h

#################
# generated snolib files

host.sno: host.awk lib/snolib/host.h
	awk -f host.awk lib/snolib/host.h > host.sno

#################
# dependency generation is slow and ugly (and wrong?)
# doesn't change much!!

#procs:	gendep.sno
#	$(SNO) gendep.sno < $(SIL) > procs2
#	mv -f procs2 procs

##################################################################
# housekeeping

# directly generated files (to avoid recompilation when no change in outputs)
G2=data.c2 data.h2 data_init.h2 proc.h2 static.h2 equ.h2 \
	syn.h2 syn_init.h2 res.h2

# remove turds
tidy:
	rm -rf subr
	cd test; sh clean.sh
	find . -type f -a \( \
		-name '*~' -o -name '#*' -o -name '*.tmp' -o -name '.#*' \) \
		-print | xargs -t rm -f

# Inspired by PowderMilk bisquits
# (made by Norwegian Bachelor farmers, so you know they're pure, mostly);
# remove objects, turds; leave generated sources, final binary.

DISP=*.o *.a prolog bsplitu vers build.c bsdtsort \
	config.m4 config.h Makefile2 .depend *.1 *.html

cleanmostly: tidy
	rm -f $(DISP)
	rm -rf timdir.*
	cd modules; for m in [a-z]*; do \
	    test -d $$m && (cd $$m; rm -f $$m.sno *.o *.so *.bundle *.3 *.html); \
	done

# clean as a freshly unpacked kit; remove binaries, timing;
# leave version.h, version for Windoze
clean:	cleanmostly
	(cd doc; make clean)
	rm -f snobol4 xsnobol4 cpuid timing.out tested *.ln sdb *.exe

# DANGER: requires installed binary to rebuild!!
# remove objects, generated files (clean as a fresh CVS checkout)
spotless: clean
	rm -f $(GENERATED) $(G2) snobol4.c isnobol4.c snobol4 xsnobol4
