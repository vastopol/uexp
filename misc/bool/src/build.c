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
/* build.c:
 *
 * this reads in a file from a pointer and builds an evaluation tree.
 */

#include "bool.h"

extern int err,   /* syntax error detected */
           paren, /* parenthesis depth counter */
           no_print;

/*
 * this function checks a BOOL_EXP for syntax errors and sets "err"
 * accordingly.
 */

static void check_syn(t)
BOOL_EXP *t;
{
  switch (t->opcode) {
    case FALSE :
    case TRUE :
    case TOKEN :
      return;
    case AND :
    case OR :
    case XOR :
      if ((!t->b1) && (!t->b2)) {
        err= 1;
        printf("\nNull expression.\n");
      }
      else if (!t->b2) {
        err= 1;
        printf("\nMissing expression.\n");
      }
      return;
    case UNDEF :
      if ((t->b1) && (!(t->b2))) { /* single expression */
        check_syn(t->b1);
        return;
      }
      err= 1;
      printf("\nMissing operator.\n");
      t->value= '?';
      return;
    default :
      panic("check_syn");
  }
}

BOOL_EXP *build(f)
FILE       *f;            /* file we are getting input from */
{ int      a;
  char     c,             /* current char we are reading */
           tok[MAXTOKEN]; /* current token we are reading */
  BOOL_EXP *t,*u;         /* current tree node and child tree node */

/*
 * if eof then make SURE we return null.  this will force a synerr
 * during syntax checking above or an indicative error message from
 * top level.
 */

  if (feof(f))
    return(NULL);

/*
 * get a new BOOL_EXP node to build with
 */

  t= newbnode();

/*
 * loop through the entire file.  a syntax error will automatically
 * drop out of this loop and back up through the tree.  exiting is
 * also done depending on conditions found in the source.
 */

  c= ' ';
  while (!feof(f))  {

    if (c == ';') {
      check_syn(t);
      return(t);
    }
    if ((c == ' ') || (c == '\011') || (c == '\n') || (c == '\r')) {
      cget(f,c);
      continue;
    }
/*
 * end subexression evaluation.  do syntax error checking at current
 * node to be sure it's ok.  return status to caller.
 */

    if (c == ')') {
      if (!paren--) {
        err= 1;
        printf("\nMismatched parenthesis.\n");
      }
      else
        check_syn(t);
      return(t);
    }

/*
 * if structure is full then we have to make it a substruct.  note
 * that changing this can alter precedence determination from left
 * to right or right to left.
 */

    if (t->b2) {
      u= t;
      t= newbnode();
      t->b1= u;
    }

    if (begtok(c)) { /* found a token so evaluate it */

/*
 * loop for token name
 */

      a= 0;
      do {
        if (a < MAXTOKEN) /* truncate overlong tokens */
          tok[a]= c;
        a++;
        if (feof(f))
          break;
        cget(f,c);
      } while (intok(c));
      tok[a]= '\0';

/*
 * build node for token
 */

      u= newbnode();
      u->opcode= TOKEN;
      u->value= get_token(tok);
      if (t->b1)
        t->b2= u;
      else
        t->b1= u;
    }
    else {
      switch (c) {

/*
 * comments.  this doesn't have a possible backslash preceeding it because
 * I really don't care if you want to put special chars in your comments.
 * two different comment types are used to allow you to comment out whole
 * sections of code, which IS useful.
 */

        case '[' :
          while (c != ']') {
            if (feof(f)) {
              err= 1;
              return(t);
            }
            cget(f,c);
          }
          break;
        case '{' :
          while (c != '}') {
            if (feof(f)) {
              err= 1;
              return(t);
            }
            cget(f,c);
          }
          break;

/*
 * true or false
 */

        case '1' :
        case '0' :
          u= newbnode();
          if (t->b1)
            t->b2= u;
          else
            t->b1= u;
          if (c == '0')
            u->opcode= FALSE;
          else
            u->opcode= TRUE;
          break;
          
/*
 * subexpressions begin with a paren
 */

        case '(' :
          paren++;
          if (t->b1)
            t->b2= build(f);
          else
            t->b1= build(f);
          if (err)
            return(t);
          break;

/*
 * handle each unary operation
 */

        case '!' : /* not */
        case '~' :
        case '/' :
          if (t->b1)
            t->n2= c;
          else
            t->n1= c;
          break;

/*
 * handle each of the binary operators
 */

        case '&' : /* and */
        case '*' :
          t->opcode= AND;
          t->value= c;
          break;
        case '|' : /* or */
        case '+' :
          t->opcode= OR;
          t->value= c;
          break;
        case '^' : /* xor */
          t->opcode= XOR;
          t->value= c;
          break;
        default :
          err= 1;
          printf("\nIllegal character '%c'\n",c);
          return(t);
      }
      cget(f,c);
    }
  }
  check_syn(t); /* check for syntax errors before leaving */
  return(t);
}
