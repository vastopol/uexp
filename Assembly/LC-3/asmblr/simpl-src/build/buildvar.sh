source .options

if [ ! -d 'bin' ]; then
	mkdir bin
fi

if [ -z "$OPT" ]; then
    OPT="-g"
fi

if [ -z "$CC" ]; then
    CC=gcc
    CXX=g++
fi
CFLAGS="${OPT} -Wall -pedantic -Werror -I. ${COMPFLAGS} -Wparentheses"
CXXWARN="-Wall -Wold-style-cast -Woverloaded-virtual ${EXCEPTIONS} -Wparentheses"
CXXFLAGS="${OPT} ${CXXWARN} -I. ${COMPFLAGS}"

if [ "$USE_KDE" = "undef" ]; then
    KDELIB_ALL=""
else
    KDELIB_ALL="$KDELIB -lkdeui -lkio"
fi
QTWARN="-ansi -Wall -Wpointer-arith -Wwrite-strings -Wcast-align -Wconversion -Wchar-subscripts -Wformat-security"
QTDEF="-DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG"
QTINC="$QTINC $KDEINC"
QTCOMPAT="-fno-exceptions -fno-check-new -fno-common"
QTFLAGS="$OPT $QTCOMPAT $QTINC $QTDEF $QTWARN -I. ${COMPFLAGS}"
if [ -z "$QTMT" ]; then
    QTMT="-lqt-mt"
fi
QTALLLIB="$QTLIB $QTMT $KDELIB_ALL"
QTLINK="$QTFLAGS $QTOBJ $OBJ $QTALLLIB $TEXTLIB $COMPFLAGS"

function cxxcompilefile() {
        case "$FILE" in
          *.c)
	    $CC -c $CFLAGS $FILE -o $TARGET/`basename $FILE .c`.o
            ;;
          *.cpp)
            $CXX -c $CXXFLAGS $FILE -o $TARGET/`basename $FILE .cpp`.o
            ;;
          *)
            echo "Unknown file type:" $FILE
        esac
        checkresult
}

function qtcompilefile() {
	TN=`dirname $FILE`/`basename $FILE .cpp`
	$MOC $TN.h -o $TN.moc
        $CXX -c $QTFLAGS $TN.cpp -o $TARGET/`basename $TN`.o
        checkresult
        rm -f $TN.moc
}
