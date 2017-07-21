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
/* eval_file.c:
 *
 * this function calls each of the necessary routines to build and
 * evaluate a tree.
 */

#include "bool.h"

int err,
    paren,
    max_token,
    print_err;

extern int debug_mode,
           no_print;

static char *line= "----------------------------------------------------";

TOKEN_LIST *t_list;
FUNC_LIST  *f_list;

/*
 * this function processes each boolean equation
 */

void eval_file(f)
FILE *f;
{ FUNC_LIST  *fn;
  TOKEN_LIST *t;
  char       func[MAXTOKEN],
             c;
  int        a;

  paren= 0;
  max_token= 0;
  t_list= NULL;
  f_list= NULL;

/*
 * loop through file to build all functions
 */

  func[0]= '\0';
  c= ' ';
  while (!feof(f)) {
    if (blank(c)) {
      cget(f,c);
    }
    else if (begtok(c)) {
      a= 0;
      do {
        func[a++]= c;
        cget(f,c);
      } while(intok(c));
      func[a]= '\0';
    }

/*
 * when we hit an '=', start building a function
 */

    else if ((c == '=') && strlen(func)) {
      fn= new_func(func);
      fn->bool_exp= build(f);

/*
 * if we got a syntax error, explain that we're printing the function
 * as evaluated up to the error.  otherwise, print out the header and
 * do the tree evaluation.
 */

      if (err) {
        if (debug_mode) {
          printf("Parsed function up to error:\n");
          print_err= 0;
          printf("%s= ",func);
          print_tree(fn->bool_exp);
          printf("\n");
        }
        exit(1);
      }
      if (debug_mode) {
        printf("\n\nFunction parsed as:\n");
        printf("%s= ",func);
        print_tree(fn->bool_exp);
        printf(";\n");
      }
      func[0]= '\0';    /* reset for next run */
      cget(f,c);
    }
    else switch (c) {
      case '[' :
        do
          cget(f,c);
        while ((!feof(f)) && (c != ']'));
        cget(f,c);
        break;
      case '{' :
        do
          cget(f,c);
        while ((!feof(f)) && (c != '}'));
        cget(f,c);
        break;
      case '=' :
        printf("\nMissing equation declaration.\n");
        exit(1);
      case EOF :
        printf("\nUnexpected end of file.\n");
        exit(1);
      default :
        printf("\nIllegal character (possibly missing equation definition).\n");
        exit(1);
    }
  }
  printf("\n");
  t= t_list;
  while (t) {
    printf("%s ",t->token);
    t= t->next;
  }
  printf("| ");
  fn= f_list;
  while (fn) {
    printf("%s ",fn->func);
    fn= fn->next;
  }
  printf("\n");
  t= t_list;
  while (t) {
    printf("%*.*s",strlen(t->token)+1,strlen(t->token)+1,line);
    t= t->next;
  }
  printf("+-");
  fn= f_list;
  while (fn) {
    printf("%*.*s",strlen(fn->func)+1,strlen(fn->func)+1,line);
    fn= fn->next;
  }
  printf("\n");
  eval_func(t_list); /* dump output */

/*
 * because bool can be run on a lot of files each execution, we have
 * to free up memory allocated for each file.  while we're at it, we
 * print out lists of stuff used in the file if we're in debug mode.
 * might be useful to somebody.
 */

  if (debug_mode) {
    printf("\n\nFunctions Defined\n");
    printf("-----------------\n");
  }
  while (f_list) {
    if (debug_mode)
      printf("%s\n",f_list->func);
    fn= f_list;
    f_list= f_list->next;
    free_tree(fn->bool_exp);
    free((char *)fn);
  }
  if (debug_mode) {
    printf("\n\nTokens Defined\n");
    printf("--------------\n");
  }
  while (t_list) {
    if (debug_mode)
      printf("%s\n",t_list->token);
    t= t_list;
    t_list= t_list->next;
    free((char *)t);
  }
}
