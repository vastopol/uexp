CC=pcc
CFLAGS=-O

miniscm:	miniscm.c miniscm.h
	$(CC) $(CFLAGS) -o miniscm -static miniscm.c

miniscm.h:	miniscm.scm
	echo "/* This file was automatically generated. Do not edit! */" \
		> miniscm.h
	echo "char init_scm[] = " >> miniscm.h
	sed -e '/^;/d'	 		\
	    -e 's/ *;.*//'		\
	    -e '/^$$/d'			\
	    -e 's/\\/\\\\/g'		\
	    -e 's/"/\\"/g'		\
	    -e 's/  */ /g'		\
	    -e 's/ *\(.*\)/ "\1\\n"/' 	\
		< miniscm.scm >> miniscm.h
	echo ";" >> miniscm.h
	rm -f miniscm.tmp

test:	miniscm
	rm -f __TESTFILE__ && echo '(load "test.scm")' | ./miniscm -q

lint:
	cc -Wall -pedantic -O0 miniscm.c && rm -f a.out

csums:
	txsum -u <_checksums >_checksums.new && mv -f _checksums.new _checksums

clean:
	rm -f miniscm miniscm3.tar.gz core *.core __TESTFILE__

arc:	clean
	tar cvf miniscm3.tar *
	gzip -9 miniscm3.tar
