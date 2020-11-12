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
/* token.c:
 *
 * this file contains token creation, insertion, print, and value funcs.
 */

#include "bool.h"

extern TOKEN_LIST *t_list;

/*
 * these functions return a boolean value dependent on whether or
 * not a char is a legal char inside a token.
 */

int begtok(c)
char c;
{
  return(((c >='@') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) ||
         (c == '_'));
}

int intok(c)
char c;
{
  return(begtok(c) || ((c >='0') && (c <='9')));
}

/*
 * this looks through the token list and returns token values.  if no
 * token exists, it is added.
 */

int get_token(s)
char *s;
{ TOKEN_LIST *t, *u;

  if (!t_list)
    return((t_list= new_token(s))->number); /* null list so make one */

/*
 * try to find token in list
 */

  t= t_list;
  for (;;) {                 /* we should always return() out of this */
    if (!strcmp(t->token,s)) /* found it so return its number */
      return(t->number);
    else if (t->next)        /* if not end of list, keep looking */
      t= t->next;
    else {                   /* end of list so make new token */
      if (strcmp(s,t_list->token) < 0) {
        t= new_token(s);
        t->next= t_list;
        t_list= t;
        return(t_list->number);
      }
      else {
        for (t= t_list; (t->next) && (strcmp(s,t->next->token) > 0); t= t->next);
        u= t->next;
        t->next= new_token(s);
        t->next->next= u;
        return(t->next->number);
      }
    }
  }
}

void print_token(tok)
int tok;
{ TOKEN_LIST *t;

  t= t_list;
  while (t) {
    if (t->number == tok) {
      printf("%s",t->token);
      return;
    }
    t= t->next;
  }

/*
 * if we get down this far, it's screwed.
 */

  panic("print_token");
}

int token_val(tok)
int tok;
{ TOKEN_LIST *t;

  t= t_list;
  while (t) {
    if (t->number == tok)
      return(t->value);
    t= t->next;
  }
  panic("token_val");
}
