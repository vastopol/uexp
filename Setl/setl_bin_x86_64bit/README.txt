The file setlbin.tgz is a gzipped tar archive of the GNU SETL
executables.  It can be unpacked like this:

  cd /usr/local/bin     # or perhaps $HOME/bin for testing
  tar xvfz setlbin.tgz  # or gzip -cd <setlbin.tgz | tar xvf -

The other files in this directory are small text files, the first two
indicating the environment that was used to compile the executables:

  bugs.txt      -  Honest Dave's epistemology
  summary.txt
  {
    gcc-info.txt  -  output of "gcc -dumpversion; gcc -dumpmachine"
    uname-a.txt   -  output of "uname -a"
    tar-tv.txt    -  output of "tar tvfz setlbin.tgz" (file list)
    cksum.txt     -  output of "cksum setlbin.tgz"
    md5sum.txt    -  output of "md5sum setlbin.tgz"
    version.txt   -  output of "setl --version"
  }

Each of the executables has a "--help" option.

Further documentation on SETL can be found at http://setl.org/.

Please contact bacon@cs.nyu.edu (David Bacon) with questions or remarks.

     dB
