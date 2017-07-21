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
/* alloc.c:
 *
 * this file contains memory allocation and freeing routines
 */

#include "bool.h"

extern FUNC_LIST  *f_list;
extern TOKEN_LIST *t_list;
extern int        max_token,
                  err;

FUNC_LIST *new_func(name)
char *name;
{ FUNC_LIST *f,*g;

  if ((f= (FUNC_LIST *)malloc(sizeof(FUNC_LIST))) == NULL) {
    perror("\nMemory allocation error");
    exit(1);
  }
  if (!f_list)
    f_list= f;
  else {        /* append to list and check for duplicates */
    if (!strcmp(name,f_list->func)) {
      printf("\nDuplicate function name.\n");
      exit(1);
    }
    g= f_list;
    while (g->next) {
      if (!strcmp(name,g->func)) {
        printf("\nDuplicate function name.\n");
        exit(1);
      }
      g= g->next;
    }
    g->next= f;
  }
  strcpy(f->func,name);
  f->bool_exp= NULL;
  f->next= NULL;
  return(f);
}

/*
 * this function creates a new token
 */

TOKEN_LIST *new_token(s)
char *s;
{ TOKEN_LIST *t;

  if ((t= (TOKEN_LIST *)malloc(sizeof(TOKEN_LIST))) == NULL) {
    perror("\nMemory allocation error");
    exit(1);
  }
  strcpy(t->token,s);
  t->number= max_token++;
  t->next= NULL;
  return(t);
}

/*
 * this function creates a new BOOL_EXP node
 */

BOOL_EXP *newbnode()
{ BOOL_EXP *t;

    if ((t= (BOOL_EXP *)malloc(sizeof(BOOL_EXP))) == NULL) {
      err= 1;
      perror("\nMemory allocation error");
      return(NULL);
    }
    t->opcode= UNDEF;
    t->value= UNDEF;
    t->b1=
    t->b2= NULL;
    t->n1=
    t->n2= 0;
    return(t);
}

/*
 * this function free()s up a boolean tree.
 */

void free_tree(b)
BOOL_EXP *b;
{
  if (!b)
    return;
  free_tree(b->b1);
  free_tree(b->b2);
  free((char *)b);
}
