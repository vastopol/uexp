#!/bin/sh
#
# pack.sh - create distribution archive

set -e

date=`date +%Y%m%d`
tarname="fp-$date"

# create "version.fp" file
rm -fr tmpdist
mkdir -p tmpdist/$tarname

# create configuration file
echo "% version.fp - GENERATED" >tmpdist/$tarname/configuration.fp
echo "configuration = {" >>tmpdist/$tarname/configuration.fp
echo "  version = ~\"$date\"" >>tmpdist/$tarname/configuration.fp
echo "}" >>tmpdist/$tarname/configuration.fp

# copy files listed in "manifest"
for x in `cat manifest`; do
  mkdir -p tmpdist/$tarname/`dirname $x`
  cp -v $x tmpdist/$tarname/$x
done

# copy other files
echo "creating fpcboot.c ..."
./fpc -prelude prelude.fp -heap 100m fpc.fp -o tmpdist/fpc.c
echo "#define STANDALONE" >tmpdist/$tarname/fpcboot.c
echo "#define DEFAULT_HEAP_SIZE 100000000" >>tmpdist/$tarname/fpcboot.c
cat fp/fp.h fp/gc.c fp/rt.c fp/ht.c fp/lib.c tmpdist/fpc.c >>tmpdist/$tarname/fpcboot.c

# archive
rm -f $tarname.zip
cd tmpdist
zip -r ../$tarname.zip $tarname
cd ..
rm -fr tmpdist/$tarname
cd tmpdist
unzip ../$tarname.zip
cd $tarname
NODEBUG=1 sh bootstrap.sh
cd ../..
ls -l $tarname.zip
