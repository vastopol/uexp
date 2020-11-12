/*
 * Mini Scheme Interpreter
 * By Atsushi Moriwaki <moriwaki@kurims.kurims.kyoto-u.ac.jp> (1989-05-11)
 *    Akira KIDA <SDI00379@niftyserve.or.jp>                  (1994-05-17)
 *    Nils M Holm <nmh@t3x.org>                               (2009..2012)
 *
 * THIS PROGRAM IS IN THE PUBLIC DOMAIN
 * (But please leave the list of authors intact!)
 */ 

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define BANNER "Mini Scheme 2012-09-09\n"
#define PROMPT "> "
#define INIT_FILE "miniscm.scm"

/* Options */

/* #define VERBOSE_GC	/* print GC stats */
#define	LOOP_HACK	/* use "do { } while (0)" to delimit macros */
#define USE_SETJMP	/* use setjmp() to recover from low mem and SIGINT */
#define INTERNAL_INIT	/* compile init file into executable */
#define PRESERVE_FILES	/* file ops will preserve existing files */

#ifdef __TURBOC__
 #define CELL_SEGSIZE		2048	/* cells per segment */
 #define STR_SEGSIZE		2048	/* chars per string segment */
 #define INITIAL_CELLSEGS	3
#else /* Unix */
 #define CELL_SEGSIZE		8192
 #define STR_SEGSIZE		8192
 #define INITIAL_CELLSEGS 	10
#endif

#define CELL_NSEGMENT   100	/* max. cell segments */
#define STR_NSEGMENT    100	/* max. string segments */

#ifdef USE_SETJMP
 #include <setjmp.h>
 jmp_buf error_jmp;
#endif

#ifdef INTERNAL_INIT
 #include "miniscm.h"
#endif

struct _cell {
	#define cell struct _cell
	unsigned short type;
	union {
		struct {
			char	*svalue;
			short	_key;
		} _string;
		struct {
			long	ivalue;
		} _integer;
		struct {
			cell	*car;
			cell	*cdr;
		} cons;
	} object;
};

#define T_STRING	0x0001	/* 0000000000000001 */
#define T_INT		0x0002	/* 0000000000000010 */
#define T_SYMBOL	0x0004	/* 0000000000000100 */
#define T_CHAR		0x0008	/* 0000000000001000 */
#define T_SYNTAX	0x0010	/* 0000000000010000 */
#define T_PROC		0x0020	/* 0000000000100000 */
#define T_PAIR		0x0040	/* 0000000001000000 */
#define T_CLOSURE	0x0080	/* 0000000010000000 */
#define T_CONT		0x0100	/* 0000000100000000 */
#define T_MACRO		0x0200	/* 0000001000000000 */
#define T_LIST		0x0400	/* (type_ checking) */
#define T_ARGLST	0x0800	/* (typ_e checking) */
#define T_LETENV	0x1000	/* (ty_pe checking) */
#define T_ATOM		0x4000	/* 0100000000000000 */
#define CLRATOM		0xbfff	/* 1011111111111111 */
#define MARK		0x8000	/* 1000000000000000 */
#define UNMARK		0x7fff	/* 0111111111111111 */

/* macros for cell operations */

#define type(p)		((p)->type)

#define is_string(p)	(type(p)&T_STRING)
#define string(p)	((p)->object._string.svalue)
#define key(p)		((p)->object._string._key)
#define str_size(p)	(((long *) string(p))[-2])
#define str_backref(p)	(((long *) string(p))[-1])

#define is_int(p)	(type(p)&T_INT)
#define ivalue(p)	((p)->object._integer.ivalue)

#define is_char(p)	(type(p)&T_CHAR)
#define cvalue(p)	(ivalue(p))

#define is_pair(p)	(type(p)&T_PAIR)
#define car(p)		((p)->object.cons.car)
#define cdr(p)		((p)->object.cons.cdr)

#define is_symbol(p)	(type(p)&T_SYMBOL)
#define symname(p)	string(car(p))

#define is_syntax(p)	(type(p)&T_SYNTAX)
#define syntax_id(p)	key(car(p))

#define is_proc(p)	(type(p)&T_PROC)
#define proc_id(p)	ivalue(p)

#define is_closure(p)	(type(p)&T_CLOSURE)
#define closure_code(p)	car(p)
#define closure_env(p)	cdr(p)

#define is_macro(p)	(type(p)&T_MACRO)

#define is_cont(p)	(type(p)&T_CONT)
#define cont_dump(p)	cdr(p)

#define atomic(p)	(type(p)&T_ATOM)
#define setatom(p)	type(p) |= T_ATOM
#define clratom(p)	type(p) &= CLRATOM

#define marked(p)	(type(p)&MARK)
#define setmark(p)	type(p) |= MARK
#define clrmark(p)	type(p) &= UNMARK

#define caar(p)		car(car(p))
#define cadr(p)		car(cdr(p))
#define cdar(p)		cdr(car(p))
#define cddr(p)		cdr(cdr(p))
#define cadar(p)	car(cdr(car(p)))
#define caddr(p)	car(cdr(cdr(p)))
#define cddar(p)	cdr(cdr(car(p)))
#define cdddr(p)	cdr(cdr(cdr(p)))
#define cadaar(p)	car(cdr(car(car(p))))
#define cadddr(p)	car(cdr(cdr(cdr(p))))
#define cddddr(p)	cdr(cdr(cdr(cdr(p))))

/* segments */

cell	*cell_seg[CELL_NSEGMENT];
int	last_cell_seg = -1;
long	*str_seg = NULL;
int	str_segno = 0;

/* registers */

cell	*args;			/* function arguments */
cell	*envir;			/* current environment */
cell	*code;			/* current code */
cell	*dump;			/* next evaluation */

/* special objects */

cell	_NIL, *NIL = &_NIL;	/* nil */
cell	_T,   *T = &_T;		/* #t */
cell	_F,   *F = &_F;		/* #f */
cell	_EOF, *EOFOBJ = &_EOF;	/* #<eof> */
cell	*symbols = &_NIL;	/* symbol table */
cell	*global_env;		/* global environment */

cell	*LAMBDA;		/* lambda keyword */
cell	*QUOTE;			/* quote keyword */
cell	*QQUOTE;		/* quasiquote symbol */
cell	*UNQUOTE;		/* unquote symbol */
cell	*UNQUOTESP;		/* unquote-splicing symbol */

cell	*free_cell = &_NIL;	/* free list */
long    fcells = 0;		/* # of free cells */
long    ncells = 0;		/* # of total cells */
long	*free_string = NULL;	/* free string space */

FILE   *infp;			/* input file */
FILE   *outfp;			/* output file */

/* flags */

int	quiet = 0;

/* prototypes */

void	gc(cell *a, cell *b);
cell	*error(char *msg);
void	fatal(char *msg);

int alloc_cellseg(int n) {
	cell	*p;
	long	i;
	int	k;

	for (k = 0; k < n; k++) {
		if (last_cell_seg >= CELL_NSEGMENT - 1)
			return k;
		p = (cell *) malloc(CELL_SEGSIZE * sizeof(cell));
		if (!p)
			return k;
		cell_seg[++last_cell_seg] = p;
		fcells += CELL_SEGSIZE;
		ncells += CELL_SEGSIZE;
		for (i = 0; i < CELL_SEGSIZE - 1; i++, p++) {
			type(p) = 0;
			car(p) = NIL;
			cdr(p) = p + 1;
		}
		type(p) = 0;
		car(p) = NIL;
		cdr(p) = free_cell;
		free_cell = cell_seg[last_cell_seg];
	}
	return n;
}

cell *new_cell(cell *a, cell *b) {
	cell	*x;

	if (free_cell == NIL) {
		gc(a, b);
		/*
		 * NEVER, ever let the pool run dry!
		 * This tiny hack reduces the run time of
		 * (IOTA 1 100000) from 63.4s to 4.4s (!)
		 * on my 600MHz machine. --nmh
		 */
		if (fcells < ncells/2) {
#ifdef USE_SETJMP
			if (alloc_cellseg(1) == 0) {
				args = envir = code = dump = NIL;
				error("ran out of cells");
			}
#else
			if (alloc_cellseg(1) == 0)
				fatal("ran out of cells");
#endif
		}
	}
	x = free_cell;
	free_cell = cdr(x);
	--fcells;
	return x;
}

cell *cons(cell *a, cell *b) {
	cell	*x = new_cell(a, b);

	type(x) = T_PAIR;
	car(x) = a;
	cdr(x) = b;
	return x;
}

cell *mk_number(long num) {
	cell	*x = new_cell(NIL, NIL);

	type(x) = (T_INT | T_ATOM);
	ivalue(x) = num;
	return x;
}

cell *mk_char(int c) {
	cell	*x = new_cell(NIL, NIL);

	type(x) = (T_CHAR | T_ATOM);
	cvalue(x) = c;
	return x;
}

#define seg_str_size(p)	(p[0])
#define seg_backref(p)	((cell *) p[1])

int alloc_strseg(void) {
	long	free, size;
	long	*p, *old_seg;
	long	delta;
	cell	*x;

	if (str_segno >= STR_NSEGMENT)
		return 0;
	if (!free_string)
		free_string = str_seg;
	free = free_string - str_seg;
	old_seg = str_seg;
	size = ++str_segno * STR_SEGSIZE * sizeof(char *);
	str_seg = (long *) realloc(str_seg, size);
	if (!str_seg)
		return 0;
	free_string = str_seg + free;
	/*
	 * fix references to string objects
	 * in case realloc() moved the pool.
	 */
	delta = (long) str_seg - (long) old_seg;
	if (old_seg && delta) {
		for (p = str_seg; p < free_string; p += seg_str_size(p)) {
			x = seg_backref(p);
			if (is_string(x)) string(x) += delta;
		}
	}
	return 1;
}

#define size_in_longs(k) \
	(((k) + sizeof(long) - 1) / sizeof(long))

#define string_size(k) \
	((k) + 1 + 2 * (long) sizeof(long))

#define remain() \
	((long) (STR_SEGSIZE * str_segno) -	\
	 ((long) free_string - (long) str_seg))

char *new_string(cell *parent, char *text, long len) {
	char	*s;

	len = string_size(len);
	for (;;) {
		if (remain() >= len)
			break;
		gc(parent, NIL);
		if (remain() >= len)
			break;
		if (!alloc_strseg())
			fatal("ran out of string space");
	}
	free_string[0] = size_in_longs(len);
	free_string[1] = 0L;
	s = (char *) &free_string[2];
	strcpy(s, text);
	free_string += size_in_longs(len);
	return s;
}

cell *mk_string2(char *str, long len) {
	cell	*x = new_cell(NIL, NIL);

	string(x) = new_string(x, str, len);
	type(x) = (T_STRING | T_ATOM);
	key(x) = (short) -1;
	str_backref(x) = (long) x;
	return x;
}

cell *mk_string(char *str) {
	return mk_string2(str, (long) strlen(str));
}

cell *findsym(char *name) {
	cell	*x;

	for (x = symbols; x != NIL; x = cdr(x))
		if (!strcmp(name, symname(car(x))))
			return car(x);
	return NIL;
}

cell *mk_symbol(char *name) {
	cell	*x;

	if ((x = findsym(name)) != NIL)
		return x;
	x = cons(mk_string(name), NIL);
	type(x) = T_SYMBOL;
	symbols = cons(x, symbols);
	return x;
}

/* make symbol or number atom from string */
cell *mk_atom(char *q) {
	int	c, sign = 1;
	char	*p;
	long	v;

	p = q;
	if (!isdigit(c = *p++)) {
		if ((c != '+' && c != '-') || !isdigit(*p))
			return mk_symbol(q);
	}
	for (; (c = *p) != 0; ++p)
		if (!isdigit(c))
			return mk_symbol(q);
	if (*q == '-') {
		sign = -1;
		q++;
	}
	else if (*q == '+') {
		sign = 1;
		q++;
	}
	for (v=0; *q; q++)
		if ((v = v * 10 + *q-'0') < 0)
			return error("numeric literal too big");
	return mk_number(v * sign);
}

int strcmp_i(char *a, char *b) {
	for (; *a && tolower(*a) == tolower(*b); a++, b++)
		;
	return tolower(*a) - tolower(*b);
}

cell *mk_const(char *name) {
	int		c;

	if (!strcmp_i(name, "t"))		/* #t */
		return T;
	else if (!strcmp_i(name, "f"))		/* #f */
		return F;
	else if (*name == '\\') {		/* #\c */
		if (!strcmp_i(name+1, "space"))
			c = ' ';
		else if (!strcmp_i(name+1, "newline"))
			c = '\n';
		else if (name[2])
			return NIL;
		else
			c = name[1];
		return mk_char(c);
	} else
		return NIL;
}

/*
 * Garbage Collector, Mark phase
 * Knuth's algorithm E (The Art of Computer Programming Vol.1, sec.3.5)
 */
void mark(cell *a) {
	cell	*t, *q, *p;
	enum	{ E1, E2, E3, E4 };
	int	k;

	t = NULL;
	p = a;
	for (k = E1; k != E4;) {
		switch (k) {
		case E1:
			setmark(p);
			if (atomic(p)) {
				if (is_string(p))
					str_size(p) = -labs(str_size(p));
				k = E3;
				break;
			}
			q = car(p);
			if (q && !marked(q)) {
				setatom(p);
				car(p) = t;
				t = p;
				p = q;
				k = E1;
				break;
			}
		case E2:
			q = cdr(p);
			if (q && !marked(q)) {
				cdr(p) = t;
				t = p;
				p = q;
				k = E1;
				break;
			}
		case E3:
			if (!t) {
				k = E4;
				break;
			}
			q = t;
			if (atomic(q)) {
				clratom(q);
				t = car(q);
				car(q) = p;
				p = q;
				k = E2;
				break;
			} else {
				t = cdr(q);
				cdr(q) = p;
				p = q;
				k = E3;
				break;
			}
		}
	}
}

/********************************************************************
 * WARNING: the string pool compactor *will* relocate string values *
 * and symbol names! Subsequently code like this will not work:     *
 *         x = mk_string("", 3);                                    *
 *         S = string(x);                                           *
 *         x = cons(x, NIL); // trigger GC                          *
 *         strcpy(S, "foo"); // BOOM! -- S has moved!               *
 ********************************************************************/

void compact(void) {
	long	*fr, *to, k;

	to = str_seg;
	for (fr = str_seg; fr < free_string; fr += k) {
		k = labs(seg_str_size(fr));
		if (seg_str_size(fr) < 0) {
			seg_str_size(fr) = k;
			string(seg_backref(fr)) = (char *) (to+2);
			if (to != fr)
				memmove(to, fr, k * sizeof(long));
			to += k;
		}
	}
	free_string = to;
}

/* parameters a, b are marked */
void gc(cell *a, cell *b) {
	cell	*p;
	int	i;
	long	j;

#ifdef VERBOSE_GC
	printf("gc: ");
#endif
	mark(symbols);
	mark(global_env);
	mark(args);
	mark(envir);
	mark(code);
	mark(dump);
	mark(a);
	mark(b);
	clrmark(NIL);
	fcells = 0;
	free_cell = NIL;
	for (i = 0; i <= last_cell_seg; i++) {
		for (j = 0, p = cell_seg[i]; j < CELL_SEGSIZE; j++, p++) {
			if (marked(p))
				clrmark(p);
			else {
				type(p) = 0;
				cdr(p) = free_cell;
				car(p) = NIL;
				free_cell = p;
				++fcells;
			}
		}
	}
	compact();
#ifdef VERBOSE_GC
	printf(" %ld cells recovered\n", fcells);
#endif
}

/* Reader */

#define TOK_EOF		-1
#define TOK_LPAREN	0
#define TOK_RPAREN	1
#define TOK_DOT		2
#define TOK_ATOM	3
#define TOK_QUOTE	4
#define TOK_COMMENT	5
#define TOK_DQUOTE	6
#define TOK_BQUOTE	7
#define TOK_COMMA	8
#define TOK_SPLICE	9
#define TOK_SHARP	10

#define	STRSIZE		256

char	strbuf[STRSIZE];
char	*currentchar = NULL;
char	*endline = NULL;

volatile int	intr = 0;

int _inchar(void) {
	if (intr)
		return EOF;
#ifdef INTERNAL_INIT
	if (!infp) {
		if (currentchar >= endline) {	/* end of buffer */
			infp = stdin;
			if (!quiet) printf(PROMPT);
			return fgetc(infp);
		}
		return *currentchar++;
	}
#endif
	return fgetc(infp);
}

int inchar() {
	return tolower(_inchar());
}

/* back to standard input */
void flushinput(void) {
	if (infp != stdin) {
		if (infp) fclose(infp);
		infp = stdin;
	}
}

void backchar(int c) {
	if (!infp)
		currentchar--;
	else
		ungetc(c, infp);
}

char *readtok(int fold) {
	char	*p = strbuf;
	char	delim[] = "();\t\n ";
	int	c;

	for (;;) {
		if (p - strbuf >= STRSIZE)
			return error("token too long");
		c = fold? inchar(): _inchar();
		*p = c;
		if (p == strbuf+1 && *(p-1) == '\\')
			;
		else if (strchr(delim, c))
			break;
		p++;
	}
	backchar(c);
	*p = '\0';
	return strbuf;
}

char *readstring(void) {
	char	c, *p = strbuf;

	for (;;) {
		if (p - strbuf >= STRSIZE)
			return error("string too long");
		if ((c = _inchar()) == '\\') {
			*p++ = _inchar();
		} else if (c == EOF || c == '"') {
			*p = '\0';
			return strbuf;
		} else {
			*p++ = c;
		}
	}
}

void skipspace(void) {
	int	c;

	while (isspace(c = inchar()))
		;
	backchar(c);
}

int token(void) {
	int	c;

	skipspace();
	switch (c = inchar()) {
	case EOF:	return TOK_EOF;
	case '(':	return TOK_LPAREN;
	case ')':	return TOK_RPAREN;
	case '.':	return TOK_DOT;
	case '\'':	return TOK_QUOTE;
	case ';':	return TOK_COMMENT;
	case '"':	return TOK_DQUOTE;
	case '`':	return TOK_BQUOTE;
	case ',':	if ((c = inchar()) == '@')
				return TOK_SPLICE;
			else {
				backchar(c);
				return TOK_COMMA;
			}
	case '#':	return TOK_SHARP;
	default:	backchar(c);
			return TOK_ATOM;
	}
}

/* Printer */

void put_qstr(char *s) {
	fputc('"', outfp);
	for ( ; *s; ++s) {
		if (*s == '"' || *s == '\\')
			fputc('\\', outfp);
		fputc(*s, outfp);
	}
	fputc('"', outfp);
}

char *qchar(char *p, int c) {
	if (c == ' ')
		strcpy(p, "#\\space");
	else if (c == '\n')
		strcpy(p, "#\\newline");
	else
		sprintf(p, "#\\%c", c);
	return p;
}

void printatom(cell *l, int f) {
	char	*p;
	
	if (l == NIL)
		p = "()";
	else if (l == T)
		p = "#t";
	else if (l == F)
		p = "#f";
	else if (l == EOFOBJ)
		p = "#<eof>";
	else if (is_int(l)) {
		p = strbuf;
		sprintf(p, "%ld", ivalue(l));
	} else if (is_char(l)) {
		if (!f)
			sprintf(p = strbuf, "%c", (int) cvalue(l));
		else
			p = qchar(strbuf, cvalue(l));
	} else if (is_string(l)) {
		if (!f)
			p = string(l);
		else {
			put_qstr(string(l));
			p = NULL;
		}
	} else if (is_symbol(l))
		p = symname(l);
	else if (is_proc(l) || is_closure(l) || is_cont(l)) {
		p = "#<procedure>";
	} else if (is_macro(l)) {
		p = "#<macro>";
	}
	if (p) fputs(p, outfp);
}

/* Evaluator */

cell *mk_closure(cell *code, cell *env) {
	cell	*x = new_cell(code, env);

	type(x) = T_CLOSURE;
	car(x) = code;
	cdr(x) = env;
	return x;
}

cell *mk_continuation(cell *d) {
	cell	*x = new_cell(NIL, d);

	type(x) = T_CONT;
	cont_dump(x) = d;
	return x;
}

cell *reverse(cell *a) {	/* a must be marked by gc */
	cell	*p = NIL;

	for (; is_pair(a); a = cdr(a))
		p = cons(car(a), p);
	return p;
}

cell *nreverse(cell *result, cell *list) {
	cell	*p = list, *q;

	while (p != NIL) {
		q = cdr(p);
		cdr(p) = result;
		result = p;
		p = q;
	}
	return result;
}

cell *append(cell *a, cell *b) {	/* a,b must be marked by gc */
	cell	*p = b, *q;

	if (a != NIL) {
		a = reverse(a);
		while (a != NIL) {
			q = cdr(a);
			cdr(a) = p;
			p = a;
			a = q;
		}
	}
	return p;
}

int eqv(cell *a, cell *b) {
	if (is_int(a)) {
		if (is_int(b))
			return ivalue(a) == ivalue(b);
		else
			return 0;
	} else if (is_char(a)) {
		if (is_char(b))
			return cvalue(a) == cvalue(b);
		else
			return 0;
	} else
		return a == b;
}

#define true(p)		((p) != F)
#define false(p)	((p) == F)

#ifdef	LOOP_HACK
 #define	BEGIN	do {
 #define	END	} while (0)
#else
 #define	BEGIN	{
 #define	END	}
#endif

/* error macros */

#define error_0(w, s) BEGIN			\
	args = cons(mk_string(s), NIL);		\
	args = cons(mk_string(w), args);	\
	operator = (short)OP_ERR0;		\
	return T; END

#define error_1(w, s, a) BEGIN			\
	args = cons((a), NIL);			\
	args = cons(mk_string(s), args);	\
	args = cons(mk_string(w), args);	\
	operator = (short)OP_ERR0;		\
	return T; END

/* type checking macros */

#define typechk_0(w, a) BEGIN			\
	e = check0(a);				\
	if (e) error_0(w, e); END

#define typechk_1(w, a, t1) BEGIN		\
	e = check1(a, t1);			\
	if (e) error_0(w, e); END

#define typechk_2(w, a, t1, t2) BEGIN		\
	e = check2(a, t1, t2);			\
	if (e) error_0(w, e); END

#define typechk_3(w, a, t1, t2, t3) BEGIN	\
	e = check3(a, t1, t2, t3);		\
	if (e) error_0(w, e); END

#define typechk_v(w, a, t) BEGIN		\
	e = check(a, t);			\
	if (e) error_0(w, e); END

/* control macros for eval_cycle */

#define s_goto(a) BEGIN				\
	operator = (short)(a);			\
	return T; END

#define s_save(a, b, c)  (			\
	dump = cons(envir, cons((c), dump)),	\
	dump = cons((b), dump),			\
	dump = cons(mk_number((long)(a)), dump))

#define s_return(a) BEGIN			\
	value = (a);				\
	operator = ivalue(car(dump));		\
	args = cadr(dump);			\
	envir = caddr(dump);			\
	code = cadddr(dump);			\
	dump = cddddr(dump);			\
	return T; END

#define s_retbool(tf)	s_return((tf)? T: F)

/* opcodes */

enum {
	OP_INIT = 0, OP_LOAD, OP_T0LVL, OP_T1LVL, OP_READ, OP_RDSEXPR,
	OP_RDLIST, OP_RDDOT, OP_RDQUOTE, OP_RDQQUOTE, OP_RDUNQUOTE,
	OP_RDUQTSP, OP_PRINT, OP_PEVAL, OP_EVAL, OP_E0ARGS, OP_E1ARGS,
	OP_DOMACRO, OP_PAPPLY, OP_APPLY, OP_EXP0MAC, OP_EXP1MAC,
	OP_GENSYM, OP_LAMBDA, OP_QUOTE, OP_DEF0, OP_DEF1, OP_SET0,
	OP_SET1, OP_BEGIN, OP_IF0, OP_IF1, OP_LET0, OP_LET1, OP_LET2,
	OP_LET0SEQ, OP_LET1SEQ, OP_LET2SEQ, OP_LET0REC, OP_LET1REC,
	OP_LET2REC, OP_COND0, OP_COND1, OP_AND0, OP_AND1, OP_OR0,
	OP_OR1, OP_0DEFMAC, OP_1DEFMAC, OP_CASE0, OP_CASE1, OP_CASE2,
	OP_CALLCC, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_REM, OP_ABS,
	OP_EVEN, OP_ODD, OP_MIN, OP_MAX, OP_EXPT, OP_GCD, OP_LCM,
	OP_CAR, OP_CDR, OP_CAAR, OP_CADR, OP_CDAR, OP_CDDR, OP_CAAAR,
	OP_CAADR, OP_CADAR, OP_CADDR, OP_CDAAR, OP_CDADR, OP_CDDAR,
	OP_CDDDR, OP_CONS, OP_LSTTAIL, OP_LSTREF, OP_MEMQ, OP_MEMV,
	OP_ASSQ, OP_ASSV, OP_SETCAR, OP_SETCDR, OP_NOT, OP_BOOL,
	OP_NULL, OP_ZEROP, OP_POSP, OP_NEGP, OP_NUMEQ, OP_NUMPRED,
	OP_LESS, OP_GRE, OP_LEQ, OP_GEQ, OP_SYMBOL, OP_NUMBER,
	OP_STRING, OP_PROC, OP_PAIR, OP_LIST, OP_EQ, OP_EQV, OP_WRITE,
	OP_P0LIST, OP_P1LIST, OP_DISPLAY, OP_NEWLINE, OP_ERROR,
	OP_ERR0, OP_ERR1, OP_REVERSE, OP_APPEND, OP_LENGTH,
	OP_W0INFILE, OP_W1INFILE, OP_W0OUTFILE, OP_W1OUTFILE,
	OP_EOFOBJ, OP_RDCHR, OP_PKCHR, OP_WRCHR, OP_CHAR, OP_CHREQ,
	OP_CHRPRED, OP_CHRLT, OP_CHRGT, OP_CHRLE, OP_CHRGE, OP_CHREQI,
	OP_CHRLTI, OP_CHRGTI, OP_CHRLEI, OP_CHRGEI, OP_CHRALPH,
	OP_CHRNUM, OP_CHRUPP, OP_CHRLOW, OP_CHRWHIT, OP_CHRDNC,
	OP_CHRUPC, OP_CHRINT, OP_INTCHR, OP_MKSTR, OP_STREQ,
	OP_STREQI, OP_STRLT, OP_STRLTI, OP_STRLEN, OP_STRAPND,
	OP_STRREF, OP_STRSET, OP_SUBSTR, OP_LSTSTR, OP_STRLST,
	OP_STRFIL, OP_STRCPY, OP_STRSYM, OP_SYMSTR
};

char *check(cell *arg, int t) {
	cell	*p;
	char	*badargs = "bad argument list";

	switch(t) {
	case T_PAIR:
		if (!is_pair(arg)) return "expected pair";
		break;
	case T_LIST:
		if (arg != NIL && !is_pair(arg)) return "expected list";
		break;
	case T_SYMBOL:
		if (!is_symbol(arg)) return "expected symbol";
		break;
	case T_INT:
		if (!is_int(arg)) return "expected integer";
		break;
	case T_CHAR:
		if (!is_char(arg)) return "expected char";
		break;
	case T_STRING:
		if (!is_string(arg)) return "expected string";
		break;
	case T_PROC:
		if (!is_proc(arg) && !is_closure(arg) && !is_cont(arg))
			return "expected procedure";
		break;
	case T_ARGLST:
		if (is_symbol(arg))
			return NULL;
		for (p = arg; p != NIL && is_pair(p); p = cdr(p))
			if (!is_symbol(car(p)))
				return badargs;
		if (p != NIL && !is_symbol(p))
			return badargs;
		break;
	case T_LETENV:
		for (p = arg; p != NIL; p = cdr(p))
			if (	!is_pair(p) ||
				!is_pair(car(p)) ||
				!is_pair(cdar(p)) ||
				cddar(p) != NIL
			)
				return "expected 'let' environment";
		break;
	}
	return NULL;
}

char	wrong_args[] = "wrong number of arguments";

char *check0(cell *args) {
	if (args != NIL)
		return wrong_args;
	return NULL;
}

char *check1(cell *args, int t1) {
	if (args == NIL || cdr(args) != NIL)
		return wrong_args;
	return check(car(args), t1);
}

char *check2(cell *args, int t1, int t2) {
	char	*e;

	if (args == NIL || cdr(args) == NIL || cddr(args) != NIL)
		return wrong_args;
	e = check(car(args), t1);
	return e? e: check(cadr(args), t2);
}

char *check3(cell *args, int t1, int t2, int t3) {
	char	*e;

	if (	args == NIL || cdr(args) == NIL || cddr(args) == NIL ||
		cdddr(args) != NIL
	)
		return wrong_args;
	e = check(car(args), t1);
	if (e) return e;
	e = check(cadr(args), t2);
	return e? e: check(caddr(args), t3);
}

int	tok;
int	print_flag;
cell	*value;
short	operator;
short	sub_op;
long	gensym = 0;
char	gensymbuf[30];

cell *invalid_op(void) {
	sprintf(strbuf, "invalid operator: %d", operator);
	error_0("internal", strbuf);
}

void extend(cell *sym, cell *val) {
	cell	*x;

	for (x = car(envir); x != NIL; x = cdr(x))
		if (caar(x) == sym)
			break;
	if (x != NIL)
		cdar(x) = val;
	else
		car(envir) = cons(cons(sym, val), car(envir));
}

cell *op_init(short op) {
#ifdef INTERNAL_INIT
	currentchar = init_scm;
	endline = currentchar + strlen(currentchar);
	infp = NULL;
	args = NIL;
	s_goto(OP_T0LVL);
#else
	args = cons(mk_string(INIT_FILE), NIL);
	s_goto(OP_LOAD);
#endif
}

cell *op_load(short op) {
	char	*e;

	typechk_1("load", args, T_STRING);
	if ((infp = fopen(string(car(args)), "r")) == NULL) {
		infp = stdin;
		error_1("load", "cannot open:", car(args));
	}
	s_goto(OP_T0LVL);
}

cell *op_toplvl(short op) {
	switch (op) {
	case OP_T0LVL:	/* top level */
		if (!quiet && infp == stdin)
			putchar('\n');
		dump = NIL;
		args = NIL;
		envir = global_env;
		s_save(OP_PRINT, NIL, NIL);
		s_save(OP_T1LVL, NIL, NIL);
		if (!quiet && infp == stdin)
			printf(PROMPT);
		s_goto(OP_READ);
	case OP_T1LVL:	/* top level */
		code = value;
		if (value == EOFOBJ) {
			if (infp == stdin) {
				if (!quiet) putc('\n', outfp);
				exit(0);
			}
			fclose(infp);
			infp = stdin;
			s_return(T);
		}
		s_goto(OP_EVAL);
	}
}

cell *op_read(short op) {
	cell	*x;
	char	*e;

	switch (op) {
	case OP_READ:	/* read */
		typechk_0("read", args);
		tok = token();
		s_goto(OP_RDSEXPR);
	case OP_RDSEXPR:
		switch (tok) {
		case TOK_EOF:
			s_return(EOFOBJ);
		case TOK_COMMENT:
			while (inchar() != '\n')
				;
			tok = token();
			s_goto(OP_RDSEXPR);
		case TOK_LPAREN:
			tok = token();
			if (tok == TOK_RPAREN) {
				s_return(NIL);
			} else if (tok == TOK_DOT) {
				error_0("read", "unexpected dot");
			} else {
				s_save(OP_RDLIST, NIL, NIL);
				s_goto(OP_RDSEXPR);
			}
		case TOK_QUOTE:
			s_save(OP_RDQUOTE, NIL, NIL);
			tok = token();
			s_goto(OP_RDSEXPR);
		case TOK_BQUOTE:
			s_save(OP_RDQQUOTE, NIL, NIL);
			tok = token();
			s_goto(OP_RDSEXPR);
		case TOK_COMMA:
			s_save(OP_RDUNQUOTE, NIL, NIL);
			tok = token();
			s_goto(OP_RDSEXPR);
		case TOK_SPLICE:
			s_save(OP_RDUQTSP, NIL, NIL);
			tok = token();
			s_goto(OP_RDSEXPR);
		case TOK_ATOM:
			s_return(mk_atom(readtok(1)));
		case TOK_DQUOTE:
			s_return(mk_string(readstring()));
		case TOK_SHARP:
			if ((x = mk_const(readtok(0))) == NIL) {
				error_0("read", "undefined # syntax");
			} else {
				s_return(x);
			}
		default:
			error_0("read", "syntax error");
		}
		break;
	case OP_RDLIST:
		args = cons(value, args);
		tok = token();
		if (tok == TOK_COMMENT) {
			while (inchar() != '\n')
				;
			tok = token();
		}
		if (tok == TOK_RPAREN) {
			s_return(nreverse(NIL, args));
		} else if (tok == TOK_DOT) {
			s_save(OP_RDDOT, args, NIL);
			tok = token();
			s_goto(OP_RDSEXPR);
		} else {
			s_save(OP_RDLIST, args, NIL);;
			s_goto(OP_RDSEXPR);
		}
	case OP_RDDOT:
		if (token() != TOK_RPAREN) {
			error_0("read", "malformed dotted pair");
		} else {
			s_return(nreverse(value, args));
		}
	case OP_RDQUOTE:
		s_return(cons(QUOTE, cons(value, NIL)));
	case OP_RDQQUOTE:
		s_return(cons(QQUOTE, cons(value, NIL)));
	case OP_RDUNQUOTE:
		s_return(cons(UNQUOTE, cons(value, NIL)));
	case OP_RDUQTSP:
		s_return(cons(UNQUOTESP, cons(value, NIL)));
	}
}

cell *op_print(short op) {
	print_flag = 1;
	args = value;
	if (!quiet && infp == stdin) {
		s_save(OP_T0LVL, NIL, NIL);
		s_goto(OP_P0LIST);
	} else {
		s_goto(OP_T0LVL);
	}
}

cell *op_eval(short op) {
	cell	*x, *y;
	char	*e;

	switch (op) {
	case OP_PEVAL:		/* eval */
		typechk_1("eval", args, 0);
		code = car(args);
		args = NIL;
		s_goto(OP_EVAL);
	case OP_EVAL:		/* expression evaluation */
		if (is_symbol(code)) {	/* symbol */
			for (x = envir; x != NIL; x = cdr(x)) {
				for (y = car(x); y != NIL; y = cdr(y))
					if (caar(y) == code)
						break;
				if (y != NIL)
					break;
			}
			if (x != NIL) {
				s_return(cdar(y));
			} else {
				error_1("eval", "unbound symbol:", code);
			}
		} else if (is_pair(code)) {
			if (is_syntax(x = car(code))) {	/* syntax */
				code = cdr(code);
				s_goto(syntax_id(x));
			} else {	/* first, eval members */
				s_save(OP_E0ARGS, NIL, code);
				code = car(code);
				s_goto(OP_EVAL);
			}
		} else {
			s_return(code);
		}
	case OP_E0ARGS:		/* eval arguments */
		if (is_macro(value)) {	/* macro expansion */
			s_save(OP_DOMACRO, NIL, NIL);
			args = cdr(code);
			code = value;
			s_goto(OP_APPLY);
		} else {
			code = cdr(code);
			s_goto(OP_E1ARGS);
		}
	case OP_E1ARGS:		/* eval arguments */
		args = cons(value, args);
		if (is_pair(code)) {	/* continue */
			s_save(OP_E1ARGS, args, cdr(code));
			code = car(code);
			args = NIL;
			s_goto(OP_EVAL);
		} else {	/* end */
			args = reverse(args);
			code = car(args);
			args = cdr(args);
			s_goto(OP_APPLY);
		}
	case OP_DOMACRO:	/* do macro */
		code = value;
		s_goto(OP_EVAL);
	}
}

cell *op_apply(short op) {
	cell	*x, *y;
	char	*e;

	switch (op) {
	case OP_PAPPLY:		/* apply */
		typechk_v("apply", car(args), T_PROC);
		if (cdr(args) == NIL)
			error_0("apply", wrong_args);
		code = car(args);
		if (cddr(args) != NIL) {
			y = cdr(args);
			for (x = cdr(y); cdr(x) != NIL; x = cdr(x))
				y = cdr(y);
			cdr(y) = car(x);
			x = car(x);
			args = cdr(args);
		}
		else
			x = args = cadr(args);
		if (x != NIL && !is_pair(x))
			error_0("apply", "argument list expected");
		s_goto(OP_APPLY);
	case OP_APPLY:		/* apply */
		if (is_proc(code)) {
			s_goto(proc_id(code));	/* primitive */
		} else if (is_closure(code)) {	/* closure */
			/* make environment */
			envir = cons(NIL, closure_env(code));
			for (	x = car(closure_code(code)), y = args;
				is_pair(x);
				x = cdr(x), y = cdr(y)
			) {
				if (y == NIL) {
					error_0("(apply)",
						"too few arguments");
				} else {
					car(envir) = cons(cons(car(x), car(y)),
							car(envir));
				}
			}
			if (x == NIL) {
				 if (y != NIL) {
				 	error_0("(apply)",
				 		"too many arguments");
				 }
			} else if (is_symbol(x))
				car(envir) = cons(cons(x, y), car(envir));
			else {
				error_0("(apply)", "bad argument list");
			}
			code = cdr(closure_code(code));
			args = NIL;
			s_goto(OP_BEGIN);
		} else if (is_cont(code)) {	/* continuation */
			dump = cont_dump(code);
			s_return(args != NIL ? car(args) : NIL);
		} else {
			error_0("eval", "invalid function");
		}
	}
}

cell *op_expmac(short op) {
	char	*e;

	switch (op) {
	case OP_EXP0MAC:	/* expand-macro */
		typechk_1("expand-macro", args, T_PAIR);
		s_save(OP_EXP1MAC, args, NIL);
		code = caar(args);
		s_goto(OP_EVAL);
	case OP_EXP1MAC:
		if (!is_macro(value))
			s_return(car(args));
		args = cdar(args);
		code = value;
		s_goto(OP_APPLY);
	}
}

cell *op_gensym(short op) {
	char	*e;

	typechk_0("gensym", args);
	do {
		sprintf(gensymbuf, "g%ld", gensym++);
	} while (findsym(gensymbuf) != NIL);
	s_return(mk_symbol(gensymbuf));
}

cell *op_lambda(short op) {
	char	*e;

	if (code == NIL || cdr(code) == NIL)
		error_0("lambda", wrong_args);
	typechk_v("lambda", car(code), T_ARGLST);
	s_return(mk_closure(code, envir));
}

cell *op_quote(short op) {
	char	*e;

	typechk_1("quote", code, 0);
	s_return(car(code));
}

cell *op_def(short op) {
	cell	*x;
	char	*e;

	switch (op) {
	case OP_DEF0:	/* define */
		if (is_pair(car(code))) {
			typechk_v("define", car(code), T_ARGLST);
			x = caar(code);
			code = cons(LAMBDA, cons(cdar(code), cdr(code)));
		} else {
			x = car(code);
			typechk_v("define", x, T_SYMBOL);
			code = cadr(code);
		}
		if (!is_symbol(x))
			error_0("define", "invalid symbol");
		s_save(OP_DEF1, NIL, x);
		s_goto(OP_EVAL);
	case OP_DEF1:	/* define */
		extend(code, value);
		s_return(T);
	}
}

cell *op_set(short op) {
	cell	*x, *y;
	char	*e;

	switch (op) {
	case OP_SET0:		/* set! */
		typechk_2("set!", code, T_SYMBOL, 0);
		s_save(OP_SET1, NIL, car(code));
		code = cadr(code);
		s_goto(OP_EVAL);
	case OP_SET1:		/* set! */
		for (x = envir; x != NIL; x = cdr(x)) {
			for (y = car(x); y != NIL; y = cdr(y))
				if (caar(y) == code)
					break;
			if (y != NIL)
				break;
		}
		if (x != NIL) {
			cdar(y) = value;
			s_return(value);
		} else
			error_1("set!", "unbound symbol:", code);
	}
}

cell *op_begin(short op) {
	if (!is_pair(code)) {
		s_return(code);
	}
	if (cdr(code) != NIL) {
		s_save(OP_BEGIN, NIL, cdr(code));
	}
	code = car(code);
	s_goto(OP_EVAL);
}

cell *op_if(short op) {
	switch (op) {
	case OP_IF0:		/* if */
		if (code == NIL || cdr(code) == NIL)
			error_0("if", wrong_args);
		s_save(OP_IF1, NIL, cdr(code));
		code = car(code);
		s_goto(OP_EVAL);
	case OP_IF1:		/* if */
		if (true(value))
			code = car(code);
		else
			code = cadr(code);	/* (if #f 1) ==> () because
						 * car(NIL) = NIL */
		s_goto(OP_EVAL);
	}
}

cell *op_let(short op) {
	cell	*x, *y;
	char	*e;

	switch (op) {
	case OP_LET0:		/* let */
		args = NIL;
		value = code;
		if (code == NIL || cdr(code) == NIL)
			error_0("let", wrong_args);
		if (is_symbol(car(code))) {
			if (cddr(code) == NIL)
				error_0("let", wrong_args);
			code = cadr(code);
		} else
			code = car(code);
		typechk_v("let", code, T_LETENV);
		s_goto(OP_LET1);
	case OP_LET1:		/* let (eval parameters) */
		args = cons(value, args);
		if (is_pair(code)) {	/* continue */
			s_save(OP_LET1, args, cdr(code));
			code = cadar(code);
			args = NIL;
			s_goto(OP_EVAL);
		} else {	/* end */
			args = reverse(args);
			code = car(args);
			args = cdr(args);
			s_goto(OP_LET2);
		}
	case OP_LET2:		/* let */
		envir = cons(NIL, envir);
		for (x = is_symbol(car(code))? cadr(code): car(code), y = args;
		     y != NIL;
		     x = cdr(x), y = cdr(y)
		    )
			car(envir) = cons(cons(caar(x), car(y)), car(envir));
		if (is_symbol(car(code))) {	/* named let */
			for (x = cadr(code), args = NIL; x != NIL; x = cdr(x))
				args = cons(caar(x), args);
			x = mk_closure(cons(reverse(args), cddr(code)), envir);
			car(envir) = cons(cons(car(code), x), car(envir));
			code = cddr(code);
			args = NIL;
		} else {
			code = cdr(code);
			args = NIL;
		}
		s_goto(OP_BEGIN);
	}
}

cell *op_letseq(short op) {
	char	*e;

	switch (op) {
	case OP_LET0SEQ:	/* let* */
		if (code == NIL || cdr(code) == NIL)
			error_0("let*", wrong_args);
		typechk_v("let*", car(code), T_LETENV);
		if (car(code) == NIL) {
			envir = cons(NIL, envir);
			code = cdr(code);
			s_goto(OP_BEGIN);
		}
		s_save(OP_LET1SEQ, cdr(code), car(code));
		code = cadaar(code);
		s_goto(OP_EVAL);
	case OP_LET1SEQ:	/* let* (make new frame) */
		envir = cons(NIL, envir);
		s_goto(OP_LET2SEQ);
	case OP_LET2SEQ:	/* let* (eval parameters) */
		car(envir) = cons(cons(caar(code), value), car(envir));
		code = cdr(code);
		if (is_pair(code)) {	/* continue */
			s_save(OP_LET2SEQ, args, code);
			code = cadar(code);
			args = NIL;
			s_goto(OP_EVAL);
		} else {	/* end */
			code = args;
			args = NIL;
			s_goto(OP_BEGIN);
		}
	}
}

cell *op_letrec(short op) {
	cell	*x, *y;
	char	*e;

	switch (op) {
	case OP_LET0REC:	/* letrec */
		if (code == NIL || cdr(code) == NIL)
			error_0("letrec", wrong_args);
		typechk_v("letrec", car(code), T_LETENV);
		envir = cons(NIL, envir);
		args = NIL;
		value = code;
		code = car(code);
		s_goto(OP_LET1REC);
	case OP_LET1REC:	/* letrec (eval parameters) */
		args = cons(value, args);
		if (is_pair(code)) {	/* continue */
			s_save(OP_LET1REC, args, cdr(code));
			code = cadar(code);
			args = NIL;
			s_goto(OP_EVAL);
		} else {	/* end */
			args = reverse(args);
			code = car(args);
			args = cdr(args);
			s_goto(OP_LET2REC);
		}
	case OP_LET2REC:	/* letrec */
		for (x = car(code), y = args; y != NIL; x = cdr(x), y = cdr(y))
			car(envir) = cons(cons(caar(x), car(y)), car(envir));
		code = cdr(code);
		args = NIL;
		s_goto(OP_BEGIN);
	}
}

cell *op_cond(short op) {
	cell	*x;
	char	*e;

	switch (op) {
	case OP_COND0:		/* cond */
		for (x = code; x != NIL && is_pair(x); x = cdr(x))
			typechk_v("cond", car(x), T_PAIR);
		s_save(OP_COND1, NIL, code);
		code = caar(code);
		s_goto(OP_EVAL);
	case OP_COND1:		/* cond */
		if (true(value)) {
			if ((code = cdar(code)) == NIL) {
				s_return(value);
			}
			s_goto(OP_BEGIN);
		} else {
			if ((code = cdr(code)) == NIL) {
				s_return(NIL);
			} else {
				s_save(OP_COND1, NIL, code);
				code = caar(code);
				s_goto(OP_EVAL);
			}
		}
	}
}

cell *op_and(short op) {
	switch (op) {
	case OP_AND0:		/* and */
		if (code == NIL)
			s_return(T);
		s_save(OP_AND1, NIL, cdr(code));
		code = car(code);
		s_goto(OP_EVAL);
	case OP_AND1:		/* and */
		if (false(value)) {
			s_return(value);
		} else if (code == NIL) {
			s_return(value);
		} else {
			s_save(OP_AND1, NIL, cdr(code));
			code = car(code);
			s_goto(OP_EVAL);
		}
	}
}

cell *op_or(short op) {
	switch (op) {
	case OP_OR0:		/* or */
		if (code == NIL)
			s_return(F);
		s_save(OP_OR1, NIL, cdr(code));
		code = car(code);
		s_goto(OP_EVAL);
	case OP_OR1:		/* or */
		if (true(value)) {
			s_return(value);
		} else if (code == NIL) {
			s_return(value);
		} else {
			s_save(OP_OR1, NIL, cdr(code));
			code = car(code);
			s_goto(OP_EVAL);
		}
	}
}

cell *op_defmac(short op) {
	cell	*x;

	switch (op) {
	case OP_0DEFMAC:	/* define-macro */
		x = car(code);
		code = cadr(code);
		if (!is_symbol(x))
			error_0("define-macro", "invalid symbol");
		s_save(OP_1DEFMAC, NIL, x);
		s_goto(OP_EVAL);
	case OP_1DEFMAC:	/* define-macro */
		type(value) |= T_MACRO;
		extend(code, value);
		s_return(T);
	}
}

cell *op_case(short op) {
	cell	*x, *y;

	switch (op) {
	case OP_CASE0:		/* case */
		if (code == NIL)
			error_0("case", wrong_args);
		s_save(OP_CASE1, NIL, cdr(code));
		code = car(code);
		s_goto(OP_EVAL);
	case OP_CASE1:		/* case */
		for (x = code; x != NIL; x = cdr(x)) {
			if (!is_pair(y = caar(x)))
				break;
			for ( ; y != NIL; y = cdr(y))
				if (eqv(car(y), value))
					break;
			if (y != NIL)
				break;
		}
		if (x != NIL) {
			if (is_pair(caar(x))) {
				code = cdar(x);
				s_goto(OP_BEGIN);
			} else { /* else */
				s_save(OP_CASE2, NIL, cdar(x));
				code = caar(x);
				s_goto(OP_EVAL);
			}
		} else {
			s_return(NIL);
		}
	case OP_CASE2:		/* case */
		if (true(value)) {
			s_goto(OP_BEGIN);
		} else {
			s_return(NIL);
		}
	}
}

cell *op_callcc(short op) {
	char	*e;

	typechk_1("call/cc", args, T_PROC);
	code = car(args);
	args = cons(mk_continuation(dump), NIL);
	s_goto(OP_APPLY);
}

char	expnum[] = "expected number";

cell *op_add(short op) {
	cell	*x;
	long	v, w;

	for (x = args, v = 0; x != NIL; x = cdr(x)) {
		if (!is_int(car(x)))
			error_0("+", expnum);
		w = ivalue(car(x));
		if ((v<0) == (w<0) && (v<0) != ((v+w)<0))
			error_0("+", "overflow");
		v += w;
	}
	s_return(mk_number(v));
}

cell *op_sub(short op) {
	cell	*x;
	long	v, w;

	if (args == NIL)
		error_0("-", wrong_args);
	if (cdr(args) == NIL) {
		if (!is_int(car(args)))
			error_0("-", expnum);
		v = -ivalue(car(args));
	} else {
		for (	x = cdr(args), v = ivalue(car(args));
			x != NIL;
			x = cdr(x)
		) {
			if (!is_int(car(x)))
				error_0("-", expnum);
			w = ivalue(car(x));
			if (v<0 && w>0 && (v-w)>=0)
				error_0("-", "overflow");
			v -= w;
		}
	}
	s_return(mk_number(v));
}

cell *op_mul(short op) {
	cell	*x;
	long	v, w, o;

	for (x = args, v = 1; x != NIL; x = cdr(x)) {
		if (!is_int(car(x)))
			error_0("*", expnum);
		w = ivalue(car(x));
		o = v;
		v *= w;
		if (w && v/w != o)
			error_0("*", "overflow");
	}
	s_return(mk_number(v));
}

cell *op_div(short op) {
	long	v, w;
	char	*e;

	typechk_2("quotient", args, T_INT, T_INT);
	w = ivalue(cadr(args));
	if (!w) error_0("quotient", "divide by zero");
	v = ivalue(car(args)) / w;
	s_return(mk_number(v));
}

cell *op_rem(short op) {
	long	v, w;
	char	*e;

	typechk_2("remainder", args, T_INT, T_INT);
	w = ivalue(cadr(args));
	if (!w) error_0("remainder", "divide by zero");
	v = ivalue(car(args)) % w;
	s_return(mk_number(v));
}

cell *op_abs(short op) {
	char	*e;

	typechk_1("abs", args, T_INT);
	s_return(mk_number(abs(ivalue(car(args)))));
}

cell *op_even(short op) {
	char	*e;

	typechk_1("even?", args, T_INT);
	s_return(ivalue(car(args)) % 2? F: T);
}

cell *op_odd(short op) {
	char	*e;

	typechk_1("odd?", args, T_INT);
	s_retbool(ivalue(car(args)) % 2);
}

cell *minmax(char *name, int max) {
	cell	*x;
	long	k, i;

	if (args == NIL)
		error_0(name, wrong_args);
	k = ivalue(car(args));
	for (x = args; x != NIL; x = cdr(x)) {
		if (!is_int(car(x)))
			error_1(name, "integer expected", car(x));
		i = ivalue(car(x));
		if (max && i > k || !max && i < k)
			k = i;
	}
	s_return(mk_number(k));
}

cell *op_min(short op) { return minmax("min", 0); }
cell *op_max(short op) { return minmax("max", 1); }

cell *op_expt(short op) {
	long	x, v, w, o;
	char	*e;

	typechk_2("expt", args, T_INT, T_INT);
	v = ivalue(car(args));
	w = ivalue(cadr(args));
	if (w < 0) error_0("expt", "exponent is negative");
	x = 1;
	while (w--) {
		o = x;
		x *= v;
		if (x / v != o) error_0("expt", "overflow");
	}
	s_return(mk_number(x));
}

long gcd(long v, long w) {
	long	t;

	while (v && w) {
		if (v < w)
			w %= v;
		else {
			t = w;
			w = v % w;
			v = t;
		}
	}
	return v? v: w;
}

cell *op_gcd(short op) {
	char	*e;

	typechk_2("gcd", args, T_INT, T_INT);
	s_return(mk_number(gcd(labs(ivalue(car(args))),
				labs(ivalue(cadr(args))))));
}

cell *op_lcm(short op) {
	long	v, w, d, lcm;
	char	*e;

	typechk_2("lcm", args, T_INT, T_INT);
	v = labs(ivalue(car(args)));
	w = labs(ivalue(cadr(args)));
	d = gcd(v, w);
	lcm = d * v/d * w/d;
	if (lcm / (v/d) / (w/d) != d)
		error_0("lcm", "overflow");
	s_return(mk_number(lcm));
}

cell *carcdr(char *name, char *op) {
	cell	*x = car(args);

	if (args == NIL || cdr(args) != NIL)
		error_0(name, wrong_args);
	while (*op) {
		if (!is_pair(x)) error_0(name, "pair expected");
		x = *op++ == 'a'? car(x): cdr(x);
	}
	s_return(x);
}

cell *op_car(short op)   { return carcdr("car", "a"); }
cell *op_cdr(short op)   { return carcdr("cdr", "d"); }
cell *op_caar(short op)  { return carcdr("caar", "aa"); }
cell *op_cadr(short op)  { return carcdr("cadr", "da"); }
cell *op_cdar(short op)  { return carcdr("cdar", "ad"); }
cell *op_cddr(short op)  { return carcdr("cddr", "dd"); }
cell *op_caaar(short op) { return carcdr("caaar", "aaa"); }
cell *op_caadr(short op) { return carcdr("caadr", "daa"); }
cell *op_cadar(short op) { return carcdr("cadar", "ada"); }
cell *op_caddr(short op) { return carcdr("caddr", "dda"); }
cell *op_cdaar(short op) { return carcdr("cdaar", "aad"); }
cell *op_cdadr(short op) { return carcdr("cdadr", "dad"); }
cell *op_cddar(short op) { return carcdr("cddar", "add"); }
cell *op_cdddr(short op) { return carcdr("cdddr", "ddd"); }

cell *listref(char *name, int ref) {
	char	*e, range[] = "index out of range";
	long	n;
	cell	*x;

	typechk_2(name, args, T_LIST, T_INT);
	for (x = car(args), n = ivalue(cadr(args)); n; n--) {
		if (!is_pair(x))
			error_0(name, range);
		x = cdr(x);
	}
	if (ref && x == NIL) error_0(name, range);
	s_return(ref? car(x): x);
}

cell *op_lsttail(short op) { return listref("list-tail", 0); }
cell *op_lstref(short op)  { return listref("list-ref", 1); }

cell *memqv(char *name, int eq) {
	char	*e;
	cell	*x, *y;

	typechk_2(name, args, 0, T_LIST);
	y = car(args);
	for (x = cadr(args); is_pair(x); x = cdr(x)) {
		if (eq && car(x) == y || !eq && eqv(car(x), y))
			s_return(x);
	}
	s_return(F);
}

cell *op_memq(short op) { return memqv("memq", 1); }
cell *op_memv(short op) { return memqv("memv", 0); }

cell *assqv(char *name, int eq) {
	char	*e;
	cell	*x, *y;

	typechk_2(name, args, 0, T_LIST);
	y = car(args);
	for (x = cadr(args); is_pair(x); x = cdr(x)) {
		if (!is_pair(car(x)))
			error_1(name, "bad alist member", car(x));
		if (eq && caar(x) == y || !eq && eqv(caar(x), y))
			s_return(car(x));
	}
	s_return(F);
}

cell *op_assq(short op) { return assqv("assq", 1); }
cell *op_assv(short op) { return assqv("assv", 0); }

cell *op_cons(short op) {
	char	*e;

	typechk_2("cons", args, 0, 0);
	cdr(args) = cadr(args);
	s_return(args);
}

cell *op_setcar(short op) {
	char	*e;

	typechk_2("set-car!", args, T_PAIR, 0);
	caar(args) = cadr(args);
	s_return(car(args));
}

cell *op_setcdr(short op) {
	char	*e;

	typechk_2("set-cdr!", args, T_PAIR, 0);
	cdar(args) = cadr(args);
	s_return(car(args));
}

cell *op_not(short op) {
	char	*e;

	typechk_1("not", args, 0);
	s_retbool(false(car(args)));
}

cell *op_bool(short op) {
	char	*e;

	typechk_1("boolean?", args, 0);
	s_retbool(car(args) == F || car(args) == T);
}

cell *op_null(short op) {
	char	*e;

	typechk_1("null?", args, 0);
	s_retbool(car(args) == NIL);
}

cell *op_zerop(short op) {
	char	*e;

	typechk_1("zero?", args, T_INT);
	s_retbool(ivalue(car(args)) == 0);
}

cell *op_posp(short op) {
	char	*e;

	typechk_1("positive?", args, T_INT);
	s_retbool(ivalue(car(args)) > 0);
}

cell *op_negp(short op) {
	char	*e;

	typechk_1("negative?", args, T_INT);
	s_retbool(ivalue(car(args)) < 0);
}

cell *op_numeq(short op) {
	cell	*x;
	long	v, v2;
	char	*s;

	switch (op) {
	case OP_NUMEQ:		/* = */
		sub_op = op;
		s_goto(OP_NUMPRED);
	case OP_NUMPRED:
		switch (sub_op) {
		case OP_NUMEQ:	s = "="; break;
		case OP_LESS:	s = "<"; break;
		case OP_GRE:	s = ">"; break;
		case OP_LEQ:	s = "<="; break;
		case OP_GEQ:	s = ">="; break;
		default:	error_0("internal", "bad numeric predicate");
		}
		if (args == NIL || cdr(args) == NIL)
			error_0(s, "too few arguments");
		v = ivalue(car(args));
		for (x = cdr(args); is_pair(x); x = cdr(x)) {
			if (!is_int(car(x)))
				error_0(s, "expected integer");
			v2 = ivalue(car(x));
			switch (sub_op) {
			case OP_NUMEQ:	if (v != v2) s_return(F); break;
			case OP_LESS:	if (v >= v2) s_return(F); break;
			case OP_GRE:	if (v <= v2) s_return(F); break;
			case OP_LEQ:	if (v > v2) s_return(F); break;
			case OP_GEQ:	if (v < v2) s_return(F); break;
			}
			v = v2;
		}
		s_return(T);
	}
}

cell *op_less(short op) {
	sub_op = op;
	s_goto(OP_NUMPRED);
}

cell *op_gre(short op) {
	sub_op = op;
	s_goto(OP_NUMPRED);
}

cell *op_leq(short op) {
	sub_op = op;
	s_goto(OP_NUMPRED);
}

cell *op_geq(short op) {
	sub_op = op;
	s_goto(OP_NUMPRED);
}

cell *op_symbol(short op) {
	char	*e;

	typechk_1("symbol?", args, 0);
	s_retbool(is_symbol(car(args)));
}

cell *op_number(short op) {
	char	*e;

	typechk_1("number?", args, 0);
	s_retbool(is_int(car(args)));
}

cell *op_string(short op) {
	char	*e;

	typechk_1("string?", args, 0);
	s_retbool(is_string(car(args)));
}

cell *op_proc(short op) {
	char	*e;

	typechk_1("procedure?", args, 0);
	s_retbool(is_proc(car(args)) ||
		  is_closure(car(args)) ||
		  is_cont(car(args)));
}

cell *op_pair(short op) {
	char	*e;

	typechk_1("pair?", args, 0);
	s_retbool(is_pair(car(args)));
}

cell *op_list(short op) {
	char	*e;
	cell	*x, *y;
	int	lst = 1;

	x = car(args);
	if (x == NIL)
		;
	else if (!is_pair(x))
		lst = 0;
	else {
		for (y = cdr(x); x != y; x = cddr(x), y = cdr(y))
			if (!is_pair(x) || !is_pair(cdr(x)))
				break;
		lst = x == NIL || cdr(x) == NIL;
	}
	s_retbool(lst);
}

cell *op_eq(short op) {
	char	*e;

	typechk_2("eq?", args, 0, 0);
	s_retbool(car(args) == cadr(args));
}

cell *op_eqv(short op) {
	char	*e;

	typechk_2("eqv?", args, 0, 0);
	s_retbool(eqv(car(args), cadr(args)));
}

long length(cell *x) {
	long	v;

	for (v = 0; is_pair(x); x = cdr(x))
		++v;
	return v;
}

#define	one_arg(x)	(is_pair(x) && cdr(x) == NIL)

cell *op_write(short op) {
	char	*e;

	switch (op) {
	case OP_WRITE:		/* write */
		typechk_1("write", args, 0);
		print_flag = 1;
		args = car(args);
		s_goto(OP_P0LIST);
	case OP_P0LIST:
		if (!is_pair(args)) {
			printatom(args, print_flag);
			s_return(T);
		} else if (car(args) == QUOTE && one_arg(cdr(args))) {
			fprintf(outfp, "'");
			args = cadr(args);
			s_goto(OP_P0LIST);
		} else if (car(args) == QQUOTE && one_arg(cdr(args))) {
			fprintf(outfp, "`");
			args = cadr(args);
			s_goto(OP_P0LIST);
		} else if (car(args) == UNQUOTE && one_arg(cdr(args))) {
			fprintf(outfp, ",");
			args = cadr(args);
			s_goto(OP_P0LIST);
		} else if (car(args) == UNQUOTESP && one_arg(cdr(args))) {
			fprintf(outfp, ",@");
			args = cadr(args);
			s_goto(OP_P0LIST);
		} else {
			fprintf(outfp, "(");
			s_save(OP_P1LIST, cdr(args), NIL);
			args = car(args);
			s_goto(OP_P0LIST);
		}
	case OP_P1LIST:
		if (is_pair(args)) {
			s_save(OP_P1LIST, cdr(args), NIL);
			fprintf(outfp, " ");
			args = car(args);
			s_goto(OP_P0LIST);
		} else {
			if (args != NIL) {
				fprintf(outfp, " . ");
				printatom(args, print_flag);
			}
			fprintf(outfp, ")");
			s_return(T);
		}

	}
}

cell *op_display(short op) {
	char	*e;

	typechk_1("display", args, 0);
	print_flag = 0;
	args = car(args);
	s_goto(OP_P0LIST);
}

cell *op_newline(short op) {
	char	*e;

	typechk_0("newline", args);
	fprintf(outfp, "\n");
	s_return(T);
}

cell *op_error(short op) {
	switch (op) {
	case OP_ERROR:
		if (args == NIL)
			error_0("error", wrong_args);
		args = cons(mk_string("error"), args);
		s_goto(OP_ERR0);
	case OP_ERR0:		/* error */
		if (args == NIL || cdr(args) == NIL)
			error_0("error", wrong_args);
		if (!is_string(car(args)) || !is_string(cadr(args))) {
			fatal("error handler: expected strings");
		}
		printf("error: ");
		printf("%s: %s", string(car(args)),
				string(cadr(args)));
		args = cddr(args);
		s_goto(OP_ERR1);
	case OP_ERR1:		/* error */
		printf(" ");
		if (args != NIL) {
			s_save(OP_ERR1, cdr(args), NIL);
			args = car(args);
			print_flag = 1;
			s_goto(OP_P0LIST);
		} else {
			printf("\n");
			flushinput();
			s_goto(OP_T0LVL);
		}
	}
}

cell *op_reverse(short op) {
	char	*e;

	typechk_1("reverse", args, T_LIST);
	s_return(reverse(car(args)));
}

cell *r_append(cell *args) {
	cell	*x;

	if (args == NIL)
		return NIL;
	else if (cdr(args) == NIL)
		return car(args);
	else {
		if (car(args) != NIL && !is_pair(car(args)))
			return NULL;
		if ((x = r_append(cdr(args))) == NULL)
			return NULL;
		return append(car(args), x);
	}
}

cell *op_append(short op) {
	cell	*x;

	if ((x = r_append(args)) == NULL)
		error_0("append", "list expected");
	s_return(x);
}

cell *op_length(short op) {
	char	*e;

	typechk_1("length", args, T_LIST);
	s_return(mk_number(length(car(args))));
}

cell *op_winfile(short op) {
	char	*e;
	FILE	*f;
	char	winf[] = "with-input-from-file";

	switch (op) {
	case OP_W0INFILE:
		typechk_2(winf, args, T_STRING, T_PROC);
		if ((f = fopen(string(car(args)), "r")) == NULL)
			error_1(winf, "could not open file:", car(args));
		s_save(OP_W1INFILE, mk_number((long) infp), NIL);
		infp = f;
		code = cons(cadr(args), NIL);
		s_goto(OP_EVAL);
	case OP_W1INFILE:
		fclose(infp);
		infp = (FILE *) ivalue(args);
		s_return(value);
	}
}

cell *op_woutfile(short op) {
	char	*e;
	FILE	*f;
	char	woutf[] = "with-output-to-file";

	switch (op) {
	case OP_W0OUTFILE:
		typechk_2(woutf, args, T_STRING, T_PROC);
#ifdef PRESERVE_FILES
		if ((f = fopen(string(car(args)), "r")) != NULL) {
			fclose(f);
			error_1(woutf, "file exists:", car(args));
		}
#endif
		if ((f = fopen(string(car(args)), "w")) == NULL)
			error_1(woutf, "could not open file:", car(args));
		s_save(OP_W1OUTFILE, mk_number((long) outfp), NIL);
		outfp = f;
		code = cons(cadr(args), NIL);
		s_goto(OP_EVAL);
	case OP_W1OUTFILE:
		fclose(outfp);
		outfp = (FILE *) ivalue(args);
		s_return(T);
	}
}

cell *op_eofobj(short op) {
	char	*e;

	typechk_1("eof-object?", args, 0);
	s_retbool(car(args) == EOFOBJ);
}

cell *op_rdchr(short op) {
	char	*e;
	int	c;

	typechk_0("read-char", args);
	c = fgetc(infp);
	s_return(feof(infp)? EOFOBJ: mk_char(c));
}

cell *op_pkchr(short op) {
	char	*e;
	int	c;

	typechk_0("peek-char", args);
	c = fgetc(infp);
	ungetc(c, infp);
	s_return(feof(infp)? EOFOBJ: mk_char(c));
}

cell *op_wrchr(short op) {
	char	*e;

	typechk_1("write-char", args, T_CHAR);
	fputc(cvalue(car(args)), outfp);
	s_return(T);
}

cell *op_char(short op) {
	char	*e;

	typechk_1("char?", args, 0);
	s_retbool(is_char(car(args)));
}

cell *op_chreq(short op) {
	cell	*x;
	char	*s;
	int	c, c2;

	switch (op) {
	case OP_CHREQ:		/* char=? */
		sub_op = op;
		s_goto(OP_CHRPRED);
	case OP_CHRPRED:
		switch (sub_op) {
		case OP_CHREQ:	s = "char=?"; break;
		case OP_CHRLT:	s = "char<?"; break;
		case OP_CHRGT:	s = "char>?"; break;
		case OP_CHRLE:	s = "char<=?"; break;
		case OP_CHRGE:	s = "char>=?"; break;
		case OP_CHREQI:	s = "char=ci=?"; break;
		case OP_CHRLTI:	s = "char=ci<?"; break;
		case OP_CHRGTI:	s = "char=ci>?"; break;
		case OP_CHRLEI:	s = "char=ci<=?"; break;
		case OP_CHRGEI:	s = "char=ci>=?"; break;
		default:	error_0("internal", "bad char predicate");
		}
		if (args == NIL || cdr(args) == NIL)
			error_0(s, "too few arguments");
		c = cvalue(car(args));
		/* assert: CHRxxI > CHRxx */
		if (sub_op >= OP_CHREQI) c = tolower(c);
		for (x = cdr(args); is_pair(x); x = cdr(x)) {
			if (!is_char(car(x)))
				error_0(s, "expected char");
			c2 = cvalue(car(x));
			if (sub_op >= OP_CHREQI) c2 = tolower(c2);
			switch (sub_op) {
			case OP_CHREQI:
			case OP_CHREQ:	if (c != c2) s_return(F); break;
			case OP_CHRLTI:
			case OP_CHRLT:	if (c >= c2) s_return(F); break;
			case OP_CHRGTI:
			case OP_CHRGT:	if (c <= c2) s_return(F); break;
			case OP_CHRLEI:
			case OP_CHRLE:	if (c > c2) s_return(F); break;
			case OP_CHRGEI:
			case OP_CHRGE:	if (c < c2) s_return(F); break;
			}
			c = c2;
		}
		s_return(T);
	}
}

cell *op_chrlt(short op)  { sub_op = op; s_goto(OP_CHRPRED); }
cell *op_chrgt(short op)  { sub_op = op; s_goto(OP_CHRPRED); }
cell *op_chrle(short op)  { sub_op = op; s_goto(OP_CHRPRED); }
cell *op_chrge(short op)  { sub_op = op; s_goto(OP_CHRPRED); }
cell *op_chreqi(short op) { sub_op = op; s_goto(OP_CHRPRED); }
cell *op_chrlti(short op) { sub_op = op; s_goto(OP_CHRPRED); }
cell *op_chrgti(short op) { sub_op = op; s_goto(OP_CHRPRED); }
cell *op_chrlei(short op) { sub_op = op; s_goto(OP_CHRPRED); }
cell *op_chrgei(short op) { sub_op = op; s_goto(OP_CHRPRED); }

cell *op_chralph(short op) {
	char	*e;

	typechk_1("char-alphabetic?", args, T_CHAR);
	s_retbool(isalpha(cvalue(car(args))));
}

cell *op_chrnum(short op) {
	char	*e;

	typechk_1("char-numeric?", args, T_CHAR);
	s_retbool(isdigit(cvalue(car(args))));
}

cell *op_chrwhit(short op) {
	char	*e;

	typechk_1("char-whitespace?", args, T_CHAR);
	s_retbool(strchr(" \n\r\t\f", cvalue(car(args))));
}

cell *op_chrupp(short op) {
	char	*e;

	typechk_1("char-upper-case?", args, T_CHAR);
	s_retbool(isupper(cvalue(car(args))));
}

cell *op_chrlow(short op) {
	char	*e;

	typechk_1("char-lower-case?", args, T_CHAR);
	s_retbool(islower(cvalue(car(args))));
}

cell *op_chrdnc(short op) {
	char	*e;

	typechk_1("char-downcase", args, T_CHAR);
	s_return(mk_char(tolower(cvalue(car(args)))));
}

cell *op_chrupc(short op) {
	char	*e;

	typechk_1("char-upcase", args, T_CHAR);
	s_return(mk_char(toupper(cvalue(car(args)))));
}

cell *op_chrint(short op) {
	char	*e;

	typechk_1("char->integer", args, T_CHAR);
	s_return(mk_number(cvalue(car(args))));
}

cell *op_intchr(short op) {
	char	*e;
	long	i;

	typechk_1("integer->char", args, T_INT);
	i = ivalue(car(args));
	if (i < 0 || i > 127)
		error_1("integer->char", "value out of range",
			car(args));
	s_return(mk_char(i));
}

cell *op_mkstr(short op) {
	cell	*x;
	char	*e, *s, name[] = "make-string";
	long	i;
	int	c = ' ';

	if (cdr(args) == NIL)
		typechk_1(name, args, T_INT);
	else {
		typechk_2(name, args, T_INT, T_CHAR);
		c = cvalue(cadr(args));
	}
	i = ivalue(car(args));
	if (i < 0)
		error_0("make-string", "expected non-negative value");
	s = string(x = mk_string2("", i));
	memset(s, c, i);
	s[i] = 0;
	s_return(x);
}

cell *op_streq(short op) {
	char	*e;

	typechk_2("string=?", args, T_STRING, T_STRING);
	s_retbool(!strcmp(string(car(args)), string(cadr(args))));
}

cell *op_strlt(short op) {
	char	*e;

	typechk_2("string<?", args, T_STRING, T_STRING);
	s_retbool(strcmp(string(car(args)), string(cadr(args))) < 0);
}

cell *op_strlen(short op) {
	char	*e;

	typechk_1("string-length", args, T_STRING);
	s_return(mk_number(strlen(string(car(args)))));
}

cell *op_streqi(short op) {
	char	*e;

	typechk_2("string-ci=?", args, T_STRING, T_STRING);
	s_retbool(!strcmp_i(string(car(args)), string(cadr(args))));
}

cell *op_strlti(short op) {
	char	*e;

	typechk_2("string-ci<?", args, T_STRING, T_STRING);
	s_retbool(strcmp_i(string(car(args)), string(cadr(args))) < 0);
}

cell *op_strapnd(short op) {
	cell	*x, *str;
	char	*e;
	long	i, k;

	for (i = 0, x = args; x != NIL; x = cdr(x)) {
		if (!is_string(car(x)))
			error_0("string-append", "string expected");
		i += (long) strlen(string(car(x)));
	}
	str = mk_string2("", i);
	for (k = i = 0, x = args; x != NIL; x = cdr(x)) {
		strcpy(&string(str)[k], string(car(x)));
		k += strlen(string(car(x)));
	}
	s_return(str);
}

char	range[] = "index out of range";

cell *op_strref(short op) {
	char	*e, *s;
	long	i;

	typechk_2("string-ref", args, T_STRING, T_INT);
	s = string(car(args));
	i = ivalue(cadr(args));
	if (i < 0 || i >= (long) strlen(s))
		error_0("string-ref", range);
	s_return(mk_char(s[i]));
}

cell *op_strset(short op) {
	char	*e, *s;
	long	i;

	typechk_3("string-set!", args, T_STRING, T_INT, T_CHAR);
	s = string(car(args));
	i = ivalue(cadr(args));
	if (i < 0 || i >= (long) strlen(s))
		error_0("string-set!", range);
	s[i] = cvalue(caddr(args));
	s_return(NIL);
}

cell *op_substr(short op) {
	cell	*x;
	char	*e;
	long	i, j, k;

	typechk_3("substring", args, T_STRING, T_INT, T_INT);
	k = (long) strlen(string(car(args)));
	i = ivalue(cadr(args));
	j = ivalue(caddr(args));
	if (i < 0 || i > k || j < i || j > k)
		error_0("substring", range);
	x = mk_string2("", j-i);
	memcpy(string(x), string(car(args))+i, j);
	string(x)[j-i] = 0;
	s_return(x);
}

cell *op_lststr(short op) {
	cell	*x, *y;
	char	*e, *s;
	long	i;

	typechk_1("list->string", args, T_LIST);
	x = car(args);
	y = mk_string2("", length(x));
	s = string(y);
	for (i=0; is_pair(x); i++, x = cdr(x)) {
		if (!is_char(car(x)))
			error_0("list->string",
				"expected list of char");
		s[i] = cvalue(car(x));
	}
	s[i] = 0;
	s_return(y);
}

cell *op_strlst(short op) {
	char	*e, *s;
	long	i;

	typechk_1("string->list", args, T_STRING);
	args = cons(NIL, args);	/* gc protect */
	for (i = (long) strlen(string(cadr(args)))-1; i>=0; i--) {
		s = string(cadr(args));
		car(args) = cons(mk_char(s[i]), car(args));
	}
	s_return(car(args));
}

cell *op_strfil(short op) {
	char	*e, *s;

	typechk_2("string-fill!", args, T_STRING, T_CHAR);
	s = string(car(args));
	memset(s, cvalue(cadr(args)), strlen(s));
	s_return(T);
}

cell *op_strcpy(short op) {
	cell	*x;
	char	*e;

	typechk_1("string-copy", args, T_STRING);
	x = mk_string2("", strlen(string(car(args))));
	strcpy(string(x), string(car(args)));
	s_return(x);
}

cell *op_strsym(short op) {
	char	*e, *s;
	char	sym[256];

	typechk_1("string->symbol", args, T_STRING);
	s = string(car(args));
	if (strlen(s) >= sizeof(sym))
		error_0("string->symbol",
			"implementation restriction: string too long");
	strcpy(sym, s);
	s_return(mk_symbol(s));
}

cell *op_symstr(short op) {
	cell	*x;
	char	*e, *s;

	typechk_1("symbol->string", args, T_SYMBOL);
	s = symname(car(args));
	x = mk_string2("", strlen(symname(car(args))));
	strcpy(string(x), symname(car(args)));
	s_return(x);
}

cell	*(*dispatch_table[])(short) = {
	op_init,	/* OP_INIT */
	op_load,	/* OP_LOAD */
	op_toplvl,	/* OP_T0LVL */
	op_toplvl,	/* OP_T1LVL */
	op_read,	/* OP_READ */
	op_read,	/* OP_RDSEXPR */
	op_read,	/* OP_RDLIST */
	op_read,	/* OP_RDDOT */
	op_read,	/* OP_RDQUOTE */
	op_read,	/* OP_RDQQUOTE */
	op_read,	/* OP_RDUNQUOTE */
	op_read,	/* OP_RDUQTSP */
	op_print,	/* OP_PRINT */
	op_eval,	/* OP_EVAL */
	op_eval,	/* OP_E0ARGS */
	op_eval,	/* OP_E1ARGS */
	op_eval,	/* OP_DOMACRO */
	op_eval,	/* OP_PEVAL */
	op_apply,	/* OP_APPLY */
	op_apply,	/* OP_PAPPLY */
	op_expmac,	/* OP_EXP0MAC */
	op_expmac,	/* OP_EXP1MAC */
	op_gensym,	/* OP_GENSYM */
	op_lambda,	/* OP_LAMBDA */
	op_quote,	/* OP_QUOTE */
	op_def,		/* OP_DEF0 */
	op_def,		/* OP_DEF1 */
	op_set,		/* OP_SET0 */
	op_set,		/* OP_SET1 */
	op_begin,	/* OP_BEGIN */
	op_if,		/* OP_IF0 */
	op_if,		/* OP_IF1 */
	op_let,		/* OP_LET0 */
	op_let,		/* OP_LET1 */
	op_let,		/* OP_LET2 */
	op_letseq,	/* OP_LET0SEQ */
	op_letseq,	/* OP_LET1SEQ */
	op_letseq,	/* OP_LET2SEQ */
	op_letrec,	/* OP_LET0REC */
	op_letrec,	/* OP_LET1REC */
	op_letrec,	/* OP_LET2REC */
	op_cond,	/* OP_COND0 */
	op_cond,	/* OP_COND1 */
	op_and,		/* OP_AND0 */
	op_and,		/* OP_AND1 */
	op_or,		/* OP_OR0 */
	op_or,		/* OP_OR1 */
	op_defmac,	/* OP_0DEFMAC */
	op_defmac,	/* OP_1DEFMAC */
	op_case,	/* OP_CASE0 */
	op_case,	/* OP_CASE1 */
	op_case,	/* OP_CASE2 */
	op_callcc,	/* OP_CALLCC */
	op_add,		/* OP_ADD */
	op_sub,		/* OP_SUB */
	op_mul,		/* OP_MUL */
	op_div,		/* OP_DIV */
	op_rem,		/* OP_REM */
	op_abs,		/* OP_ABS */
	op_even,	/* OP_EVEN*/
	op_odd,		/* OP_ODD */
	op_min,		/* OP_MIN */
	op_max,		/* OP_MAX */
	op_expt,	/* OP_EXPT */
	op_gcd,		/* OP_GCD */
	op_lcm,		/* OP_LCM */
	op_car,		/* OP_CAR */
	op_cdr,		/* OP_CDR */
	op_caar,	/* OP_CAAR */
	op_cadr,	/* OP_CADR */
	op_cdar,	/* OP_CDAR */
	op_cddr,	/* OP_CDDR */
	op_caaar,	/* OP_CAAAR */
	op_caadr,	/* OP_CAADR */
	op_cadar,	/* OP_CADAR */
	op_caddr,	/* OP_CADDR */
	op_cdaar,	/* OP_CDAAR */
	op_cdadr,	/* OP_CDADR */
	op_cddar,	/* OP_CDDAR */
	op_cdddr,	/* OP_CDDDR */
	op_cons,	/* OP_CONS */
	op_lsttail,	/* OP_LSTTAIL */
	op_lstref,	/* OP_LSTREF */
	op_memq,	/* OP_MEMQ */
	op_memv,	/* OP_MEMV */
	op_assq,	/* OP_ASSQ */
	op_assv,	/* OP_ASSV */
	op_setcar,	/* OP_SETCAR */
	op_setcdr,	/* OP_SETCDR */
	op_not,		/* OP_NOT */
	op_bool,	/* OP_BOOL */
	op_null,	/* OP_NULL */
	op_zerop,	/* OP_ZEROP */
	op_posp,	/* OP_POSP */
	op_negp,	/* OP_NEGP */
	op_numeq,	/* OP_NUMEQ */
	op_numeq,	/* OP_NUMPRED */
	op_less,	/* OP_LESS */
	op_gre,		/* OP_GRE */
	op_leq,		/* OP_LEQ */
	op_geq,		/* OP_GEQ */
	op_symbol,	/* OP_SYMBOL */
	op_number,	/* OP_NUMBER */
	op_string,	/* OP_STRING */
	op_proc,	/* OP_PROC */
	op_pair,	/* OP_PAIR */
	op_list,	/* OP_LIST */
	op_eq,		/* OP_EQ */
	op_eqv,		/* OP_EQV */
	op_write,	/* OP_WRITE */
	op_write,	/* OP_P0LIST */
	op_write,	/* OP_P1LIST */
	op_display,	/* OP_DISPLAY */
	op_newline,	/* OP_NEWLINE */
	op_error,	/* OP_ERR0 */
	op_error,	/* OP_ERR1 */
	op_error,	/* OP_ERROR */
	op_reverse,	/* OP_REVERSE */
	op_append,	/* OP_APPEND */
	op_length,	/* OP_LENGTH */
	op_winfile,	/* OP_W0INFILE */
	op_winfile,	/* OP_W1INFILE */
	op_woutfile,	/* OP_W0OUTFILE */
	op_woutfile,	/* OP_W1OUTFILE */
	op_eofobj,	/* OP_EOFOBJ */
	op_rdchr,	/* OP_RDCHR */
	op_pkchr,	/* OP_PKCHR */
	op_wrchr,	/* OP_WRCHR */
	op_char,	/* OP_CHAR */
	op_chreq,	/* OP_CHREQ */
	op_chreq,	/* OP_CHRPRED */
	op_chrlt,	/* OP_CHRLT */
	op_chrgt,	/* OP_CHRGT */
	op_chrle,	/* OP_CHRLE */
	op_chrge,	/* OP_CHRGE */
	op_chreqi,	/* OP_CHREQI */
	op_chrlti,	/* OP_CHRLTI */
	op_chrgti,	/* OP_CHRGTI */
	op_chrlei,	/* OP_CHRLEI */
	op_chrgei,	/* OP_CHRGEI */
	op_chralph,	/* OP_CHRALPH */
	op_chrnum,	/* OP_CHRNUM */
	op_chrupp,	/* OP_CHRUPP */
	op_chrlow,	/* OP_CHRLOW */
	op_chrwhit,	/* OP_CHRWHIT */
	op_chrdnc,	/* OP_CHRDNC */
	op_chrupc,	/* OP_CHRUPC */
	op_chrint,	/* OP_CHRINT */
	op_intchr,	/* OP_INTCHR */
	op_mkstr,	/* OP_MKSTR */
	op_streq,	/* OP_STREQ */
	op_streqi,	/* OP_STREQI */
	op_strlt,	/* OP_STRLT */
	op_strlti,	/* OP_STRLTI */
	op_strlen,	/* OP_STRLEN */
	op_strapnd,	/* OP_STRAPND */
	op_strref,	/* OP_STRREF */
	op_strset,	/* OP_STRSET */
	op_substr,	/* OP_SUBSTR */
	op_lststr,	/* OP_LSTSTR */
	op_strlst,	/* OP_STRLST */
	op_strfil,	/* OP_STRFIL */
	op_strcpy,	/* OP_STRCPY */
	op_strsym,	/* OP_STRSYM */
	op_symstr,	/* OP_SYMSTR */
};

/* interpreter loop */

cell *eval_cycle(short op) {
	operator = op;
	for (;;) {
#ifdef USE_SETJMP
		if (intr) {
			intr = 0;
			error("interrupted");
		}
#endif
		if ((*dispatch_table[operator])(operator) == NIL)
			return NIL;
	}
}

/* Initialization */

void mk_syntax(short op, char *name) {
	cell	*x;

	x = cons(mk_string(name), NIL);
	type(x) = (T_SYNTAX | T_SYMBOL);
	syntax_id(x) = op;
	symbols = cons(x, symbols);
}

void mk_proc(short op, char *name) {
	cell	*x, *y;

	x = mk_symbol(name);
	y = new_cell(NIL, NIL);
	type(y) = (T_PROC | T_ATOM);
	ivalue(y) = (long) op;
	car(global_env) = cons(cons(x, y), car(global_env));
}

void init_vars_global(void) {
	cell	*x;

	infp = stdin;
	outfp = stdout;
	type(NIL) = (T_ATOM | MARK);
	car(NIL) = cdr(NIL) = NIL;
	type(T) = (T_ATOM | MARK);
	car(T) = cdr(T) = T;
	type(F) = (T_ATOM | MARK);
	car(F) = cdr(F) = F;
	global_env = cons(NIL, NIL);
	x = mk_symbol("else");
	car(global_env) = cons(cons(x, T), car(global_env));
}

void init_syntax(void) {
	mk_syntax(OP_AND0, "and");
	mk_syntax(OP_BEGIN, "begin");
	mk_syntax(OP_CASE0, "case");
	mk_syntax(OP_COND0, "cond");
	mk_syntax(OP_DEF0, "define");
	mk_syntax(OP_IF0, "if");
	mk_syntax(OP_LAMBDA, "lambda");
	mk_syntax(OP_LET0, "let");
	mk_syntax(OP_LET0SEQ, "let*");
	mk_syntax(OP_LET0REC, "letrec");
	mk_syntax(OP_0DEFMAC, "define-macro");
	mk_syntax(OP_OR0, "or");
	mk_syntax(OP_QUOTE, "quote");
	mk_syntax(OP_SET0, "set!");
}

void init_procs(void) {
	mk_proc(OP_ABS, "abs");			/* nmh */
	mk_proc(OP_ADD, "+");
	mk_proc(OP_APPEND, "append");
	mk_proc(OP_ASSQ, "assq");		/* nmh */
	mk_proc(OP_ASSV, "assv");		/* nmh */
	mk_proc(OP_BOOL, "boolean?");
	mk_proc(OP_CAAAR, "caaar");		/* nmh */
	mk_proc(OP_CAADR, "caadr");		/* nmh */
	mk_proc(OP_CAAR, "caar");		/* nmh */
	mk_proc(OP_CADAR, "cadar");		/* nmh */
	mk_proc(OP_CADDR, "caddr");		/* nmh */
	mk_proc(OP_CADR, "cadr");		/* nmh */
	mk_proc(OP_CALLCC, "call-with-current-continuation");
	mk_proc(OP_CAR, "car");
	mk_proc(OP_CDAAR, "cdaar");		/* nmh */
	mk_proc(OP_CDADR, "cdadr");		/* nmh */
	mk_proc(OP_CDAR, "cdar");		/* nmh */
	mk_proc(OP_CDDAR, "cddar");		/* nmh */
	mk_proc(OP_CDDDR, "cdddr");		/* nmh */
	mk_proc(OP_CDDR, "cddr");		/* nmh */
	mk_proc(OP_CDR, "cdr");
	mk_proc(OP_CHAR, "char?");		/* nmh */
	mk_proc(OP_CHRALPH, "char-alphabetic?");/* nmh */
	mk_proc(OP_CHRDNC, "char-downcase");	/* nmh */
	mk_proc(OP_CHREQ, "char=?");		/* nmh */
	mk_proc(OP_CHREQI, "char-ci=?");	/* nmh */
	mk_proc(OP_CHRGE, "char>=?");		/* nmh */
	mk_proc(OP_CHRGEI, "char-ci>=?");	/* nmh */
	mk_proc(OP_CHRGT, "char>?");		/* nmh */
	mk_proc(OP_CHRGTI, "char-ci>?");	/* nmh */
	mk_proc(OP_CHRINT, "char->integer");	/* nmh */
	mk_proc(OP_CHRLE, "char<=?");		/* nmh */
	mk_proc(OP_CHRLEI, "char-ci<=?");	/* nmh */
	mk_proc(OP_CHRLOW, "char-lower-case?");	/* nmh */
	mk_proc(OP_CHRLT, "char<?");		/* nmh */
	mk_proc(OP_CHRLTI, "char-ci<?");	/* nmh */
	mk_proc(OP_CHRNUM, "char-numeric?");	/* nmh */
	mk_proc(OP_CHRUPC, "char-upcase");	/* nmh */
	mk_proc(OP_CHRUPP, "char-upper-case?");	/* nmh */
	mk_proc(OP_CHRWHIT, "char-whitespace?");/* nmh */
	mk_proc(OP_CONS, "cons");
	mk_proc(OP_DISPLAY, "display");
	mk_proc(OP_DIV, "quotient");
	mk_proc(OP_EOFOBJ, "eof-object?");	/* nmh */
	mk_proc(OP_EQ, "eq?");
	mk_proc(OP_EQV, "eqv?");
	mk_proc(OP_ERROR, "error");
	mk_proc(OP_EVEN, "even?");		/* nmh */
	mk_proc(OP_EXPT, "expt");		/* nmh */
	mk_proc(OP_EXP0MAC, "expand-macro");
	mk_proc(OP_GCD, "gcd");			/* nmh */
	mk_proc(OP_GENSYM, "gensym");
	mk_proc(OP_GEQ, ">=");
	mk_proc(OP_GRE, ">");
	mk_proc(OP_INTCHR, "integer->char");	/* nmh */
	mk_proc(OP_LCM, "lcm");			/* nmh */
	mk_proc(OP_LENGTH, "length");		/* a.k */
	mk_proc(OP_LEQ, "<=");
	mk_proc(OP_LESS, "<");
	mk_proc(OP_LIST, "list?");		/* nmh */
	mk_proc(OP_LOAD, "load");
	mk_proc(OP_LSTTAIL, "list-tail");	/* nmh */
	mk_proc(OP_LSTREF, "list-ref");		/* nmh */
	mk_proc(OP_LSTSTR, "list->string");	/* nmh */
	mk_proc(OP_MEMQ, "memq");		/* nmh */
	mk_proc(OP_MEMV, "memv");		/* nmh */
	mk_proc(OP_MKSTR, "make-string");	/* nmh */
	mk_proc(OP_MUL, "*");
	mk_proc(OP_NEGP, "negative?");
	mk_proc(OP_NEWLINE, "newline");
	mk_proc(OP_NOT, "not");
	mk_proc(OP_NULL, "null?");
	mk_proc(OP_NUMBER, "number?");
	mk_proc(OP_NUMEQ, "=");
	mk_proc(OP_ODD, "odd?");		/* nmh */
	mk_proc(OP_MAX, "max");			/* nmh */
	mk_proc(OP_MIN, "min");			/* nmh */
	mk_proc(OP_PAIR, "pair?");
	mk_proc(OP_PAPPLY, "apply");
	mk_proc(OP_PEVAL, "eval");
	mk_proc(OP_PKCHR, "peek-char");		/* nmh */
	mk_proc(OP_POSP, "positive?");
	mk_proc(OP_PROC, "procedure?");
	mk_proc(OP_RDCHR, "read-char");		/* nmh */
	mk_proc(OP_READ, "read");
	mk_proc(OP_REM, "remainder");
	mk_proc(OP_REVERSE, "reverse");
	mk_proc(OP_SETCAR, "set-car!");
	mk_proc(OP_SETCDR, "set-cdr!");
	mk_proc(OP_STRAPND, "string-append");	/* nmh */
	mk_proc(OP_STRCPY, "string-copy");	/* nmh */
	mk_proc(OP_STREQ, "string=?");		/* nmh */
	mk_proc(OP_STREQI, "string-ci=?");	/* nmh */
	mk_proc(OP_STRFIL, "string-fill!");	/* nmh */
	mk_proc(OP_STRING, "string?");
	mk_proc(OP_STRLEN, "string-length");	/* nmh */
	mk_proc(OP_STRLST, "string->list");	/* nmh */
	mk_proc(OP_STRLT, "string<?");		/* nmh */
	mk_proc(OP_STRLTI, "string-ci<?");	/* nmh */
	mk_proc(OP_STRREF, "string-ref");	/* nmh */
	mk_proc(OP_STRSET, "string-set!");	/* nmh */
	mk_proc(OP_STRSYM, "string->symbol");	/* nmh */
	mk_proc(OP_SUB, "-");
	mk_proc(OP_SUBSTR, "substring");	/* nmh */
	mk_proc(OP_SYMBOL, "symbol?");
	mk_proc(OP_SYMSTR, "symbol->string");	/* nmh */
	mk_proc(OP_W0INFILE, "with-input-from-file");	/* nmh */
	mk_proc(OP_W0OUTFILE, "with-output-to-file");	/* nmh */
	mk_proc(OP_WRCHR, "write-char");	/* nmh */
	mk_proc(OP_WRITE, "write");
	mk_proc(OP_ZEROP, "zero?");
}

void init_globals(void) {
	init_vars_global();
	init_syntax();
	init_procs();
	LAMBDA = mk_symbol("lambda");
	QUOTE = mk_symbol("quote");
	QQUOTE = mk_symbol("quasiquote");
	UNQUOTE = mk_symbol("unquote");
	UNQUOTESP = mk_symbol("unquote-splicing");
}

void init_scheme(void) {
	if (alloc_cellseg(INITIAL_CELLSEGS) != INITIAL_CELLSEGS)
		fatal("unable to allocate initial cell segments");
	if (!alloc_strseg())
		fatal("unable to allocate initial string segment");
	init_globals();
}

/* Error Handler */

void fatal(char *msg) {
	printf("fatal error: %s\n", msg);
	exit(1);
}

cell *error(char *msg) {
	printf("error: %s\n", msg);
	flushinput();
#ifdef USE_SETJMP
	longjmp(error_jmp, OP_T0LVL);
#endif
	return F;
}

#ifdef USE_SETJMP
void catch_intr(int dummy) {
	intr = 1;
	signal(SIGINT, catch_intr);
}
#endif

/* Main */

int main(int argc, char **argv) {
	short	op = (short) OP_INIT;

	if (argc == 2 && !strcmp(argv[1], "-q"))
		quiet = 1;
	if (!quiet) printf(BANNER);
	init_scheme();
#ifdef USE_SETJMP
	signal(SIGINT, catch_intr);
	op = setjmp(error_jmp);	/* OP_INIT == 0 */
#endif
	eval_cycle(op);
	return 0;
}
