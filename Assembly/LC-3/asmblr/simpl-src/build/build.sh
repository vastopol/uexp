#!/bin/bash

# This file is the 'action' part of the build script.
# Here, different action types are added.
#
# Each package (bin/ directory) has its own tool for compilation in this
# directory too.
#
# In addition, build variables are loaded via the buildvar.sh.

source build/buildvar.sh

PF="($2)"

function checkresult() {
    if [ "$?" != "0" ]; then
        rm -rf $TARGET
        exit 1
    fi
}

function loadpackage() {
    if [ `basename $PACKAGEARG` = "DONE" ]; then
        PACKAGEARG=`dirname $PACKAGEARG`
    fi
    PACKAGE=`basename $PACKAGEARG`
    source build/$PACKAGE
    TARGET="bin/$PACKAGE"
    FILESLINE=""
    for E in $FILES; do
        FILESLINE="$FILESLINE $E"
    done
    HEADERSLINE=""
    for E in $HEADERS; do
        HEADERSLINE="$HEADERSLINE $E"
    done
}

if [ "$1" = "makefile" ]; then
    WARNING="# WARNING: THIS IS AUTO-GENERATED -- MODIFY config/Makefile.in INSTEAD"
    if [ "$2" = "create" ]; then
        rm -f Makefile Makefile.auto
        source build/packages
        echo "$PF- Creating Makefile"
        for E in $PACKAGES; do
            echo "$PF-- Configuring package $E..."
            build/build.sh makefile compile $E
        done
        echo "$PF-- Assembling Makefile..."
        cat >Makefile <<__EOF__
$WARNING
ALLEXEC=$3
default: \$(ALLEXEC)

__EOF__
        cat Makefile.auto >>Makefile
        rm -f Makefile.auto
        cat build/Makefile.in >>Makefile
        echo "$PF-- Cleaning targets..."
        make clean
    fi
    if [ "$2" = "compile" ]; then
        PACKAGEARG="$3"
        loadpackage
        DEPS="$HEADERSLINE"
        for PKG in $INCPACKAGES; do
            PACKAGEARG="$PKG"
            loadpackage
            DEPS="$DEPS $HEADERSLINE"
        done
        PACKAGEARG="$3"
        loadpackage
        echo "$WARNING" >>Makefile.auto
        echo "SOURCES_$PACKAGE=$FILESLINE" >>Makefile.auto
        echo "DEPS_$PACKAGE=$DEPS" >>Makefile.auto
        echo "HEADERS_$PACKAGE=$HEADERSLINE" >>Makefile.auto
        echo "FILES_$PACKAGE=\$(SOURCES_$PACKAGE) \$(HEADERS_$PACKAGE)" >>Makefile.auto
        echo "ALL_SOURCES+=\$(FILES_$PACKAGE)" >>Makefile.auto
        echo "">>Makefile.auto
        echo "$TARGET/DONE: \$(SOURCES_$PACKAGE) \$(DEPS_$PACKAGE)" >> Makefile.auto
        echo "	$0 compile \$@" >> Makefile.auto
        echo "" >>Makefile.auto
        echo "$TARGET: $TARGET/DONE" >> Makefile.auto
        echo "	@touch $TARGET" >>Makefile.auto
        echo "" >>Makefile.auto
    fi
fi

if [ "$1" = "compile" ]; then
    function prepdir() {
        if [ -e "$TARGET" ]; then
            rm -rf $TARGET
        fi
        mkdir $TARGET
    }

    function find_dir() {
        touch $TARGET/DONE
        if [ -n "$LINKFLAGS" ]; then
            echo $LINKFLAGS >$TARGET/linkflags
        fi
    }
    
    PACKAGEARG="$2"
    loadpackage
    prepdir
    for E in $FILES; do
        echo "$PF Working on $E"
        FILE=$E
        $COMPILECMD
        checkresult
    done
    LINKFLAGS="$LINK"
    find_dir
fi

if [ "$1" = "link" ]; then
    TARGET=$2
    #echo "$PF Linking: $@"
    
    SOURCES=""
    LINKFLAGS=""
    MISC=""
    
    for E in $3 $4 $5 $6 $7 $8 $9; do
        if [ -e "$E" ]; then
            BN=`basename $E`
            if [ "$BN" = "DONE" ]; then
                E=`dirname $E`
            fi
            if [ -d "$E" ]; then
                SOURCES="$SOURCES $E/*.o"
                if [ -e "$E/linkflags" ]; then
                    LINKFLAGS="$LINKFLAGS `cat $E/linkflags`"
                fi
            else
                SOURCES="$SOURCES $E"
            fi
        else
            MISC="$MISC $E"
        fi
    done
    
    echo "$PF-- Target: $2  |  Extra flags: '$MISC'"
    #echo "$PF-- Sources: $SOURCES"
    #echo "$PF-- Link flags: $LINKFLAGS"
    
    $CXX -o $2 $SOURCES $LINKFLAGS $MISC $COMPFLAGS
    exit $?
fi

if [ "$1" = "install" ]; then
    INSTFILES="$2"
    BINFILES="$3"
    INSTDIR="$DEST"
    
    FAIL=0
    
cat <<}

  Copying these files to $INSTDIR:
     $INSTFILES
}
    
    cp $INSTFILES $INSTDIR || FAIL=1
    
cat <<}

  Creating links to these binaries in $BINDIR:
     $BINFILES
}

    if [ -n "$BINDIR" ]; then
        if [ ! -e "$BINDIR" ]; then
            mkdir $BINDIR || FAIL=1
        fi
        for E in $BINFILES; do
            BN=`basename $E`
            if [ ! -e "$BINDIR/$BN" ]; then
                ln -s $INSTDIR/$BN $BINDIR/$BN || FAIL=1
            fi
        done
    fi

    if [ "$FAIL" != "0" ]; then
        echo "Failed!"
        exit 1
    fi
fi

if [ "$1" = "qtmacapp" ]; then
	# This target is Mac only.
	echo "$PF Making Mac app file"
	
	APPDIR=$2
	EXEC=$3
	MISCFILES="$4 $5 $6 $7 $8 $9"
	
	rm -rf $APPDIR
	mkdir $APPDIR
	mkdir $APPDIR/Contents
	mkdir $APPDIR/Contents/Frameworks
	mkdir $APPDIR/Contents/MacOS
	
	for E in $MISCFILES; do
		if [ "$E" != "$EXEC" ]; then
			echo "$PF-- Copying $E"
			cp $E $APPDIR
		fi
	done
	
	echo "$PF-- Now making stuff"
	
	APPNAME=`basename $EXEC`
	
	echo "APPL????" > $APPDIR/Contents/PkgInfo
	echo '<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE plist SYSTEM "file://localhost/System/Library/DTDs/PropertyList.dtd"><plist version="0.9"><dict><key>CFBundleIconFile</key><string>application.icns</string><key>CFBundlePackageType</key><string>APPL</string><key>CFBundleSignature</key><string>????</string><key>CFBundleExecutable</key><string>'$APPNAME'</string></dict></plist>' >$APPDIR/Contents/Info.plist
	cp $EXEC $APPDIR/Contents/MacOS/$APPNAME
	if [ -e libqt-mt.3.dylib ]; then
		ln libqt-mt.3.dylib $APPDIR/Contents/Frameworks
	else
		cp ${QTLIBDIR}/libqt-mt.3.dylib $APPDIR/Contents/Frameworks
	fi
	install_name_tool -id @executable_path/../Frameworks/libqt-mt.3.dylib \
		$APPDIR/Contents/Frameworks/libqt-mt.3.dylib
	install_name_tool -change libqt-mt.3.dylib \
		@executable_path/../Frameworks/libqt-mt.3.dylib \
		$APPDIR/Contents/MacOS/$APPNAME
fi

if [ "$1" = "qtwinapp" ]; then
	echo "$PF Making zip file with all relevant Qt libraries"
	APPDIR=$2
	ZIPNAME=$3
	MISCFILES="$4 $5 $6 $7 $8 $9"
	
	rm -rf $APPDIR
	mkdir $APPDIR
	THELIBS="Qt3Support4 QtCore4 QtGui4 QtNetwork4 QtOpenGL4 QtSql4 QtXml4"
	THELIBDIR=${QTLIB:2};
	for E in $THELIBS; do
		echo "$PF-- Copying $E.dll from $THELIBDIR/../bin"
		cp $THELIBDIR/../bin/$E.dll $APPDIR
	done
	echo "$PF-- Copying mingwm10.dll from $MINGW_ROOT/bin"
	cp $MINGW_ROOT/bin/mingwm10.dll $APPDIR
	
	for E in $MISCFILES; do
		echo "$PF-- Copying $E from here"
		cp $E $APPDIR
	done
	
	rm -f $ZIPNAME
	zip -9 $ZIPNAME $APPDIR/*
fi

if [ "$1" = "texpdf" ]; then
	DIRNAME=`dirname "$2"`
	cd "$DIRNAME"
	BNAME=`basename "$2" .pdf`
	rm -f $BNAME.aux
	echo "$PF Making tex..."
	latex $BNAME.tex
	echo "$PF Running tex again to fix references"
	latex $BNAME.tex
	echo "$PF Converting to postscript..."
	dvips $BNAME.dvi -o $BNAME.ps
	echo "$PF Converting to pdf..."
	ps2pdf $BNAME.ps
	echo "$PF Done."
fi


