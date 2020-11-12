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
/* misc.c:
 *
 * any miscellaneous functions that i didn't think belonged anywhere
 * else.
 */

#include "bool.h"

extern int print_err;

int blank(c)
char c;
{
  return((c == ' ') || (c == '\011') || (c == '\n') || (c == '\r'));
}

/*
 * this function prints out the tree.  if the tree is incomplete, it
 * just stops.
 */

void print_tree(b)
BOOL_EXP *b;
{ if (b == NULL) {
    if (!print_err) {
      printf("?");
      print_err= 1;
    }
    return;
  }
  switch (b->opcode) {
    case FALSE :
      printf("0");
      break;
    case TRUE :
      printf("1");
      break;
    case TOKEN :
      print_token(b->value);
      break;
    case UNDEF :    /* all of these have a char stored in b-value */
      if ((b->b1) && (!b->b2)) {
        if (b->n1)
          printf("!");
        print_tree(b->b1);
      }
      else
        print_err= 1;
      break;
    case OR :
    case AND :
    case XOR :
      printf("(");
      if (b->n1)                     /* left not */
        printf("%c",b->n1);
      print_tree(b->b1);             /* left tree */
      printf(" %c ",(char)b->value); /* operator */
      if (b->n2)                     /* right not */
        printf("%c",b->n2);
      print_tree(b->b2);             /* right tree */
      if (b->b2)                     /* only print if not syntax err */
        printf(")");
      break;
    default :
      panic("print_tree");
  }
}


/*
 * we got a real serious error
 */

void panic(s)
char *s;
{
  printf("\n\nPANIC!!  Internal program error in %s\n",s);
  printf("This error should not have happened.  Please contact you distributer\n");
  printf("and keep a sample of the data that caused this crash.\n");
  exit(1);
}

