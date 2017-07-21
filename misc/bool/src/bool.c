/* (c) copyright 1987 jim frost
 * all rights reserved
 *
 * this program is copyrighted material.  the author gives permission
 * to duplicate and redistribute this program provided the following
 * conditions are met:
 *   - this copyright notice is not removed.
 *   - all duplicate copies or distributions contain full source
 *     and documentation, including copyright notices.
 *   - duplicate copies or distributions outside a single site are
 *     original distributions without modifications.  (this is to keep
 *     bastardized versions from showing up all over thie place.)
 *
 * this program source may be modified provided the following
 * conditions are met:
 *   - modified source is not distributed to other sites.
 *   - modifications (including, but not limited to, bug fixes) are
 *     sent to the author if the modifications are to be distributed.
 *     no modified source is to be distributed unless done so by the
 *     author.
 *
 * no warranty, either express or implied, is given for this program.
 * the author makes no guarantees of fitness for any use of this
 * program.  the author is not responsible for damages resulting from
 * the use of this program for any purpose.
 *
 * 'site' refers to one or more computers under a single management.
 * 'author' refers to the copyright holder, jim frost.
 * 'source' refers to all files related to this program.
 * 'documentation' refers to non-compilable files in the distribution.
 *
 * basically this notice is to keep me out of trouble should anything
 * go wrong (i really *do* test these things though) and to make sure
 * that the distribution of code is centralized.  makes bug fixes and
 * enhancements much easier.
 *
 * thank you for your attention to this copyright notice.  if everyone
 * follows this, you may find this a useful tool that is pretty well
 * supported.
 *
 * author information:
 *   jim frost                    permanent usnail address:
 *   madd@bucsb.bu.edu            75 washington street
 *   ..!harvard!bu-cs!bucsb!madd  laconia, nh  03246.
 */
/* bool.c:
 *
 * this program evaluates boolean equations and builds truth tables from
 * the equations.
 *
 * history:
 *   9.29.87    original version.
 *   10.19.87   single expression (no operator) bugs fixed.  it couldn't
 *              handle a single expression.  FALSE constant bug fixed.
 *              returned TRUE by mistake.  typo.  changes made to output
 *              to make it more readable.  bug in centering function of
 *              the evaluation print routine fixed.  forgot to follow the
 *              evaluated number by a space.
 *   10.20.87   duplicate function declaration error fixed.  wouldn't
 *              find duplicate function name if the duplicate was the
 *              first in the function list.
 */

#include "bool.h"

int debug_mode,
    no_print;

main(argc,argv)
int argc;
char *argv[];
{ FILE *f;
  int a;

  debug_mode= 0; /* no debugging mode unless -D */
  no_print= 0;   /* print unless -n */

  if (argc == 1) {
    printf("Usage: %s [-DEBUG -noprint] filename [filename ...]\n",PROG);
    exit(1);
  }

/*
 * evaluate switches
 */

  for (a= 1; *argv[a] == '-'; a++) {
    if ((!strcmp(argv[a]+1,"DEBUG")) || (!strcmp(argv[a]+1,"D")))
      debug_mode= 1;
    else if ((!strcmp(argv[a]+1,"noprint")) || (!strcmp(argv[a]+1,"n")))
      no_print= 1;
  }

/*
 * evaluate for every file on command line
 */

  for (; a < argc; a++) {
    if (strcmp(argv[a]+strlen(argv[a])-2,".b"))
      printf("%s: not %s source file\n",argv[a],PROG);
    else if ((f= fopen(argv[a],"r")) != NULL) {
      eval_file(f);
      fclose(f);
    }
    else
      perror(argv[a]);
  }
}
