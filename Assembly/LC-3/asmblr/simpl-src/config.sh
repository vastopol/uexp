#!/bin/bash

source build/targets

VERSION="2.1.1b2"

DEFAULT_DEST="/usr/local/simplc"
DEFAULT_BINDIR="/usr/local/bin"


if [ "$1" != "clean" ]; then
    if [ -e ".options" ]; then
        if [ -n "$DEST" ]; then
            DTEMP="$DEST"
        fi
        source .options
        LOADED="1"
        if [ -n "$DTEMP" ]; then
            DEST="$DTEMP"
        fi
    fi
fi

cat <<__EOF__
Configuring SimpLC version $VERSION.

All options are stored in a file called ".options"

__EOF__

if [ -n "$LOADED" ]; then
  cat <<__EOF__
  If you want to start fresh and remove the previously detected options,
  ctrl-C this right now and run:

   $0 clean
__EOF__
fi

echo
echo "-------------------------------------------------------------------"
echo


while [ -z "$INSTALL" ]; do
cat <<__EOF__
Are you going to install the simulator for other users? (y/n)
[Answering 'no' is much simpler.]

__EOF__
    read INSTALL

    case "$INSTALL" in
        [yY]*)
            INSTALL="y"
            echo "Okay, installing."
            ;;
        *)
            INSTALL="n"
            echo "Okay, NOT installing."
            ;;
    esac
done

if [ "$INSTALL" = "y" ]; then

    while [ -z "$DEST" ]; do
cat <<__EOF__
Directory to install the *simulator* to.

This is where all relevant files will go.

If this is your own computer, $DEFAULT_DEST would be a good choice.

If you are fine with the above directory, or you will just run simplc from
this location without installing it, just press enter.

   (Installation is optional - instead, you can run it immediately by
    running the simp or simpl executable directly from this directory.)

Installation directory:

__EOF__
        read DEST
        if [ "$DEST" = "" ]; then
            DEST="$DEFAULT_DEST"
            echo "Installing to $DEST"
        else
            if [ ! -d "$DEST" ]; then
                BN=`basename $DEST`
                if [ ! -d "$BN" ]; then
                    DEST=""
                    echo " Not Found!  Please create the directory first."
                fi
            fi
        fi
    done

    echo
    echo "-------------------------------------------------------------------"
    echo


    while [ -z "$BINDIR" ]; do
cat <<__EOF__
Specify the directory to install the *binaries* to.

If this is your own computer, $DEFAULT_BINDIR would be a good choice.

  (If you are not installing, just hit enter.  If you do not want to
  install links to a separate binary directory, specify the same
  directory as for installation.)

__EOF__
        read BINDIR
        if [ "$BINDIR" = "" ]; then
            BINDIR="$DEFAULT_BINDIR"
            echo "Installing to /usr/local/bin"
        else
            SAVE=`pwd`
            cd $BINDIR
            if [ "$?" != "0" ]; then
                echo "Can't find '$BINDIR' - try again.  (You must create it)"
                BINDIR=""
            else
                BINDIR=`pwd`
            fi
            cd $SAVE
        fi
    done
else
    DEST="$DEFAULT_DEST"
    BINDIR="$DEFAULT_BINDIR"
fi

echo
echo "-------------------------------------------------------------------"
echo

echo "Looking for Qt to make a GUI...."

if [ -z "$QTINC" ]; then
    O=`echo /usr/include/qt3* /usr/qt/3*/include /usr/lib/qt3*/include`
    for E in $O; do
        if [ -d "$E" ]; then
            QTINC="-I$E"
        fi
    done
fi

if [ -z "$QTLIB" ]; then
    O=`echo /usr/qt/3*/lib /usr/lib/qt3*/lib`
    for E in $O; do
        if [ -d "$E" ]; then
            QTLIB="-L$E"
        fi
    done
fi

if [ -z "$MOC" ]; then
    O=`echo /usr/bin/moc* /usr/qt/3*/bin/moc /usr/qt3*/bin/moc`
    for E in $O; do
        if [ -x "$E" ]; then
            MOC="$E"
        fi
    done
fi

TARGETS="$TARGTEXT"

if [ -z "$QTINC" ]; then
    echo "Did not detect Qt includes; not using a GUI."
else
    TARGETS="$TARGQT $TARGETS"
    
    if [ -n "$NO_KDE" ]; then
        USE_KDE="undef"
    fi

    if [ -z "$KDEINC" ]; then
        O=`echo /usr/kde/*/include /usr/include/kde`
        for E in $O; do
            if [ -d "$E" ]; then
                KDEINC="-I$E"
            fi
        done
    fi

    if [ -z "$KDELIB" ]; then
        O=`echo /usr/kde/*/lib /usr/include/kde`
        for E in $O; do
            if [ -d "$E" ]; then
                KDELIB="-L$E"
            fi
        done
    fi

    if [ -z "$USE_KDE" ]; then
        if [ -n "$KDEINC" ]; then
            echo
            echo "I found KDE includes on your system.  Would you like to"
            echo "use some basic KDE integration?  This will use the same"
            echo "network-transparent file dialogs that KDE uses, but other"
            echo "than that all functionality remains the same."
            echo
            echo "If you are not using KDE right now, then KDE integration"
            echo "will dramatically slow down the simulator's startup"
            echo "time. (To tell if you are using KDE, look for a K"
            echo "logo with gears in the lower left corner, same place as"
            echo "the Start icon.)"
            echo
            echo " KDE integration? (y/n) "
            read USE_KDE
            if [ "$USE_KDE" = "y" ]; then
                echo "Using KDE."
                USE_KDE="define"
            else
                echo "Not using KDE."
                USE_KDE="undef"
            fi
        else
            echo "Did not find any KDE includes, not enabling KDE integration."
            echo "(This is fine - no functionality is affected.)"
            USE_KDE="undef"
        fi
    fi
fi

echo
echo "-------------------------------------------------------------------"
echo

if [ "$USE_KDE" = "undef" ]; then
    KDEINC=
    KDELIB=
    LKDE=""
else
    USE_KDE="define"
    echo "Using KDE"
    

    LKDE="-lkdeui -lkio"
fi

echo "Installing to $DEST, binaries $BINDIR"

MF_EXCEPTIONS=-fno-exceptions

ME=`whoami`
TMP=/tmp/simcfg-$me
SRC=$TMP-test.c
EXEC=$TMP-test

rm -f config.h

echo "Checking for size of types..."

cat >$SRC <<__EOF__
int printf(const char *format, ...);
#define DOTYPE(t, n) printf("#define SIZEOF_%s %d\n", #n, sizeof(t))
int main(void) {
    DOTYPE(int,INT);DOTYPE(short,SHORT);DOTYPE(long,LONG);DOTYPE(void*,PTR);
    return 0;
}
__EOF__

gcc $SRC -o $EXEC
$EXEC >>config.h

echo "Done."

echo "Looking for readline()..."

cat >$SRC <<__EOF__
void add_history(const char *history);
char *readline(const char *prompt);
void rl_insert(int, int);
void rl_bind_key(int key, void (*fn)(int, int));
int main(void) {
    add_history("hello");
    (void)readline("");
    rl_bind_key('\t', rl_insert);
}
__EOF__

rm -f $EXEC
gcc $SRC -lreadline -lncurses -o $EXEC

if [ "$?" = "0" ]; then
    NO_READLINE="undef"
    MF_READLINE="-lreadline"
    MF_CURSES="-lncurses"
    echo "Found!"
else
    rm -f $EXEC
    gcc $SRC -lreadline5 -lncurses -o $EXEC
    
    if [ "$?" = "0" ]; then
        NO_READLINE="undef"
        MF_READLINE="-lreadline5"
        MF_CURSES="-lncurses"
        echo "Found (-lreadline5)!"
    else
        NO_READLINE="define"
        MF_READLINE=""
        MF_CURSES=""
        echo "No readline... using ad-hoc wannabe readline"
    fi
fi

echo "#$NO_READLINE NO_READLINE" >>config.h

echo "#$USE_KDE USE_KDE" >>config.h

echo "#define INSTALLPATH \"$DEST\"" >>config.h
echo "#define VERSION_STRING \"$VERSION\"" >>config.h

cat >.options <<__EOF__
READLINE="$MF_READLINE"
CURSES="$MF_CURSES"
EXCEPTIONS="$MF_EXCEPTIONS"
INSTALL="$INSTALL"
DEST="$DEST"
BINDIR="$BINDIR"
QTLIB="$QTLIB"
QTLIBDIR="${QTLIB:2}"
QTINC="$QTINC"
QTINCDIR="${QTINC:2}"
KDELIB="$KDELIB"
KDELIBDIR="${KDELIB:2}"
KDEINC="$KDEINC"
KDEINCDIR="${KDEINC:2}"
MOC="$MOC"
USE_KDE="$USE_KDE"

__EOF__

cat <<__EOF__

All options selected are now in .options.  If you need to change anything
you may edit the file.

Options selected (in .options):
__EOF__

cat -n .options

echo
echo

rm -f $TMP*

cat >simpl.desktop <<__EOF__
[Desktop Entry]
Comment=LC-3 Simulator
Comment[en_US]=LC-3 Simulator
Encoding=UTF-8
Exec=$DEST/simpl
GenericName=Simpl
GenericName[en_US]=Simpl
Icon=$DEST/hi32-app-simpl.png
MimeType=
Name=Simpl
Name[en_US]=Simpl
Path=
StartupNotify=true
Terminal=false
TerminalOptions=
Type=Application
__EOF__

build/build.sh makefile create "$TARGETS"

cat <<__EOF__



Configuration is finished.  Now you may compile the simulator by typing:

  make

If anything is broken for you, please email garryb@gmail.com with "simplc" in
the email header.

After compilation, the following will be created:
  $TARGETS

__EOF__

if [ "$INSTALL" = "y" ]; then
cat <<__EOF__
Optionally, to install to $DEST:
  make install   # Make sure you have enough privileges first

__EOF__
fi
