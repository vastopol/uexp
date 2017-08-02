# MS-DOS (Intel DOS Extender) or OS/2 version.
# Remove all the ".exe" strings for Unix version.

snocone:	snocone.sno snocone.sc
#
# Use "snocone in snobol4" to compile "snocone in snocone" to sc.spt
	spitbol -b snocone.sno <snocone.sc >sc.spt
#
# Run sc.spt to produce snocone.exe, which we rename sc.exe.
	spitbol -b sc.spt
	mv snocone sc
#
# Recompile "snocone in snocone" using the first version of snocone,
# to produce snocone.spt.
	sc snocone.sc
	rm sc
#
# Compile snocone.spt to produce 2nd version of snocone to snocone.exe,
# which we rename xx.exe.
	spitbol -b snocone.spt
	mv snocone xx
#
# Now run xx.exe to perform all initialization and generate the
# final form of snocone, snocone.exe.
	xx
	rm xx

test:	sc.spt
	spitbol -b sc.spt
	mv snocone sc
	sc snocone.sc
	rm sc
	spitbol -b snocone.spt
	mv snocone xx
	xx
	mv snocone yy
	yy snocone.sc
	spitbol -b snocone.spt
	fc /b snocone xx

clean:
	rm sc.spt
	rm sc
	rm snocone.spt
	rm xx
	rm yy
