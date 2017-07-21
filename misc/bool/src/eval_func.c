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
/* eval_func.c:
 *
 * this contains boolean expression evaluation routines.
 */

#include "bool.h"

extern FUNC_LIST  *f_list;
extern TOKEN_LIST *t_list;
extern int        line;
/*
 * this function evaluates a boolean expression tree
 */

int eval_exp(b)
BOOL_EXP *b;
{ int e1, e2;

  if (b->opcode > TOKEN) {
    e1= eval_exp(b->b1);
    e2= eval_exp(b->b2);
    if (b->n1)
      e1= !e1;
    if (b->n2)
      e2= !e2;
  }
  switch(b->opcode) {
    case UNDEF : /* single expressions have undefined opcodes */
      if (b->n1)
        return(!eval_exp(b->b1));
      else
        return(eval_exp(b->b1));
    case TRUE :
      return(1);
    case FALSE :
      return(0);
    case TOKEN :
      return(token_val(b->value));
    case OR :
      return(e1 || e2);
    case AND :
      return(e1 && e2);
    case XOR :
      return((e1 || e2) && (!(e1 && e2)));
  }
}

/*
 * this routine evaluates a function for all possible values of all tokens
 */

void eval_func(t)
TOKEN_LIST   *t;
{ FUNC_LIST  *f;
  TOKEN_LIST *u;
  int        l1,l2;

  if (t == NULL) { /* end of list so do evaluations */

/*
 * print out values of each token
 */

    u= t_list;
    while (u) {
      l1= l2= strlen(u->token)/2;
      if (!(strlen(u->token) % 2))
        l1--;
      printf("%*.*s%d%*.*s ",l1,l1,"",u->value,l2,l2,"");
      u= u->next;
    }
    printf("| ");

/*
 * print out values of each tree
 */

    f= f_list;
    while(f) {
      l1= l2= strlen(f->func)/2;
      if (!(strlen(f->func) % 2))
        l1--;
      printf("%*.*s%d%*.*s ",l1,l1,"",eval_exp(f->bool_exp),l2,l2,"");
      f= f->next;
    }
    printf("\n");
    return;
  }

/*
 * this section handles value setting to evaluate for all possible
 * token values.
 */

  t->value= 0;
  eval_func(t->next);
  t->value= 1;
  eval_func(t->next);
}
