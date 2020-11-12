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
/* bool.h:
 *
 * definition file for ttable.c
 */

#include <stdio.h>

#define PROG     "bool"    /* our name */
#define MAXTOKEN 32        /* maximum length of a token */

typedef struct bool_exp {
  int opcode,          /* operation code/constant/token */
      value;           /* token number/opcode char */
  char n1,             /* unary "not" indicators */
       n2;
  struct bool_exp *b1, /* boolean expressions */
                  *b2;
} BOOL_EXP;

/*
 * opcode field values
 */

#define UNDEF -1 /* undefined operation */
#define TRUE   0
#define FALSE  1
#define TOKEN  2
#define OR     3
#define AND    4
#define XOR    5

typedef struct token_list {
  char token[MAXTOKEN];    /* character representation of the token */
  int  number,             /* numeric token representation */
       value;              /* current value of token */
  struct token_list *next; /* next token in list */
} TOKEN_LIST;

typedef struct func_list {
  char func[MAXTOKEN];     /* name of this function */
  BOOL_EXP *bool_exp;      /* boolean expression */
  struct func_list *next;  /* next function in list */
} FUNC_LIST;

BOOL_EXP   *build(),
           *newbnode();
FUNC_LIST  *new_func();
TOKEN_LIST *new_token();
void       eval_file(),
           eval_func(),
           free_tree(),
           panic(),
           print_token(),
           print_tree();
int        begtok(),
           eval_tree(),
           get_token(),
           intok(),
           token_val();
char       *malloc(),
           *strcpy();

#define cget(F,C) if (!feof(F)) {\
                    C= fgetc(F);\
                    if ((!no_print) && (C != EOF))\
                      printf("%c",C);\
                  } else
