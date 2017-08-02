                    The Snocone Programming Language

Snocone is an experimental language preprocessor created by Andrew Koenig
of AT&T Bell Laboratories.  Snocone provides a "C-like" preprocessor to the
SNOBOL4 programming language.

YOU MUST READ AND ACCEPT THE TERMS SPECIFIED IN THE FILE LICENSE.TXT PRIOR
TO OBTAINING OR USING THE SNOCONE SOFTWARE.  PLEASE DO SO NOW.

Koenig's technical report on Snocone is provided in the doc sub-directory
as file report.txt or report.doc.

The Snocone processor translates a Snocone program into a SNOBOL4 program
suitable for execution with either Catspaw's SNOBOL4+ or Macro SPITBOL.

Three executables are provided:

snocnt95.exe is a 32-bit version that runs in a command window under
Windows NT or Windows 95.  It is a 32-bit native application that does
not require a DOS Extender.

snoc386.exe is a 32-bit, MSDOS-Extended translator suitable for 80386 and
higher computers running MS-DOS or Windows 3.1.

snoc8088.exe is a 16-bit MSDOS translator that will run on all Intel
machines from 8088 and up.  It's translation capabilities may be limited
by available memory.

Whichever translator you use, or if you you build one for Unix systems using
the source code provided, the remainder of this note assumes you have
renamed the executable "sc"  (or sc.exe).

To translate the test program toposort.sc to SPITBOL program toposort.spt,
type

sc toposort

To compile and run the program with the sample data set:

spitbol toposort <toposort.dat

To translate the test program toposort.sc to SNOBOL4+ program toposort.sno,
type

sc -+ toposort

(The -+ option says "translate for SNOBOL4+").

To compile and run the program using SNOBOL4+ with the sample data set:

snobol4 toposort <toposort.dat


Snocone is being distributed by Catspaw, Inc. as an unsupported, licensed
program by special arrangement with AT&T Bell Laboratories.
Neither Andrew Koenig, AT&T Bell Laboratories, or Catspaw, Inc. will
be responsible for the correctness of the generated code.

Although there is no formal support for Snocone, Catspaw, Inc. would like
to hear of any problems or successes you have with it.

                        Catspaw, Inc.
                        P.O. Box 1123
			Salida, CO 81201 USA
			719-539-3884 (Voice)
                        719-539-4830 (Fax)
                        support@snobol4.com


Files provided:
+---\                   Overview
|    readme.txt           This document
|    license.txt          License agreement for your use of Snocone.
|
+---source              Sample programs:
|       hello.sc          Trivial "Hello World" Snocone program
|       sum.sc            Simple program to sum numbers
|       toposort.dat      Data input for toposort.sc
|       toposort.sc       Topological sorting program
|                       Source for scocone translator:
|       makefile          Makefile to build Snocone
        make8088.bat      MS-DOS batch file to build 8088 Snocone
|       snocone.sno       Snocone written in SNOBOL4 to bootstrap system
|       snocone.sc        Snocone written in Snocone
|
+---doc                 Documentation:
|       report.txt        ASCII text of report, 80 character lines
|       report.doc        ASCII text of report, each paragraph as one line
|       report.ps         Postscript format of report
|       report.htm        HTML format of report
|       report.pdf        Adobe Acrobat format of report
|
+---lib                 Utility procedures:
|       args.h            Set up argc, argv, and getenv like C program
|       foreach.h         Call procedure for each array or table element
|       pwparse.h         Parse UNIX id, password, etc.
|
+---execs               Executable binaries of Snocone translator
        snoc8088.exe      MS-DOS 8088 snocone translator
        snocnt95.exe      Windows NT/95 command window translator
        snoc386.exe       MS-DOS 80386 32-bit snocone translator
        sc.bat            MS-DOS batch file to translate and compile Snocone source

If you compile snocone for any other platforms, please upload the executable
to ftp://ftp.snobol4.com/incoming and send a mail message to
support@snobol4.com.  I will add it to the executables files directory.

Mark Emmer
Catspaw, Inc.
