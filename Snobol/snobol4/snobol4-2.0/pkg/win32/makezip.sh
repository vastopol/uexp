#!/bin/sh
# $Id: makezip.sh,v 1.2 2013/10/03 05:34:52 phil Exp $
# cygwin shell script to create windows zip file!!
if [ ! -f version ]; then
	echo version file not found
	exit 1
fi
vers=`cat version`
rm -rf zipdir
rm -f snobol4-$vers.zip
mkdir zipdir
./pkg/win32/install.bat zipdir >/dev/null
cd zipdir
zip -r -z ../snobol4-$vers.zip SNOBOL4 < snobol4/$vers/README.win32
cd ../
rm -rf zipdir
