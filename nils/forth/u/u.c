/* Useless programming language; NMH 2011; In the Public Domain */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <time.h>

#define cell	long
#define ucell	unsigned cell

#define LIB	"/u/bin/ulib"

#define Z	sizeof(cell)
#define STK	1024
#define MEM	16384*Z
#define NAM	8
#define PTH	128
#define PRG	64
#define MOD	997

typedef struct hash {
	char		s[NAM+1];
	cell		a;
	struct hash	*n;
} L;

cell	S[STK], P;
cell	R[STK], Q;
cell	M[MEM/Z+1], H;
char	*B;
L	*Y[MOD];
FILE	*I, *O;
jmp_buf	J;
int	Batch = 0;
int	Argc;
char	**Argv;

#ifndef __STDC__
 #ifndef __TURBOC__
  #define remove unlink
 #endif
#endif

#ifndef STRDUP
char *strdup(s)
char *s;
{
	char	*n;

	n = malloc(strlen(s)+1);
	if (NULL == n) abort();
	strcpy(n, s);
	return n;
}
#endif

void basename(s)
char *s;
{
	char	*q;

}

cell e(s, m)
char *s, *m;
{
	fflush(stdout);
	fflush(O);
	fprintf(stderr, m? "%s: %s!\n": "%s!\n", s, m);
	P = Q = 0;
	longjmp(J, 1);
	return 0;
}

int h(s)
char *s;
{
	char	*q;
	int	k;

	for (k=0, q=s; *q; q++)
		k = (k<<3) ^ *q;
	return k % MOD;
}

cell f(s, err)
char *s;
int err;
{
	L	*p;
	int	k;

	k = h(s);
	for (p = Y[k]; p; p = p->n)
		if (!strcmp(s, p->s))
			return p->a;
	if (err) e("undefined symbol", s);
	return 0;
}

void a(s, a)
char *s;
cell a;
{
	L	*p;
	int	k;

	k = h(s);
	for (p = Y[k]; p; p = p->n)
		if (!strcmp(s, p->s)) {
			p->a = a;
			return;
		}
	p = malloc(sizeof(L));
	strncpy(p->s, s, NAM);
	p->s[NAM] = 0;
	p->a = a;
	p->n = Y[k];
	Y[k] = p;
}

char MO[] = "memory overflow";

cell cs(s, k)
char *s;
int k;
{
	cell	a = H;

	if (H+k+1 >= MEM) e(MO, 0);
	memmove(&B[H], s, k);
	B[H+k] = 0;
	H += k+1;
	return a;
}

cell ci(i)
cell i;
{
	cell	a;

	H = (H + Z-1) & ~(Z-1);
	a = H;
	if (H+Z >= MEM) e(MO, 0);
	M[H/Z] = i;
	H += Z;
	return a;
}

void reset() {
	int	i;
	L	*p, *q;

	I = stdin;
	O = stdout;
	P = Q = 0;
	H = PRG + Z;
	B = (char *)M;
	srand(time(NULL));
	for (i=0; i<MOD; i++) {
		for (p=Y[i]; p; p=q) {
			q = p->n;
			free(p);
		}
		Y[i] = NULL;
	}
}

void U(x)
cell x;
{
	if (P >= STK) e("stack overflow", 0); else S[P++] = x;
}

void UR(x)
cell x;
{
	if (Q >= STK) e("rstack overflow", 0); else R[Q++] = x;
}

char SU[] = "stack underflow";

#define T() ((P<1)? e(SU, 0): S[P-1])
#define T2() ((P<2)? e(SU, 0): S[P-2])
#define T3() ((P<3)? e(SU, 0): S[P-3])
#define RT() ((Q<1)? e("rstack underflow", 0): R[Q-1])
#define RT2() ((Q<2)? e("rstack underflow", 0): R[Q-2])

void repl();

extern void wop();

void arg(n)
int n;
{
	#define min(a,b) a<b?a:b
	S[P-1] = H+Z;
	if (n < Argc) {
		strncpy(&B[H+Z], Argv[n], MEM-H-Z);
		U(min(MEM-H-Z, strlen(Argv[n])));
	}
	else {
		U(0);
	}
}

#ifdef WOPS
 #include "w.c"
#endif

void r(s)
char *s;
{
	cell	n, m, i, j, t;
	ucell	un;
	char	u[PTH+1];
	char	*q;
	FILE	*p;

	#define sym(c) (isdigit(c) || isalpha(c))
	#define fw()	if (s[i]) i++
	#define fwor(x)	if (s[i]) i++; else e(x, 0)
	#define go(x)	while (s[i] && s[i] != x) i++
	#define S1	S[P-1]
	#define S2	S[P-2]
	#define S3	S[P-3]
	#define R()	if (Q>1) { t = R[--Q]; s=&B[t]; i = R[--Q]; } \
			else return;
#ifdef SANDBOX
	#define path()	strncpy(u, &B[T2()], T()); u[PTH] = 0; \
			if (T() < PTH) u[T()] = 0; P-=2; \
			q=strrchr(u, '/'); if (q) memmove(u,q+1,strlen(q)+1);
#else /*!SANDBOX*/
	#define path()	strncpy(u, &B[T2()], T()); u[PTH] = 0; \
			if (T() < PTH) u[T()] = 0; P-=2;
#endif /*SANDBOX*/
	#define name()	for (n=0; sym(s[i]) && n<NAM; n++) u[n]=s[i++]; \
			u[n] = 0; if (sym(s[i])) e("name too long", u);
	#define call(x)	{ UR(i); UR(t); t = x; s=&B[t]; i=0; }

	if (strlen(s) > PRG) e("program too long", 0);
	strcpy(B, s);
	s = B;
	for (t = i = 0;;) switch (s[i++]) {
	case '\'':for (n=0; isdigit(s[i]); i++) n=n*10+s[i]-'0'; U(n); break;
	case '"': U(t+i); for (n=0; s[i] && s[i]!='"'; i++) n++; U(n); i++;
		break;
	case 'k': U(s[i]); fw(); break;
	case 'h': U(H); break;
	case 'q': Q = 0; if (Batch) exit(0); return; break;
	case 'd': U(T()); break;
	case 'x': T(); P--; break;
	case 's': T2(); n = S1; S1 = S2; S2 = n; break;
	case 'o': U(T2()); break;
	case 't': T3(); n = S3; S3 = S2; S2 = S1; S1 = n; break;
	case 'p': n = T(); if (n >= P) e("p: stack underflow", 0);
		S1 = S[P-n-1]; break;
	case '+': n = T2() + T(); P--; S1 = n; break;
	case '-': n = T2() - T(); P--; S1 = n; break;
	case '*': n = T2() * T(); P--; S1 = n; break;
	case '/': n = T2(); m = S1; if (!m) e("divide by 0", 0);
		S2 = n/m; S1 = n%m; break;
	case '&': n = T2() & T(); P--; S1 = n; break;
	case '|': n = T2() | T(); P--; S1 = n; break;
	case '^': n = T2() ^ T(); P--; S1 = n; break;
	case '<': n = T2() < T(); P--; S1 = n; break;
	case '>': n = T2() > T(); P--; S1 = n; break;
	case '=': n = T2() == T(); P--; S1 = n; break;
	case '#': n = !T(); S1 = n; break;
	case '~': n = ~T(); S1 = n; break;
	case '%': n = -T(); S1 = n; break;
	case 'c': n = T()*Z; S1 = n; break;
	case '?': T(); S1 = M[S1/Z]; break;
	case '!': n = T2(); m = T(); P-=2; M[m/Z] = n; break;
	case '[': n = T(); P--; if (n) i--; else
		{ while (s[i] && s[i] != ';' && s[i] != ']') i++; }
		fwor("[: missing ]"); break;
	case ';': go(']'); fwor(";: missing ]"); break;
	case ']': break;
	case '`': break;
	case '{': n = T(); P--; if (n) i--; else go('}');
		fwor("{: missing }"); break;
	case '}': i=0; go('`'); fwor("}: missing `"); break;
	case '(': n = T(); P--; if (n > 0) UR(n); else
		{ go(')'); fwor("(: missing )"); } break;
	case ')': R[Q-1]--; if (RT() > 0) { i=0; go('('); } else { Q--; i--; }
		fwor("): missing ("); break;
	case 'i': U(RT()); break;
	case 'j': U(RT2()); break;
	case '\\': switch (s[i++]) {
		case '<': n = T2() << T(); P--; S1 = n; break;
		case '>': un = T2(); un >>= T(); P--; S1 = un; break;
		case '+': n = T()+1; S1 = n; break;
		case '-': n = T()-1; S1 = n; break;
		case '!': n = T2(); m = T(); P-=2; M[m/Z] += n; break;
		case 's': U(P); break;
		case 'z': reset(); if (Batch) exit(0); return; break;
		case 'q': exit(0); return; break;
		default: e("\\: invalid op", 0); break;
		}
		break;
	case 'r': switch (s[i++]) {
		case '>': UR(T()); P--; break;
		case '<': U(RT()); Q--; break;
		case 's': U(Q); break;
		case 'x': RT(); Q--; break;
		default: e("r: invalid op", 0); break;
		}
		break;
	case 'f': switch (s[i++]) {
		case '?': U(fgetc(I)); break;
		case '!': fputc(T(), O); P--; break;
		case ',': fprintf(O, "%ld", T()); P--; break;
		case '\'': fscanf(I, "%ld", &n); U(n); break;
		case '@': U(!feof(I)); break;
		case 'f': fflush(O); break;
		case '>': U((cell)I); break;
		case '<': I = (FILE*)T(); P--; break;
		case 'r': n = fread(&B[T2()], 1, T(), I); P--; S1=n;
			if (n < 0) e("read failed", 0); break;
		case 'w': m = T(); n = fwrite(&B[T2()], 1, m, O); P-=2;
			if (n != m) e("write failed", 0); break;
		case '%': ungetc(T(), I); P--; break;
		case 'e': O = stderr; break;
		case 'a': path(); p = fopen(u, "a");
			if (!p) e("cannot open(a)", u); else O=p;
			break;
		case 'i': path(); if (u[0])
			{ p = fopen(u, "r"); if (!p) e("cannot open(i)", u);
			  else I=p; }
			else { if (I != stdin) fclose(I); I=stdin; }
			break;
		case 'o': path(); if (u[0])
			{ p = fopen(u, "w"); if (!p) e("cannot open(o)", u);
			  else O=p; }
			else { if (O != stdout && O != stderr) fclose(O);
				O=stdout; }
			break;
		case 't': path(); U((p = fopen(u, "r")) != 0 && u[0]);
			if (p) fclose(p); break;
		case 'x': path(); if (remove(u)) e("cannot remove", u); break;
		default: e("f: invalid op", 0); break;
		}
		break;
	case 'b': switch (s[i++]) {
		case ',': u[0] = T(); cs(u, 1); H--; P--; break;
		case '?': T(); S1 = B[S1] & 0xff; break;
		case '!': n = T2(); m = T(); P-=2; B[m] = n; break;
		case 'm': memmove(&B[T2()], &B[T3()], T()); P-=3; break;
		case 'c': n =  memcmp(&B[T2()], &B[T3()], T()); P-=2;
			S1 = n; break;
		case 'f': memset(&B[T3()], T(), T2()); P-=3; break;
		case 's': q = &B[T3()]; n = T2(); m = T(); P-=2;
			for (j=0; j<n; j++) if (q[j] == m) break;
			if (j==n) S1 = -1; else S1 = j; break;
		default: e("b: invalid op", 0); break;
		}
		break;
	case 'l': path(); q = strdup(B); p = fopen(u, "r");
		if (!p) e("cannot open", u); repl(0, p); fclose(p);
		strcpy(B, q); free(q); break;
	case 'n': name(); a(u, T()); P--; break;
	case 'v': name(); a(u, ci(T())); P--; break;
	case 'a': n = T(); if (H+n >= MEM) e(MO, 0); H += n; P--; break;
	case ',': ci(T()); P--; break;
	case ':': name(); j = i; go(0); a(u, -cs(&s[j], i-j)); break;
	case '@': name(); U(f(u, 1)); break;
	case '_': name(); m = f(u, 1); if (m<0) call(-m) else U(m); break;
	case 'e': n = T(); P--; if (n<0) call(-n) else e("e: bad token", 0);
		break;
	case 'g': n = s[i++]; go(n); if (s[i] == n) i++;
		else e("g: label not found", 0);
	case 'y': R(); break;
	case '\0': R(); break;
	case 'z': P = 0; break;
	case 'u': U(rand()); break;
	case 'm': arg(T()); break;
#ifdef WOPS
	case 'w': wop(s[i++]); break;
#endif
	case '.': break;
	case ' ': break;
	default: e("syntax error", &s[i-1]); break;
	}
}

void repl(pr, in)
int pr;
FILE *in;
{
	char	p[PRG+2];
	int	k;

	while (1) {
		if (pr) { printf("* "); fflush(stdout); }
		fgets(p, PRG+1, in);
		k = strlen(p);
		if (p[k-1] == '\n') p[k-1] = 0;
		if (feof(in)) break;
		r(p);
	}
}

int load(s, opt)
char *s;
int opt;
{
	FILE	*f = fopen(s, "r");

	if (f == NULL) {
		if (opt)
			return -1;
		else
			e("file not found", s);
	}
	repl(0, f);
	fclose(f);
	return 0;
}

int main(argc, argv)
int argc;
char **argv;
{
	Argc = argc-2;
	Argv = argv+2;
	reset();
	if (!setjmp(J) && load(LIB, 1)) load("ulib", 0);
	Batch = argc > 1;
	if (Batch)
		if (!setjmp(J)) {
			if (strcmp(argv[1], "-"))
				load(argv[1], 0);
		}
		else {
			exit(1);
		}
	repl(1, stdin);
	return 0;
}
