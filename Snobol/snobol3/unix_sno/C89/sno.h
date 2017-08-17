/*
 *   Snobol III
 */

/* c libraries */
# include <stdlib.h>
# include <stdint.h>
# include <stdio.h> 

 /* #include <string.h> */ 

struct	node {
	struct node *p1;
	struct node *p2;
	char typ;
	char ch;
};

int	freesize;
struct	node *lookf;
struct	node *looks;
struct	node *lookend;
struct	node *lookstart;
struct	node *lookdef;
struct	node *lookret;
struct	node *lookfret;
int	cfail;
int	rfail;
struct	node *freelist;
struct	node *namelist;
int	lc;
struct	node *schar;


/* all function declarations for sno1 - sno4  except main() */

/* sno1 */
void mes(char* s);
struct node* init(char *s, int t);
struct node* syspit();
void syspot(struct node* string);
struct node* _strstr(char* s);
int class(int c);
struct node* _alloc();
void _free(struct node* pointer);
int nfree();
struct node* look(struct node* string);
struct node* copy(struct node* string);
int equal(struct node* string1, struct node* string2);
int strbin(struct node* string);
struct node* binstr(int binary);
struct node* add(struct node* string1, struct node* string2);
struct node* sub(struct node* string1, struct node* string2);
struct node* mult(struct node* string1, struct node* string2);
struct node* _div(struct node* string1, struct node* string2);
struct node* cat(struct node* string1, struct node* string2);
struct node* dcat(struct node* a, struct node* b);
void delete(struct node* string);
void sysput(struct node* string);
void dump();
void dump1(struct node* base);
void writes(char* s);
struct node* _getc();

/* sno2 */
struct node* compon();
struct node* nscomp();
struct node* push(struct node* stack);
struct node* pop(struct node* stack);
struct node* expr(struct node* start, char eof, struct node* e);
struct node* match(struct node* start, struct node* m);
struct node* compile();

/* sno3 */
int bextend(struct node* str, struct node* last);
int ubextend(struct node* str, struct node* last);
struct node* search(struct node* arg, struct node* r);

/* sno4 */
struct node* and(struct node* ptr);
struct node* eval(struct node* e, int t);
struct node* doop(struct node* op, struct node* arg1, struct node* arg2);
struct node* execute(struct node* e);
void assign(struct node* adr, struct node* val);
