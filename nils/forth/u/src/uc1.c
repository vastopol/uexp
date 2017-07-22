#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define cell long
#define Z sizeof(cell)
#define MMAX 49152
#define align() H = (H+Z-1) & ~(Z-1)
#define B unsigned char
cell S[1024], R[1024], M[MMAX/Z+1];
B *BM = (B*)M;
FILE *I, *O;
int Argc;
char **Argv;
cell P = 0, Q = 0, K = 0, A, H = 224;
#ifndef __STDC__
 #define remove unlink
#endif
cell e(m, a)
char *m, *a;
{
 fputs("runtime error: ", stderr);
 fputs(m, stderr);
 if (a) {
  fputs(": ", stderr);
  fputs(a, stderr);
 }
 fputs("!\n", stderr);
 exit(2);
}
void wrongip() {
 e("bad instruction pointer", 0);
}
char *zstr() {
 static char buf[129];
 if (S[P-1] > 128)
  e("string argument too long", 0);
 memmove(buf, (char*)S[P-2], S[P-1]);
 buf[S[P-1]] = 0;
 P -= 2;
 return buf;
}
void inputfile() {
 char *s = zstr();
 FILE *p;
 if (s[0]) {
  p = fopen(s, "r");
  if (!p)
   e("cannot open(i)", s);
  else
   I = p;
 } else {
  if (I != stdin) fclose(I);
  I = stdin;
 }
}
void outputfile() {
 char *s = zstr();
 FILE *p;
 if (s[0]) {
  p = fopen(s, "w");
  if (!p)
   e("cannot open(o)", s);
  else
   O = p;
 } else {
  if (O != stdout && O != stderr)
   fclose(O);
  O = stdout;
 }
}
void appendfile() {
 char *s = zstr();
 FILE *p;
 p = fopen(s, "a");
 if (!p)
  e("cannot open(a)", s);
 else
  O = p;
}
cell testfile() {
 char *s = zstr();
 FILE *p;
 p = *s? fopen(s, "r"): NULL;
 if (p) fclose(p);
 return p != NULL;
}
void deletefile() {
 char *s = zstr();
 if (remove(s))
  e("cannot remove", s);
}
cell getarg(n)
cell n;
{
 S[P++] = (cell) (n < Argc-1? Argv[n+1]: "");
 return strlen((char *)S[P-1]);
}
cell bscan(q, n, b)
B *q;
cell n, b;
{
 int j;
 for (j=0; j<n; j++)
  if (q[j] == b) break;
 return j==n? -1: j;
}
#define cm() if (H >= MMAX) \
              e("memory overflow", NULL);
int main(argc, argv)
int argc;
char **argv;
{
I = stdin; O = stdout;
Argc = argc; Argv = argv;
srand(time(NULL));
for (;;) switch(K) {
case 0:
case 1:
K = 392; break;
/* 2s */ case 2:
R[Q++] = S[--P];
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
S[P++] = R[--Q];
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
K = R[--Q]; break;
K = 392; break;
/* 2o */ case 3:
S[P++] = 4;
S[P-1] = S[P-S[P-1]-1];
S[P++] = 4;
S[P-1] = S[P-S[P-1]-1];
K = R[--Q]; break;
K = 392; break;
/* copy */ case 4:
S[P] = S[P-1]; P++;
R[Q++] = S[--P];
R[Q++] = S[--P];
case 393:
if (R[Q-1] < 1) { K = 394; break; }
S[P++] = R[Q-2];
S[P-1] = S[P-S[P-1]-1];
--R[Q-1];
{ K = 393; break; }
case 394:
--Q;
--Q;
K = R[--Q]; break;
case 392:
K = 395; break;
/* 2eq */ case 5:
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P-2] = S[P-2] == S[P-1]; --P;
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
S[P-2] = S[P-2] == S[P-1]; --P;
S[P-2] = S[P-2] & S[P-1]; --P;
K = R[--Q]; break;
K = 395; break;
/* abs */ case 6:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] < 0;
if (!S[--P]) { K = 396; break; }
S[P-1] = -S[P-1];
case 396:
K = R[--Q]; break;
K = 395; break;
/* fac */ case 7:
S[P++] = 1;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = S[--P];
case 397:
if (R[Q-1] < 1) { K = 398; break; }
S[P++] = R[Q-1];
S[P-2] = S[P-2] * S[P-1]; --P;
--R[Q-1];
{ K = 397; break; }
case 398:
--Q;
K = R[--Q]; break;
K = 395; break;
/* in */ case 8:
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] >= S[P-1]; --P;
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
S[P-2] = S[P-2] <= S[P-1]; --P;
S[P-2] = S[P-2] & S[P-1]; --P;
K = R[--Q]; break;
K = 395; break;
/* max */ case 9:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] < S[P-1]; --P;
if (!S[--P]) { K = 399; break; }
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
case 399:
--P;
K = R[--Q]; break;
K = 395; break;
/* min */ case 10:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] >= S[P-1]; --P;
if (!S[--P]) { K = 400; break; }
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
case 400:
--P;
K = R[--Q]; break;
K = 395; break;
/* pow */ case 11:
S[P++] = 1;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = S[--P];
case 401:
if (R[Q-1] < 1) { K = 402; break; }
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] * S[P-1]; --P;
--R[Q-1];
{ K = 401; break; }
case 402:
--Q;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
K = R[--Q]; break;
K = 395; break;
/* rnd */ case 12:
S[P++] = rand();
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
A = S[P-2] % S[P-1]; S[P-2] = S[P-2] / S[P-1]; S[P-1] = A;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
K = R[--Q]; break;
K = 395; break;
/* sqrt */ case 13:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] == 8;
if (!S[--P]) { K = 403; break; }
--P;
S[P++] = 2;
K = R[--Q]; break;
case 403:
S[P++] = 2;
S[P++] = 0;
S[P] = S[P-1]; P++;
case 404:
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] - S[P-1]; --P;
R[Q++] = 405; K = 6; break;
case 405:
if (!S[--P]) { K = 406; break; }
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
S[P++] = 4;
S[P-1] = S[P-S[P-1]-1];
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
S[P-2] = S[P-2] / S[P-1]; --P;
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1] = S[P-1] / 2;
R[Q++] = S[--P];
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
--P;
S[P++] = R[--Q];
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
{ K = 404; break; }
case 406:
--P;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
K = R[--Q]; break;
case 395:
K = 407; break;
/* align */ case 14:
S[P++] = 8;
S[P-1]--;
S[P] = S[P-1]; P++;
S[P++] = (cell)&BM[H];
S[P-2] = S[P-2] + S[P-1]; --P;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1] = ~S[P-1];
S[P-2] = S[P-2] & S[P-1]; --P;
S[P++] = (cell)&BM[H];
S[P-2] = S[P-2] - S[P-1]; --P;
H += S[--P]; cm();
K = R[--Q]; break;
K = 407; break;
/* tbl */ case 15:
S[P++] = 8;
S[P-1] = -S[P-1];
H += S[--P]; cm();
K = R[--Q]; break;
case 407:
K = 408; break;
/* bl */ case 16:
S[P++] = 32;
fputc(S[--P], O);
K = R[--Q]; break;
K = 408; break;
/* bls */ case 17:
R[Q++] = S[--P];
case 409:
if (R[Q-1] < 1) { K = 410; break; }
R[Q++] = 411; K = 16; break;
case 411:
--R[Q-1];
{ K = 409; break; }
case 410:
--Q;
K = R[--Q]; break;
K = 408; break;
/* nl */ case 18:
S[P++] = 10;
fputc(S[--P], O);
K = R[--Q]; break;
K = 408; break;
/* getl */ case 19:
S[P] = S[P-2]; P++;
R[Q++] = S[--P];
case 412:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] > 0;
S[P++] = fgetc(I);
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P++] = !feof(I);
S[P-2] = S[P-2] & S[P-1]; --P;
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] != 10;
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 413; break; }
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
*(B*)S[P-1] = S[P-2]; P-=2;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]++;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]--;
{ K = 412; break; }
case 413:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] != 10;
if (!S[--P]) { K = 414; break; }
ungetc(S[--P], I);
{ K = 415; break; }
case 414:
--P;
case 415:
--P;
S[P++] = R[--Q];
S[P-2] = S[P-2] - S[P-1]; --P;
K = R[--Q]; break;
K = 408; break;
/* pad */ case 20:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-2] = S[P-2] - S[P-1]; --P;
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] > 0;
if (!S[--P]) { K = 416; break; }
R[Q++] = 417; K = 17; break;
case 417:
{ K = 418; break; }
case 416:
--P;
case 418:
K = R[--Q]; break;
K = 408; break;
/* pfn */ case 21:
S[P] = S[P-2]; P++;
S[P++] = (cell)&BM[H];
R[Q++] = 419; K = 49; break;
case 419:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 420; K = 20; break;
case 420:
fprintf(O, "%ld", S[--P]);
K = R[--Q]; break;
K = 408; break;
/* pfs */ case 22:
S[P] = S[P-2]; P++;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 421; K = 2; break;
case 421:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 422; K = 20; break;
case 422:
K = R[--Q]; break;
case 408:
K = 423; break;
/* cnum */ case 23:
S[P++] = 48;
S[P++] = 57;
R[Q++] = 424; K = 8; break;
case 424:
K = R[--Q]; break;
K = 423; break;
/* clower */ case 24:
S[P++] = 97;
S[P++] = 122;
R[Q++] = 425; K = 8; break;
case 425:
K = R[--Q]; break;
K = 423; break;
/* cupper */ case 25:
S[P++] = 65;
S[P++] = 90;
R[Q++] = 426; K = 8; break;
case 426:
K = R[--Q]; break;
K = 423; break;
/* calpha */ case 26:
S[P] = S[P-1]; P++;
R[Q++] = 427; K = 25; break;
case 427:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 428; K = 24; break;
case 428:
S[P-2] = S[P-2] | S[P-1]; --P;
K = R[--Q]; break;
K = 423; break;
/* calnum */ case 27:
S[P] = S[P-1]; P++;
R[Q++] = 429; K = 26; break;
case 429:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 430; K = 23; break;
case 430:
S[P-2] = S[P-2] | S[P-1]; --P;
K = R[--Q]; break;
K = 423; break;
/* clowc */ case 28:
S[P] = S[P-1]; P++;
R[Q++] = 431; K = 25; break;
case 431:
if (!S[--P]) { K = 432; break; }
S[P-1] = S[P-1] + 32;
case 432:
K = R[--Q]; break;
K = 423; break;
/* cupc */ case 29:
S[P] = S[P-1]; P++;
R[Q++] = 433; K = 24; break;
case 433:
if (!S[--P]) { K = 434; break; }
S[P-1] = S[P-1] - 32;
case 434:
K = R[--Q]; break;
K = 423; break;
/* rbs */ case 30:
R[Q++] = S[--P];
R[Q++] = S[--P];
case 435:
if (R[Q-1] < 1) { K = 436; break; }
S[P] = S[P-1]; P++;
S[P++] = R[Q-1];
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1]--;
S[P-1] = *(B*)S[P-1];
S[P++] = R[Q-2];
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 437; break; }
--P;
S[P++] = R[--Q];
S[P-1]--;
--Q;
K = R[--Q]; break;
case 437:
--R[Q-1];
{ K = 435; break; }
case 436:
--Q;
--P;
--Q;
S[P++] = 1;
S[P-1] = -S[P-1];
K = R[--Q]; break;
case 423:
K = 438; break;
/* seql */ case 31:
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] != S[P-1]; --P;
if (!S[--P]) { K = 439; break; }
P-=3;
S[P++] = 0;
{ K = 440; break; }
case 439:
S[P-3] = memcmp((B*)S[P-2], (B*)S[P-3], S[P-1]); P-=2;
S[P-1] = !S[P-1];
case 440:
K = R[--Q]; break;
K = 438; break;
/* smap */ case 32:
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
S[P] = S[P-1]; P++;
R[Q++] = S[--P];
case 441:
if (R[Q-1] < 1) { K = 442; break; }
S[P] = S[P-2]; P++;
S[P++] = R[Q-1];
S[P-1]--;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1] = *(B*)S[P-1];
S[P++] = 4;
S[P-1] = S[P-S[P-1]-1];
R[Q++] = 443; K = S[--P]; break;
case 443:
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
S[P-1]--;
S[P++] = R[Q-1];
S[P-2] = S[P-2] + S[P-1]; --P;
*(B*)S[P-1] = S[P-2]; P-=2;
--R[Q-1];
{ K = 441; break; }
case 442:
--Q;
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
--P;
K = R[--Q]; break;
K = 438; break;
/* smem */ case 33:
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P-3] = bscan((B*)S[P-3], S[P-2], S[P-1]); P-=2;
S[P++] = 1;
S[P-1] = -S[P-1];
S[P-2] = S[P-2] != S[P-1]; --P;
K = R[--Q]; break;
K = 438; break;
/* snext */ case 34:
S[P] = S[P-1]; P++;
if (!S[--P]) { K = 444; break; }
S[P-1]--;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]++;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
case 444:
K = R[--Q]; break;
K = 438; break;
/* sprev */ case 35:
S[P-1]++;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]--;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
K = R[--Q]; break;
K = 438; break;
/* srev */ case 36:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1]--;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1] = S[P-1] / 2;
R[Q++] = S[--P];
case 445:
if (R[Q-1] < 1) { K = 446; break; }
S[P] = S[P-2]; P++;
S[P-1] = *(B*)S[P-1];
S[P] = S[P-2]; P++;
S[P-1] = *(B*)S[P-1];
S[P++] = 4;
S[P-1] = S[P-S[P-1]-1];
*(B*)S[P-1] = S[P-2]; P-=2;
S[P] = S[P-2]; P++;
*(B*)S[P-1] = S[P-2]; P-=2;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]++;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]--;
--R[Q-1];
{ K = 445; break; }
case 446:
--Q;
P-=2;
K = R[--Q]; break;
K = 438; break;
/* ssave */ case 37:
S[P++] = (cell)&BM[H];
R[Q++] = S[--P];
S[P] = S[P-1]; P++;
S[P-1]++;
BM[H++] = S[--P]; cm();
S[P++] = (cell)&BM[H];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
memmove((B*)S[P-2], (B*)S[P-3], S[P-1]); P-=3;
S[P++] = R[--Q];
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
S[P] = S[P-1]; P++;
S[P-1]--;
H += S[--P]; cm();
R[Q++] = 447; K = 34; break;
case 447:
K = R[--Q]; break;
K = 438; break;
/* sskipw */ case 38:
case 448:
S[P] = S[P-2]; P++;
S[P-1] = *(B*)S[P-1];
S[P++] = 32;
S[P-2] = S[P-2] == S[P-1]; --P;
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] != 0;
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 449; break; }
R[Q++] = 450; K = 34; break;
case 450:
{ K = 448; break; }
case 449:
K = R[--Q]; break;
K = 438; break;
/* sskipto */ case 39:
R[Q++] = S[--P];
case 451:
S[P] = S[P-2]; P++;
S[P-1] = *(B*)S[P-1];
S[P++] = R[Q-1];
S[P-2] = S[P-2] != S[P-1]; --P;
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] != 0;
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 452; break; }
R[Q++] = 453; K = 34; break;
case 453:
{ K = 451; break; }
case 452:
S[P++] = R[--Q];
--P;
K = R[--Q]; break;
K = 438; break;
/* suntrail */ case 40:
R[Q++] = S[--P];
case 454:
if (R[Q-1] < 1) { K = 455; break; }
S[P++] = R[Q-1];
S[P-1]--;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1] = *(B*)S[P-1];
S[P++] = 32;
S[P-2] = S[P-2] != S[P-1]; --P;
if (!S[--P]) { K = 456; break; }
S[P++] = R[Q-1];
--Q;
K = R[--Q]; break;
case 456:
--R[Q-1];
{ K = 454; break; }
case 455:
--Q;
S[P++] = 0;
K = R[--Q]; break;
K = 438; break;
/* sxlate */ case 41:
R[Q++] = 457; K = 2; break;
case 457:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
R[Q++] = S[--P];
R[Q++] = S[--P];
R[Q++] = S[--P];
case 458:
if (R[Q-1] < 1) { K = 459; break; }
S[P] = S[P-1]; P++;
S[P++] = R[Q-1];
S[P-1]--;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1] = *(B*)S[P-1];
S[P++] = 4;
S[P-1] = S[P-S[P-1]-1];
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 460; break; }
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
S[P++] = R[Q-1];
S[P-1]--;
S[P-2] = S[P-2] + S[P-1]; --P;
*(B*)S[P-1] = S[P-2]; P-=2;
case 460:
--R[Q-1];
{ K = 458; break; }
case 459:
--Q;
P-=3;
S[P++] = R[--Q];
S[P++] = R[--Q];
K = R[--Q]; break;
K = 438; break;
/* sfind */ case 42:
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] - S[P-1]; --P;
S[P-1]++;
R[Q++] = S[--P];
case 461:
if (R[Q-1] < 1) { K = 462; break; }
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
S[P] = S[P-2]; P++;
R[Q++] = 463; K = 3; break;
case 463:
R[Q++] = 464; K = 31; break;
case 464:
if (!S[--P]) { K = 465; break; }
--Q;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
S[P++] = 1;
K = R[--Q]; break;
case 465:
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P-1]++;
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
--R[Q-1];
{ K = 461; break; }
case 462:
--Q;
P-=3;
S[P++] = 0;
K = R[--Q]; break;
K = 438; break;
/* srfind */ case 43:
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] - S[P-1]; --P;
S[P-1]++;
R[Q++] = S[--P];
case 466:
if (R[Q-1] < 1) { K = 467; break; }
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
S[P++] = R[Q-1];
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1]--;
S[P] = S[P-2]; P++;
R[Q++] = 468; K = 3; break;
case 468:
R[Q++] = 469; K = 31; break;
case 469:
if (!S[--P]) { K = 470; break; }
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P++] = R[--Q];
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1]--;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P++] = 1;
K = R[--Q]; break;
case 470:
--R[Q-1];
{ K = 466; break; }
case 467:
--Q;
P-=3;
S[P++] = 0;
K = R[--Q]; break;
K = 438; break;
/* scopy */ case 44:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
S[P++] = 3;
S[P-1] = S[P-S[P-1]-1];
memmove((B*)S[P-2], (B*)S[P-3], S[P-1]); P-=3;
K = R[--Q]; break;
K = 438; break;
/* sappnd */ case 45:
R[Q++] = 471; K = 3; break;
case 471:
S[P-2] = S[P-2] + S[P-1]; --P;
R[Q++] = 472; K = 44; break;
case 472:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
S[P-2] = S[P-2] + S[P-1]; --P;
K = R[--Q]; break;
K = 438; break;
/* ston */ case 46:
S[P++] = 0;
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
R[Q++] = S[--P];
case 473:
if (R[Q-1] < 1) { K = 474; break; }
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
R[Q++] = 475; K = 23; break;
case 475:
S[P-1] = !S[P-1];
if (!S[--P]) { K = 476; break; }
S[P++] = R[--Q];
P-=2;
K = R[--Q]; break;
case 476:
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
S[P++] = 48;
S[P-2] = S[P-2] - S[P-1]; --P;
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P-1] = S[P-1] * 10;
S[P-2] = S[P-2] + S[P-1]; --P;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]++;
--R[Q-1];
{ K = 473; break; }
case 474:
--Q;
--P;
K = R[--Q]; break;
K = 438; break;
/* stoi */ case 47:
S[P] = S[P-2]; P++;
S[P-1] = *(B*)S[P-1];
S[P++] = 45;
S[P-2] = S[P-2] == S[P-1]; --P;
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] != 0;
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 477; break; }
R[Q++] = 478; K = 34; break;
case 478:
R[Q++] = 479; K = 46; break;
case 479:
S[P-1] = -S[P-1];
{ K = 480; break; }
case 477:
R[Q++] = 481; K = 46; break;
case 481:
case 480:
K = R[--Q]; break;
K = 438; break;
/* ntos */ case 48:
S[P] = S[P-1]; P++;
R[Q++] = S[--P];
S[P++] = 1;
case 482:
if (!S[--P]) { K = 483; break; }
S[P] = S[P-2]; P++;
S[P++] = 10;
A = S[P-2] % S[P-1]; S[P-2] = S[P-2] / S[P-1]; S[P-1] = A;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
S[P++] = 48;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P] = S[P-2]; P++;
*(B*)S[P-1] = S[P-2]; P-=2;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1] = S[P-1] / 10;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]++;
S[P] = S[P-2]; P++;
{ K = 482; break; }
case 483:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
S[P++] = R[--Q];
S[P] = S[P-1]; P++;
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-2] = S[P-2] - S[P-1]; --P;
R[Q++] = 484; K = 36; break;
case 484:
K = R[--Q]; break;
K = 438; break;
/* itos */ case 49:
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] < 0;
if (!S[--P]) { K = 485; break; }
S[P] = S[P-1]; P++;
R[Q++] = S[--P];
S[P-1]++;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1] = -S[P-1];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 486; K = 48; break;
case 486:
S[P++] = 45;
S[P++] = R[--Q];
*(B*)S[P-1] = S[P-2]; P-=2;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]--;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]++;
{ K = 487; break; }
case 485:
R[Q++] = 488; K = 48; break;
case 488:
case 487:
K = R[--Q]; break;
case 438:
K = 489; break;
/* s */ case 50:
S[P] = P; P++;
R[Q++] = S[--P];
case 490:
if (R[Q-1] < 1) { K = 491; break; }
R[Q++] = 492; K = 16; break;
case 492:
S[P++] = R[Q-1];
S[P-1] = S[P-S[P-1]-1];
fprintf(O, "%ld", S[--P]);
--R[Q-1];
{ K = 490; break; }
case 491:
--Q;
R[Q++] = 493; K = 18; break;
case 493:
K = R[--Q]; break;
K = 489; break;
/* Uhex1 */ case 51:
S[P++] = (cell)"0123456789abcdef";
S[P++] = 16;
--P;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1] = *(B*)S[P-1];
K = R[--Q]; break;
case 489:
S[P++] = 0;
align(); M[0] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
K = 494; break;
/* Uhex */ case 52:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] >> 4;
R[Q++] = 495; K = 51; break;
case 495:
S[P++] = M[0];
*(B*)S[P-1] = S[P-2]; P-=2;
S[P-1] = S[P-1] & 15;
R[Q++] = 496; K = 51; break;
case 496:
S[P++] = M[0];
S[P-1] = S[P-1] + 1;
*(B*)S[P-1] = S[P-2]; P-=2;
S[P++] = M[0];
S[P++] = 2;
K = R[--Q]; break;
K = 494; break;
/* pr8 */ case 53:
S[P-1] = S[P-1] & 255;
R[Q++] = 497; K = 52; break;
case 497:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
K = R[--Q]; break;
K = 494; break;
/* pr16 */ case 54:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] >> 8;
R[Q++] = 498; K = 53; break;
case 498:
R[Q++] = 499; K = 53; break;
case 499:
K = R[--Q]; break;
K = 494; break;
/* pr32 */ case 55:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] >> 16;
R[Q++] = 500; K = 54; break;
case 500:
S[P-1] = S[P-1] & 65535;
R[Q++] = 501; K = 54; break;
case 501:
K = R[--Q]; break;
K = 494; break;
/* pr64 */ case 56:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] >> 16;
S[P-1] = S[P-1] >> 16;
R[Q++] = 502; K = 55; break;
case 502:
S[P-1] = S[P-1] & 4294967295;
R[Q++] = 503; K = 55; break;
case 503:
K = R[--Q]; break;
K = 494; break;
/* Udbyte */ case 57:
R[Q++] = 504; K = 16; break;
case 504:
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
R[Q++] = 505; K = 53; break;
case 505:
S[P-1]++;
K = R[--Q]; break;
K = 494; break;
/* Udchar */ case 58:
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
S[P] = S[P-1]; P++;
S[P++] = 32;
S[P++] = 126;
R[Q++] = 506; K = 8; break;
case 506:
S[P-1] = !S[P-1];
if (!S[--P]) { K = 507; break; }
--P;
S[P++] = 95;
case 507:
fputc(S[--P], O);
S[P-1]++;
K = R[--Q]; break;
K = 494; break;
/* Udbytes */ case 59:
S[P++] = 16;
R[Q++] = S[--P];
case 508:
if (R[Q-1] < 1) { K = 509; break; }
S[P++] = R[Q-1];
S[P-1] = S[P-1] == 8;
if (!S[--P]) { K = 510; break; }
R[Q++] = 511; K = 16; break;
case 511:
case 510:
R[Q++] = 512; K = 57; break;
case 512:
--R[Q-1];
{ K = 508; break; }
case 509:
--Q;
K = R[--Q]; break;
K = 494; break;
/* Udchars */ case 60:
S[P++] = 16;
R[Q++] = S[--P];
case 513:
if (R[Q-1] < 1) { K = 514; break; }
S[P++] = R[Q-1];
S[P-1] = S[P-1] == 8;
if (!S[--P]) { K = 515; break; }
R[Q++] = 516; K = 16; break;
case 516:
case 515:
R[Q++] = 517; K = 58; break;
case 517:
--R[Q-1];
{ K = 513; break; }
case 514:
--Q;
K = R[--Q]; break;
K = 494; break;
/* Udseg */ case 61:
S[P] = S[P-1]; P++;
R[Q++] = 518; K = 54; break;
case 518:
R[Q++] = 519; K = 16; break;
case 519:
S[P] = S[P-1]; P++;
R[Q++] = 520; K = 59; break;
case 520:
--P;
R[Q++] = 521; K = 16; break;
case 521:
R[Q++] = 522; K = 16; break;
case 522:
R[Q++] = 523; K = 60; break;
case 523:
R[Q++] = 524; K = 18; break;
case 524:
K = R[--Q]; break;
K = 494; break;
/* dump */ case 62:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1] = S[P-1] / 16;
S[P-1] = S[P-1] * 16;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1] = S[P-1] + 15;
S[P-1] = S[P-1] / 16;
R[Q++] = S[--P];
case 525:
if (R[Q-1] < 1) { K = 526; break; }
R[Q++] = 527; K = 61; break;
case 527:
--R[Q-1];
{ K = 525; break; }
case 526:
--Q;
--P;
K = R[--Q]; break;
case 494:
K = 528; break;
/* wkup */ case 63:
S[P-1] = S[P-1] == 259;
K = R[--Q]; break;
K = 528; break;
/* wkdn */ case 64:
S[P-1] = S[P-1] == 258;
K = R[--Q]; break;
K = 528; break;
/* wkle */ case 65:
S[P-1] = S[P-1] == 260;
K = R[--Q]; break;
K = 528; break;
/* wkri */ case 66:
S[P-1] = S[P-1] == 261;
K = R[--Q]; break;
K = 528; break;
/* wkpp */ case 67:
S[P-1] = S[P-1] == 339;
K = R[--Q]; break;
K = 528; break;
/* wknp */ case 68:
S[P-1] = S[P-1] == 338;
K = R[--Q]; break;
K = 528; break;
/* wkbs */ case 69:
S[P-1] = S[P-1] == 263;
K = R[--Q]; break;
K = 528; break;
/* wkesc */ case 70:
S[P-1] = S[P-1] == 27;
K = R[--Q]; break;
case 528:
/* START */
K = 529; break;
/* tmpfile */ case 71:
S[P++] = (cell)"./uctmp";
S[P++] = 7;
K = R[--Q]; break;
K = 529; break;
/* rlibfile */ case 72:
S[P++] = (cell)"/u/bin/ulib";
S[P++] = 11;
K = R[--Q]; break;
K = 529; break;
/* clibfile */ case 73:
S[P++] = (cell)"/u/bin/uclib";
S[P++] = 12;
K = R[--Q]; break;
K = 529; break;
/* wlibfile */ case 74:
S[P++] = (cell)"/u/bin/ucwlib";
S[P++] = 13;
K = R[--Q]; break;
K = 529; break;
/* celltype */ case 75:
S[P++] = (cell)"long";
S[P++] = 4;
K = R[--Q]; break;
case 529:
S[P++] = 8;
M[1] = S[--P];
S[P++] = 0;
align(); M[2] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[3] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
R[Q++] = 531; K = 15; break;
case 531:
S[P++] = 66;
H += S[--P]; cm();
S[P++] = 0;
align(); M[4] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[5] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 2;
align(); M[6] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[7] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[8] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[9] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 1;
align(); M[10] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[11] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[12] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[13] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[14] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[15] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[16] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[17] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[18] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[19] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 0;
align(); M[20] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
K = 530; break;
/* quit */ case 76:
O = stderr;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = (cell)": ";
S[P++] = 2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P] = S[P-1]; P++;
fputc(S[--P], O);
S[P++] = (cell)" (";
S[P++] = 2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 532; K = 53; break;
case 532:
S[P++] = (cell)"h)";
S[P++] = 2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 533; K = 18; break;
case 533:
exit(0);
K = R[--Q]; break;
K = 530; break;
/* err */ case 77:
S[P++] = (cell)"error";
S[P++] = 5;
R[Q++] = 534; K = 76; break;
case 534:
K = R[--Q]; break;
K = 530; break;
/* numchr */ case 78:
S[P++] = 48;
S[P++] = 57;
R[Q++] = 535; K = 8; break;
case 535:
K = R[--Q]; break;
K = 530; break;
/* symchr */ case 79:
S[P] = S[P-1]; P++;
S[P++] = 97;
S[P++] = 122;
R[Q++] = 536; K = 8; break;
case 536:
S[P] = S[P-2]; P++;
S[P++] = 65;
S[P++] = 90;
R[Q++] = 537; K = 8; break;
case 537:
S[P-2] = S[P-2] | S[P-1]; --P;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 538; K = 78; break;
case 538:
S[P-2] = S[P-2] | S[P-1]; --P;
K = R[--Q]; break;
K = 530; break;
/* getsym */ case 80:
S[P++] = 0;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
case 539:
S[P] = S[P-1]; P++;
R[Q++] = 540; K = 79; break;
case 540:
if (!S[--P]) { K = 541; break; }
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] + M[3];
*(B*)S[P-1] = S[P-2]; P-=2;
S[P-1]++;
S[P++] = fgetc(I);
{ K = 539; break; }
case 541:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P++] = M[3];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
K = R[--Q]; break;
K = 530; break;
/* getstr */ case 81:
S[P++] = 0;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
case 542:
S[P] = S[P-1]; P++;
S[P++] = 34;
S[P-2] = S[P-2] != S[P-1]; --P;
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] != 10;
S[P-2] = S[P-2] & S[P-1]; --P;
S[P++] = !feof(I);
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 543; break; }
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] + M[3];
*(B*)S[P-1] = S[P-2]; P-=2;
S[P++] = fgetc(I);
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]++;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
{ K = 542; break; }
case 543:
--P;
S[P++] = M[3];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
K = R[--Q]; break;
K = 530; break;
/* putqs */ case 82:
fputc(S[--P], O);
S[P] = S[P-1]; P++;
R[Q++] = S[--P];
case 544:
if (R[Q-1] < 1) { K = 545; break; }
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P++] = R[Q-1];
S[P-2] = S[P-2] - S[P-1]; --P;
S[P-1] = *(B*)S[P-1];
S[P] = S[P-1]; P++;
S[P++] = 92;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 546; break; }
S[P++] = 92;
fputc(S[--P], O);
case 546:
fputc(S[--P], O);
--R[Q-1];
{ K = 544; break; }
case 545:
--Q;
P-=2;
K = R[--Q]; break;
K = 530; break;
/* putstr */ case 83:
S[P++] = (cell)"(cell)";
S[P++] = 6;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = 34;
R[Q++] = 547; K = 82; break;
case 547:
S[P++] = 34;
fputc(S[--P], O);
K = R[--Q]; break;
K = 530; break;
/* getnum */ case 84:
S[P++] = 0;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
case 548:
S[P] = S[P-1]; P++;
R[Q++] = 549; K = 78; break;
case 549:
if (!S[--P]) { K = 550; break; }
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] + M[3];
*(B*)S[P-1] = S[P-2]; P-=2;
S[P-1]++;
S[P++] = fgetc(I);
{ K = 548; break; }
case 550:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P++] = M[3];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 551; K = 46; break;
case 551:
K = R[--Q]; break;
K = 530; break;
/* comment */ case 85:
S[P++] = (cell)")]}";
S[P++] = 3;
--P;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1] = *(B*)S[P-1];
case 552:
S[P++] = fgetc(I);
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] != S[P-1]; --P;
if (!S[--P]) { K = 553; break; }
{ K = 552; break; }
case 553:
--P;
S[P++] = fgetc(I);
K = R[--Q]; break;
K = 530; break;
/* nip2 */ case 86:
R[Q++] = S[--P];
R[Q++] = S[--P];
P-=2;
S[P++] = R[--Q];
S[P++] = R[--Q];
K = R[--Q]; break;
K = 530; break;
/* nolib */ case 87:
S[P++] = (cell)"could not find library: ";
O = stderr;
S[P++] = 24;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 554; K = 18; break;
case 554:
exit(0);
K = R[--Q]; break;
K = 530; break;
/* openloc */ case 88:
case 555:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
R[Q++] = 556; K = 34; break;
case 556:
S[P++] = 47;
R[Q++] = 557; K = 39; break;
case 557:
S[P] = S[P-1]; P++;
if (!S[--P]) { K = 558; break; }
R[Q++] = 559; K = 86; break;
case 559:
{ K = 555; break; }
case 558:
P-=2;
R[Q++] = 560; K = 34; break;
case 560:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
A = testfile(); S[P++] = A;
if (!S[--P]) { K = 561; break; }
inputfile();
{ K = 562; break; }
case 561:
R[Q++] = 563; K = 87; break;
case 563:
case 562:
K = R[--Q]; break;
K = 530; break;
/* openlib */ case 89:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
A = testfile(); S[P++] = A;
if (!S[--P]) { K = 564; break; }
inputfile();
{ K = 565; break; }
case 564:
R[Q++] = 566; K = 88; break;
case 566:
case 565:
K = R[--Q]; break;
K = 530; break;
/* include */ case 90:
R[Q++] = 567; K = 89; break;
case 567:
case 568:
S[P++] = fgetc(I);
S[P++] = !feof(I);
if (!S[--P]) { K = 569; break; }
fputc(S[--P], O);
{ K = 568; break; }
case 569:
--P;
S[P++] = (cell)"";
S[P++] = 0;
inputfile();
K = R[--Q]; break;
case 530:
K = 570; break;
/* emit */ case 91:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = 59;
fputc(S[--P], O);
R[Q++] = 571; K = 18; break;
case 571:
K = R[--Q]; break;
K = 570; break;
/* unop */ case 92:
S[P++] = (cell)"S[P-1] = ";
S[P++] = 9;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = (cell)"S[P-1]";
S[P++] = 6;
R[Q++] = 572; K = 91; break;
case 572:
K = R[--Q]; break;
K = 570; break;
/* prlab */ case 93:
S[P++] = (cell)"case ";
S[P++] = 5;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
fprintf(O, "%ld", S[--P]);
S[P++] = 58;
fputc(S[--P], O);
R[Q++] = 573; K = 18; break;
case 573:
K = R[--Q]; break;
K = 570; break;
/* prlabc */ case 94:
S[P++] = (cell)"/* ";
S[P++] = 3;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P] = S[P-1]; P++;
R[Q++] = 574; K = 160; break;
case 574:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = (cell)" */ ";
S[P++] = 4;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 575; K = 157; break;
case 575:
S[P-1] = *(cell*)S[P-1];
R[Q++] = 576; K = 93; break;
case 576:
K = R[--Q]; break;
K = 570; break;
/* mklab */ case 95:
S[P++] = *(cell*)M[6];
S[P++] = 1;
*(cell*)M[6] += S[P-1]; --P;
K = R[--Q]; break;
K = 570; break;
/* mkaddr */ case 96:
S[P++] = *(cell*)M[7];
S[P++] = M[1];
*(cell*)M[7] += S[P-1]; --P;
K = R[--Q]; break;
K = 570; break;
/* sblk */ case 97:
S[P++] = 1;
*(cell*)M[9] = S[--P];
S[P++] = *(cell*)M[10];
R[Q++] = 577; K = 93; break;
case 577:
R[Q++] = 578; K = 95; break;
case 578:
*(cell*)M[10] = S[--P];
K = R[--Q]; break;
K = 570; break;
/* blk */ case 98:
S[P++] = *(cell*)M[9];
S[P++] = *(cell*)M[8];
S[P-2] = S[P-2] | S[P-1]; --P;
S[P-1] = !S[P-1];
if (!S[--P]) { K = 579; break; }
R[Q++] = 580; K = 97; break;
case 580:
case 579:
K = R[--Q]; break;
K = 570; break;
/* eblk */ case 99:
S[P++] = (cell)"K = ";
S[P++] = 4;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = *(cell*)M[10];
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"; break";
S[P++] = 7;
R[Q++] = 581; K = 91; break;
case 581:
S[P++] = 0;
*(cell*)M[9] = S[--P];
K = R[--Q]; break;
K = 570; break;
/* call */ case 100:
S[P++] = (cell)"R[Q++] = ";
S[P++] = 9;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 582; K = 95; break;
case 582:
S[P] = S[P-1]; P++;
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"; K = ";
S[P++] = 6;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"; break";
S[P++] = 7;
R[Q++] = 583; K = 91; break;
case 583:
R[Q++] = 584; K = 93; break;
case 584:
K = R[--Q]; break;
K = 570; break;
/* return */ case 101:
S[P++] = *(cell*)M[8];
if (!S[--P]) { K = 585; break; }
R[Q++] = 586; K = 120; break;
case 586:
S[P++] = (cell)"K = R[--Q]; break";
S[P++] = 17;
R[Q++] = 587; K = 91; break;
case 587:
case 585:
S[P++] = 0;
*(cell*)M[8] = S[--P];
K = R[--Q]; break;
K = 570; break;
/* jump */ case 102:
S[P++] = (cell)"{ K = ";
S[P++] = 6;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"; break; }";
S[P++] = 10;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 588; K = 18; break;
case 588:
K = R[--Q]; break;
K = 570; break;
/* begif */ case 103:
R[Q++] = 589; K = 95; break;
case 589:
S[P] = S[P-1]; P++;
*(cell*)M[11] = S[--P];
S[P++] = (cell)"if (!S[--P]) ";
S[P++] = 13;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 590; K = 102; break;
case 590:
K = R[--Q]; break;
K = 570; break;
/* endif */ case 104:
S[P++] = *(cell*)M[11];
R[Q++] = 591; K = 93; break;
case 591:
S[P++] = 0;
*(cell*)M[11] = S[--P];
K = R[--Q]; break;
K = 570; break;
/* begwh */ case 105:
R[Q++] = 592; K = 95; break;
case 592:
S[P] = S[P-1]; P++;
R[Q++] = 593; K = 93; break;
case 593:
*(cell*)M[14] = S[--P];
K = R[--Q]; break;
K = 570; break;
/* endwh */ case 106:
S[P++] = *(cell*)M[14];
R[Q++] = 594; K = 102; break;
case 594:
S[P++] = *(cell*)M[15];
R[Q++] = 595; K = 93; break;
case 595:
S[P++] = 0;
S[P] = S[P-1]; P++;
*(cell*)M[14] = S[--P];
*(cell*)M[15] = S[--P];
K = R[--Q]; break;
K = 570; break;
/* begfn */ case 107:
S[P++] = (cell)"R[Q++] = S[--P]";
S[P++] = 15;
R[Q++] = 596; K = 91; break;
case 596:
R[Q++] = 597; K = 95; break;
case 597:
S[P] = S[P-1]; P++;
*(cell*)M[12] = S[--P];
R[Q++] = 598; K = 93; break;
case 598:
R[Q++] = 599; K = 108; break;
case 599:
K = R[--Q]; break;
K = 570; break;
/* begfn2 */ case 108:
S[P++] = (cell)"if (R[Q-1] < 1) ";
S[P++] = 16;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 600; K = 95; break;
case 600:
S[P] = S[P-1]; P++;
*(cell*)M[13] = S[--P];
R[Q++] = 601; K = 102; break;
case 601:
K = R[--Q]; break;
K = 570; break;
/* endfn */ case 109:
S[P++] = (cell)"--R[Q-1]";
S[P++] = 8;
R[Q++] = 602; K = 91; break;
case 602:
S[P++] = *(cell*)M[12];
R[Q++] = 603; K = 102; break;
case 603:
S[P++] = *(cell*)M[13];
R[Q++] = 604; K = 93; break;
case 604:
S[P++] = (cell)"--Q;";
S[P++] = 4;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 605; K = 110; break;
case 605:
K = R[--Q]; break;
K = 570; break;
/* endfn2 */ case 110:
R[Q++] = 606; K = 18; break;
case 606:
S[P++] = 0;
S[P] = S[P-1]; P++;
*(cell*)M[12] = S[--P];
*(cell*)M[13] = S[--P];
K = R[--Q]; break;
K = 570; break;
/* norml */ case 111:
S[P++] = (cell)"S[P-1] = S[P-1] != 0";
S[P++] = 20;
R[Q++] = 607; K = 91; break;
case 607:
K = R[--Q]; break;
K = 570; break;
/* rotup */ case 112:
S[P++] = (cell)"A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A";
S[P++] = 48;
R[Q++] = 608; K = 91; break;
case 608:
K = R[--Q]; break;
K = 570; break;
/* rotdn */ case 113:
S[P++] = (cell)"A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A";
S[P++] = 48;
R[Q++] = 609; K = 91; break;
case 609:
K = R[--Q]; break;
K = 570; break;
/* drop23 */ case 114:
S[P] = S[P-1]; P++;
S[P++] = 120;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 610; break; }
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"P-=3";
S[P++] = 4;
R[Q++] = 611; K = 91; break;
case 611:
{ K = 612; break; }
case 610:
S[P++] = (cell)"P-=2";
S[P++] = 4;
R[Q++] = 613; K = 91; break;
case 613:
case 612:
K = R[--Q]; break;
case 570:
K = 614; break;
/* qp */ case 115:
*(cell*)M[17] = S[--P];
*(cell*)M[18] = S[--P];
K = R[--Q]; break;
K = 614; break;
/* qpnum */ case 116:
S[P++] = 39;
R[Q++] = 615; K = 115; break;
case 615:
K = R[--Q]; break;
K = 614; break;
/* qpref */ case 117:
S[P++] = 64;
R[Q++] = 616; K = 115; break;
case 616:
K = R[--Q]; break;
K = 614; break;
/* qfval */ case 118:
S[P++] = (cell)"S[P++] = ";
S[P++] = 9;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = *(cell*)M[18];
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"";
S[P++] = 0;
R[Q++] = 617; K = 91; break;
case 617:
K = R[--Q]; break;
K = 614; break;
/* qfref */ case 119:
S[P++] = (cell)"S[P++] = M[";
S[P++] = 11;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = *(cell*)M[18];
S[P-1] = S[P-1] / M[1];
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"]";
S[P++] = 1;
R[Q++] = 618; K = 91; break;
case 618:
K = R[--Q]; break;
K = 614; break;
/* qf */ case 120:
S[P++] = *(cell*)M[17];
S[P++] = 39;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 619; break; }
R[Q++] = 620; K = 118; break;
case 620:
case 619:
S[P++] = *(cell*)M[17];
S[P++] = 64;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 621; break; }
R[Q++] = 622; K = 119; break;
case 622:
case 621:
S[P++] = 0;
*(cell*)M[17] = S[--P];
K = R[--Q]; break;
K = 614; break;
/* qs */ case 121:
S[P++] = *(cell*)M[18];
*(cell*)M[20] = S[--P];
S[P++] = *(cell*)M[17];
*(cell*)M[19] = S[--P];
S[P++] = 0;
*(cell*)M[17] = S[--P];
K = R[--Q]; break;
K = 614; break;
/* qr */ case 122:
S[P++] = *(cell*)M[20];
*(cell*)M[18] = S[--P];
S[P++] = *(cell*)M[19];
*(cell*)M[17] = S[--P];
K = R[--Q]; break;
K = 614; break;
/* qstk */ case 123:
S[P++] = (cell)"S[P-1]";
S[P++] = 6;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
K = R[--Q]; break;
K = 614; break;
/* qnum */ case 124:
S[P++] = *(cell*)M[18];
fprintf(O, "%ld", S[--P]);
K = R[--Q]; break;
K = 614; break;
/* qref */ case 125:
S[P++] = (cell)"M[";
S[P++] = 2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = *(cell*)M[18];
S[P-1] = S[P-1] / M[1];
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"]";
S[P++] = 1;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
K = R[--Q]; break;
K = 614; break;
/* qop */ case 126:
S[P++] = *(cell*)M[17];
S[P++] = 39;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 623; break; }
R[Q++] = 624; K = 124; break;
case 624:
{ K = 625; break; }
case 623:
R[Q++] = 626; K = 127; break;
case 626:
case 625:
S[P++] = 0;
*(cell*)M[17] = S[--P];
K = R[--Q]; break;
K = 614; break;
/* qop2 */ case 127:
S[P++] = *(cell*)M[17];
S[P++] = 64;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 627; break; }
R[Q++] = 628; K = 125; break;
case 628:
{ K = 629; break; }
case 627:
R[Q++] = 630; K = 123; break;
case 630:
case 629:
K = R[--Q]; break;
K = 614; break;
/* qopstk */ case 128:
S[P++] = (cell)"S[P-2] = S[P-2] ";
S[P++] = 16;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 631; K = 16; break;
case 631:
R[Q++] = 632; K = 123; break;
case 632:
S[P++] = (cell)"; --P";
S[P++] = 5;
R[Q++] = 633; K = 91; break;
case 633:
K = R[--Q]; break;
K = 614; break;
/* qopnum */ case 129:
S[P++] = (cell)"S[P-1] = S[P-1] ";
S[P++] = 16;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 634; K = 16; break;
case 634:
R[Q++] = 635; K = 124; break;
case 635:
S[P++] = (cell)"";
S[P++] = 0;
R[Q++] = 636; K = 91; break;
case 636:
K = R[--Q]; break;
K = 614; break;
/* qopref */ case 130:
S[P++] = (cell)"S[P-1] = S[P-1] ";
S[P++] = 16;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 637; K = 16; break;
case 637:
R[Q++] = 638; K = 125; break;
case 638:
S[P++] = (cell)"";
S[P++] = 0;
R[Q++] = 639; K = 91; break;
case 639:
K = R[--Q]; break;
K = 614; break;
/* qbinop */ case 131:
S[P++] = *(cell*)M[17];
S[P++] = 39;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 640; break; }
R[Q++] = 641; K = 129; break;
case 641:
{ K = 642; break; }
case 640:
R[Q++] = 643; K = 132; break;
case 643:
case 642:
S[P++] = 0;
*(cell*)M[17] = S[--P];
K = R[--Q]; break;
K = 614; break;
/* qbinop2 */ case 132:
S[P++] = *(cell*)M[17];
S[P++] = 64;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 644; break; }
R[Q++] = 645; K = 130; break;
case 645:
{ K = 646; break; }
case 644:
R[Q++] = 647; K = 128; break;
case 647:
case 646:
K = R[--Q]; break;
K = 614; break;
/* qret */ case 133:
S[P++] = (cell)"S[P++] = ";
S[P++] = 9;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
K = R[--Q]; break;
K = 614; break;
/* qretstk */ case 134:
S[P++] = (cell)"S[P-1] = ";
S[P++] = 9;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = (cell)"S[P-1]";
S[P++] = 6;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
K = R[--Q]; break;
K = 614; break;
/* qretr */ case 135:
S[P++] = *(cell*)M[17];
S[P++] = 39;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 648; break; }
R[Q++] = 649; K = 133; break;
case 649:
R[Q++] = 650; K = 124; break;
case 650:
{ K = 651; break; }
case 648:
R[Q++] = 652; K = 136; break;
case 652:
case 651:
S[P++] = (cell)"";
S[P++] = 0;
R[Q++] = 653; K = 91; break;
case 653:
S[P++] = 0;
*(cell*)M[17] = S[--P];
K = R[--Q]; break;
K = 614; break;
/* qretr2 */ case 136:
S[P++] = *(cell*)M[17];
S[P++] = 64;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 654; break; }
R[Q++] = 655; K = 133; break;
case 655:
R[Q++] = 656; K = 125; break;
case 656:
{ K = 657; break; }
case 654:
R[Q++] = 658; K = 134; break;
case 658:
case 657:
K = R[--Q]; break;
K = 614; break;
/* qsto */ case 137:
S[P++] = (cell)" = S[--P]";
S[P++] = 9;
R[Q++] = 659; K = 91; break;
case 659:
K = R[--Q]; break;
K = 614; break;
/* qstostk */ case 138:
S[P++] = (cell)"S[P-1] = S[P-2]; P-=2";
S[P++] = 21;
R[Q++] = 660; K = 91; break;
case 660:
K = R[--Q]; break;
K = 614; break;
/* qstor */ case 139:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = *(cell*)M[17];
S[P++] = 39;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 661; break; }
R[Q++] = 662; K = 124; break;
case 662:
R[Q++] = 663; K = 137; break;
case 663:
{ K = 664; break; }
case 661:
R[Q++] = 665; K = 140; break;
case 665:
case 664:
S[P++] = 0;
*(cell*)M[17] = S[--P];
K = R[--Q]; break;
K = 614; break;
/* qstor2 */ case 140:
S[P++] = *(cell*)M[17];
S[P++] = 64;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 666; break; }
R[Q++] = 667; K = 125; break;
case 667:
R[Q++] = 668; K = 137; break;
case 668:
{ K = 669; break; }
case 666:
R[Q++] = 670; K = 138; break;
case 670:
case 669:
K = R[--Q]; break;
K = 614; break;
/* qcell */ case 141:
S[P++] = *(cell*)M[17];
S[P++] = 39;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 671; break; }
S[P++] = *(cell*)M[18];
S[P-1] *= Z;
*(cell*)M[18] = S[--P];
{ K = 672; break; }
case 671:
R[Q++] = 673; K = 142; break;
case 673:
case 672:
K = R[--Q]; break;
K = 614; break;
/* qcellrt */ case 142:
R[Q++] = 674; K = 120; break;
case 674:
S[P++] = (cell)"S[P-1] *= Z";
S[P++] = 11;
R[Q++] = 675; K = 91; break;
case 675:
K = R[--Q]; break;
case 614:
K = 676; break;
/* ss */ case 143:
R[Q++] = 677; K = 37; break;
case 677:
P-=2;
K = R[--Q]; break;
case 676:
S[P++] = 0;
align(); M[21] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
R[Q++] = 679; K = 15; break;
case 679:
S[P++] = (cell)"#include <stdlib.h>";
S[P++] = 19;
R[Q++] = 680; K = 143; break;
case 680:
S[P++] = (cell)"#include <stdio.h>";
S[P++] = 18;
R[Q++] = 681; K = 143; break;
case 681:
S[P++] = (cell)"#include <string.h>";
S[P++] = 19;
R[Q++] = 682; K = 143; break;
case 682:
S[P++] = 0;
align(); M[H/Z] = S[--P]; H+=Z; cm();
K = 678; break;
/* celldef */ case 144:
S[P++] = (cell)"#define cell ";
S[P++] = 13;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 683; K = 75; break;
case 683:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 684; K = 18; break;
case 684:
K = R[--Q]; break;
case 678:
S[P++] = 0;
align(); M[22] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
R[Q++] = 686; K = 15; break;
case 686:
S[P++] = (cell)"#define Z sizeof(cell)";
S[P++] = 22;
R[Q++] = 687; K = 143; break;
case 687:
S[P++] = (cell)"#define MMAX 49152";
S[P++] = 18;
R[Q++] = 688; K = 143; break;
case 688:
S[P++] = (cell)"#define align() H = (H+Z-1) & ~(Z-1)";
S[P++] = 36;
R[Q++] = 689; K = 143; break;
case 689:
S[P++] = (cell)"#define B unsigned char";
S[P++] = 23;
R[Q++] = 690; K = 143; break;
case 690:
S[P++] = (cell)"cell S[1024], R[1024], M[MMAX/Z+1];";
S[P++] = 35;
R[Q++] = 691; K = 143; break;
case 691:
S[P++] = (cell)"B *BM = (B*)M;";
S[P++] = 14;
R[Q++] = 692; K = 143; break;
case 692:
S[P++] = (cell)"FILE *I, *O;";
S[P++] = 12;
R[Q++] = 693; K = 143; break;
case 693:
S[P++] = (cell)"int Argc;";
S[P++] = 9;
R[Q++] = 694; K = 143; break;
case 694:
S[P++] = (cell)"char **Argv;";
S[P++] = 12;
R[Q++] = 695; K = 143; break;
case 695:
S[P++] = 0;
align(); M[H/Z] = S[--P]; H+=Z; cm();
K = 685; break;
/* regdef */ case 145:
S[P++] = (cell)"cell P = 0, Q = 0, K = 0, A, H = ";
S[P++] = 33;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = *(cell*)M[7];
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)";";
S[P++] = 1;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 696; K = 18; break;
case 696:
K = R[--Q]; break;
K = 685; break;
/* inclib */ case 146:
R[Q++] = 697; K = 73; break;
case 697:
R[Q++] = 698; K = 90; break;
case 698:
S[P++] = *(cell*)M[16];
if (!S[--P]) { K = 699; break; }
R[Q++] = 700; K = 74; break;
case 700:
R[Q++] = 701; K = 90; break;
case 701:
case 699:
K = R[--Q]; break;
case 685:
S[P++] = 0;
align(); M[23] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
R[Q++] = 703; K = 15; break;
case 703:
S[P++] = (cell)"#define cm() if (H >= MMAX) \\";
S[P++] = 29;
R[Q++] = 704; K = 143; break;
case 704:
S[P++] = (cell)"              e('memory overflow', NULL);";
S[P++] = 41;
R[Q++] = 705; K = 37; break;
case 705:
S[P++] = 39;
S[P++] = 34;
R[Q++] = 706; K = 41; break;
case 706:
P-=2;
S[P++] = (cell)"int main(argc, argv)";
S[P++] = 20;
R[Q++] = 707; K = 143; break;
case 707:
S[P++] = (cell)"int argc;";
S[P++] = 9;
R[Q++] = 708; K = 143; break;
case 708:
S[P++] = (cell)"char **argv;";
S[P++] = 12;
R[Q++] = 709; K = 143; break;
case 709:
S[P++] = (cell)"{";
S[P++] = 1;
R[Q++] = 710; K = 143; break;
case 710:
S[P++] = (cell)"I = stdin; O = stdout;";
S[P++] = 22;
R[Q++] = 711; K = 143; break;
case 711:
S[P++] = (cell)"Argc = argc; Argv = argv;";
S[P++] = 25;
R[Q++] = 712; K = 143; break;
case 712:
S[P++] = (cell)"srand(time(NULL));";
S[P++] = 18;
R[Q++] = 713; K = 143; break;
case 713:
S[P++] = (cell)"for (;;) switch(K) {";
S[P++] = 20;
R[Q++] = 714; K = 143; break;
case 714:
S[P++] = 0;
align(); M[H/Z] = S[--P]; H+=Z; cm();
K = 702; break;
/* prlst */ case 147:
case 715:
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
if (!S[--P]) { K = 716; break; }
S[P] = S[P-1]; P++;
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
R[Q++] = 717; K = 34; break;
case 717:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 718; K = 18; break;
case 718:
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
S[P-2] = S[P-2] + S[P-1]; --P;
{ K = 715; break; }
case 716:
--P;
K = R[--Q]; break;
K = 702; break;
/* header */ case 148:
S[P++] = M[21];
R[Q++] = 719; K = 147; break;
case 719:
R[Q++] = 720; K = 144; break;
case 720:
S[P++] = M[22];
R[Q++] = 721; K = 147; break;
case 721:
R[Q++] = 722; K = 145; break;
case 722:
R[Q++] = 723; K = 146; break;
case 723:
R[Q++] = 724; K = 149; break;
case 724:
K = R[--Q]; break;
K = 702; break;
/* header2 */ case 149:
S[P++] = M[23];
R[Q++] = 725; K = 147; break;
case 725:
S[P++] = 0;
R[Q++] = 726; K = 93; break;
case 726:
K = R[--Q]; break;
case 702:
K = 727; break;
/* tl00 */ case 150:
S[P++] = *(cell*)M[9];
S[P-1] = !S[P-1];
if (!S[--P]) { K = 728; break; }
R[Q++] = 729; K = 97; break;
case 729:
case 728:
S[P++] = (cell)"exit(0)";
S[P++] = 7;
R[Q++] = 730; K = 91; break;
case 730:
K = R[--Q]; break;
K = 727; break;
/* tl01 */ case 151:
S[P++] = (cell)"default: wrongip(); break";
S[P++] = 25;
R[Q++] = 731; K = 91; break;
case 731:
K = R[--Q]; break;
K = 727; break;
/* tl02 */ case 152:
S[P++] = (cell)"}";
S[P++] = 1;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 732; K = 18; break;
case 732:
K = R[--Q]; break;
K = 727; break;
/* tail */ case 153:
R[Q++] = 733; K = 150; break;
case 733:
R[Q++] = 734; K = 151; break;
case 734:
R[Q++] = 735; K = 152; break;
case 735:
R[Q++] = 736; K = 152; break;
case 736:
K = R[--Q]; break;
case 727:
K = 737; break;
/* save */ case 154:
S[P] = S[P-1]; P++;
BM[H++] = S[--P]; cm();
S[P] = S[P-1]; P++;
R[Q++] = S[--P];
S[P++] = (cell)&BM[H];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
memmove((B*)S[P-2], (B*)S[P-3], S[P-1]); P-=3;
S[P++] = R[--Q];
H += S[--P]; cm();
K = R[--Q]; break;
K = 737; break;
/* link */ case 155:
S[P++] = *(cell*)M[5];
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = (cell)&BM[H];
S[P-1] = S[P-1] - 8;
*(cell*)M[5] = S[--P];
K = R[--Q]; break;
K = 737; break;
/* symlnk */ case 156:
K = R[--Q]; break;
K = 737; break;
/* symval */ case 157:
S[P-1] = S[P-1] + 8;
K = R[--Q]; break;
K = 737; break;
/* symtyp */ case 158:
S[P-1] = S[P-1] + 16;
K = R[--Q]; break;
K = 737; break;
/* symflg */ case 159:
S[P-1] = S[P-1] + 16;
S[P-1]++;
K = R[--Q]; break;
K = 737; break;
/* symnam */ case 160:
S[P-1] = S[P-1] + 16;
S[P-1]++;
S[P-1]++;
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-1]++;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
K = R[--Q]; break;
case 737:
S[P++] = 1;
M[24] = S[--P];
S[P++] = 2;
M[25] = S[--P];
S[P++] = 4;
M[26] = S[--P];
K = 738; break;
/* lookup */ case 161:
S[P++] = *(cell*)M[5];
case 739:
S[P] = S[P-1]; P++;
if (!S[--P]) { K = 740; break; }
S[P] = S[P-1]; P++;
R[Q++] = 741; K = 160; break;
case 741:
S[P++] = 5;
S[P-1] = S[P-S[P-1]-1];
S[P++] = 5;
S[P-1] = S[P-S[P-1]-1];
R[Q++] = 742; K = 31; break;
case 742:
if (!S[--P]) { K = 743; break; }
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
P-=2;
K = R[--Q]; break;
case 743:
R[Q++] = 744; K = 156; break;
case 744:
S[P-1] = *(cell*)S[P-1];
{ K = 739; break; }
case 740:
P-=3;
S[P++] = 0;
K = R[--Q]; break;
K = 738; break;
/* nosym */ case 162:
O = stderr;
S[P++] = (cell)"undefined: ";
S[P++] = 11;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 745; K = 18; break;
case 745:
exit(0);
K = R[--Q]; break;
K = 738; break;
/* findsym */ case 163:
R[Q++] = 746; K = 80; break;
case 746:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
R[Q++] = 747; K = 161; break;
case 747:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] == 0;
if (!S[--P]) { K = 748; break; }
--P;
R[Q++] = 749; K = 162; break;
case 749:
{ K = 750; break; }
case 748:
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
P-=2;
case 750:
K = R[--Q]; break;
K = 738; break;
/* setflg */ case 164:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 751; K = 159; break;
case 751:
S[P] = S[P-1]; P++;
S[P-1] = *(B*)S[P-1];
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P-2] = S[P-2] | S[P-1]; --P;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
*(B*)S[P-1] = S[P-2]; P-=2;
K = R[--Q]; break;
K = 738; break;
/* flgclr */ case 165:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 752; K = 159; break;
case 752:
S[P-1] = *(B*)S[P-1];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
S[P-2] = S[P-2] & S[P-1]; --P;
S[P-1] = !S[P-1];
K = R[--Q]; break;
K = 738; break;
/* flgset */ case 166:
R[Q++] = 753; K = 165; break;
case 753:
S[P-1] = !S[P-1];
K = R[--Q]; break;
K = 738; break;
/* refsym */ case 167:
R[Q++] = 754; K = 163; break;
case 754:
S[P] = S[P-1]; P++;
S[P++] = M[24];
R[Q++] = 755; K = 164; break;
case 755:
K = R[--Q]; break;
K = 738; break;
/* compsym */ case 168:
R[Q++] = 756; K = 155; break;
case 756:
align(); M[H/Z] = S[--P]; H+=Z; cm();
BM[H++] = S[--P]; cm();
S[P++] = 0;
BM[H++] = S[--P]; cm();
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
R[Q++] = 757; K = 154; break;
case 757:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
K = R[--Q]; break;
K = 738; break;
/* mksym */ case 169:
R[Q++] = S[--P];
R[Q++] = S[--P];
R[Q++] = 758; K = 80; break;
case 758:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
R[Q++] = 759; K = 161; break;
case 759:
S[P] = S[P-1]; P++;
if (!S[--P]) { K = 760; break; }
S[P++] = M[25];
R[Q++] = 761; K = 164; break;
case 761:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
{ K = 762; break; }
case 760:
--P;
S[P++] = R[Q-1];
S[P++] = R[Q-2];
R[Q++] = 763; K = 168; break;
case 763:
case 762:
--Q;
--Q;
K = R[--Q]; break;
K = 738; break;
/* unused */ case 170:
O = stderr;
S[P++] = (cell)"unused: ";
S[P++] = 8;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 764; K = 160; break;
case 764:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 765; K = 18; break;
case 765:
S[P++] = (cell)"";
S[P++] = 0;
outputfile();
K = R[--Q]; break;
K = 738; break;
/* duplicat */ case 171:
O = stderr;
S[P++] = (cell)"duplicate: ";
S[P++] = 11;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 766; K = 160; break;
case 766:
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 767; K = 18; break;
case 767:
S[P++] = (cell)"";
S[P++] = 0;
outputfile();
K = R[--Q]; break;
K = 738; break;
/* deadsyms */ case 172:
S[P++] = *(cell*)M[5];
case 768:
S[P] = S[P-1]; P++;
if (!S[--P]) { K = 769; break; }
S[P] = S[P-1]; P++;
S[P++] = M[24];
R[Q++] = 770; K = 165; break;
case 770:
if (!S[--P]) { K = 771; break; }
S[P] = S[P-1]; P++;
R[Q++] = 772; K = 170; break;
case 772:
case 771:
R[Q++] = 773; K = 156; break;
case 773:
S[P-1] = *(cell*)S[P-1];
{ K = 768; break; }
case 769:
--P;
K = R[--Q]; break;
K = 738; break;
/* dupsyms */ case 173:
S[P++] = *(cell*)M[5];
case 774:
S[P] = S[P-1]; P++;
if (!S[--P]) { K = 775; break; }
S[P] = S[P-1]; P++;
S[P++] = M[25];
R[Q++] = 776; K = 166; break;
case 776:
if (!S[--P]) { K = 777; break; }
S[P] = S[P-1]; P++;
R[Q++] = 778; K = 171; break;
case 778:
case 777:
R[Q++] = 779; K = 156; break;
case 779:
S[P-1] = *(cell*)S[P-1];
{ K = 774; break; }
case 775:
--P;
K = R[--Q]; break;
K = 738; break;
/* mrkusd */ case 174:
S[P++] = *(cell*)M[5];
case 780:
S[P] = S[P-2]; P++;
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] != S[P-1]; --P;
if (!S[--P]) { K = 781; break; }
S[P] = S[P-1]; P++;
S[P++] = M[24];
R[Q++] = 782; K = 164; break;
case 782:
R[Q++] = 783; K = 156; break;
case 783:
S[P-1] = *(cell*)S[P-1];
{ K = 780; break; }
case 781:
P-=2;
K = R[--Q]; break;
case 738:
K = 784; break;
/* werr */ case 175:
S[P++] = (cell)"w-op error";
S[P++] = 10;
R[Q++] = 785; K = 76; break;
case 785:
K = R[--Q]; break;
K = 784; break;
/* wwrite */ case 176:
R[Q++] = 786; K = 120; break;
case 786:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"writes((char*)S[P-2], S[P-1]); P-=2;";
S[P++] = 36;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 787; K = 18; break;
case 787:
K = R[--Q]; break;
K = 784; break;
/* wsleep */ case 177:
R[Q++] = 788; K = 120; break;
case 788:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"usleep(S[--P]*1000);";
S[P++] = 20;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 789; K = 18; break;
case 789:
K = R[--Q]; break;
K = 784; break;
/* wcls */ case 178:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"clear();";
S[P++] = 8;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 790; K = 18; break;
case 790:
K = R[--Q]; break;
K = 784; break;
/* wrefr */ case 179:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"refresh();";
S[P++] = 10;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 791; K = 18; break;
case 791:
K = R[--Q]; break;
K = 784; break;
/* wqueue */ case 180:
R[Q++] = 792; K = 120; break;
case 792:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = ttyqueue();";
S[P++] = 20;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 793; K = 18; break;
case 793:
K = R[--Q]; break;
K = 784; break;
/* whascol */ case 181:
R[Q++] = 794; K = 120; break;
case 794:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = has_colors();";
S[P++] = 22;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 795; K = 18; break;
case 795:
K = R[--Q]; break;
K = 784; break;
/* wmove */ case 182:
R[Q++] = 796; K = 120; break;
case 796:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"move(S[P-1], S[P-2]); P-=2;";
S[P++] = 27;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 797; K = 18; break;
case 797:
K = R[--Q]; break;
K = 784; break;
/* wceol */ case 183:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"clrtoeol();";
S[P++] = 11;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 798; K = 18; break;
case 798:
K = R[--Q]; break;
K = 784; break;
/* winit */ case 184:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"initcurses();";
S[P++] = 13;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 799; K = 18; break;
case 799:
K = R[--Q]; break;
K = 784; break;
/* wgeom */ case 185:
R[Q++] = 800; K = 120; break;
case 800:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = COLS; S[P++] = LINES;";
S[P++] = 30;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 801; K = 18; break;
case 801:
K = R[--Q]; break;
K = 784; break;
/* wend */ case 186:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"refresh(); endwin();";
S[P++] = 20;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 802; K = 18; break;
case 802:
K = R[--Q]; break;
K = 784; break;
/* wdelay */ case 187:
R[Q++] = 803; K = 120; break;
case 803:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"nodelay(stdscr,Sp[--P]);";
S[P++] = 24;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 804; K = 18; break;
case 804:
K = R[--Q]; break;
K = 784; break;
/* wcolor */ case 188:
R[Q++] = 805; K = 120; break;
case 805:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"color(S[P-1], S[P-2]); P-=2;";
S[P++] = 28;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 806; K = 18; break;
case 806:
K = R[--Q]; break;
K = 784; break;
/* wbeep */ case 189:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"beep();";
S[P++] = 7;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 807; K = 18; break;
case 807:
K = R[--Q]; break;
K = 784; break;
/* wattr */ case 190:
R[Q++] = 808; K = 120; break;
case 808:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"attr(S[--P]);";
S[P++] = 13;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 809; K = 18; break;
case 809:
K = R[--Q]; break;
K = 784; break;
/* wgetch */ case 191:
R[Q++] = 810; K = 120; break;
case 810:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"refresh(); S[P++] = xgetch();";
S[P++] = 29;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 811; K = 18; break;
case 811:
K = R[--Q]; break;
K = 784; break;
/* wnoraw */ case 192:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"noraw();";
S[P++] = 8;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 812; K = 18; break;
case 812:
K = R[--Q]; break;
K = 784; break;
/* wraw */ case 193:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"raw();";
S[P++] = 6;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 813; K = 18; break;
case 813:
K = R[--Q]; break;
K = 784; break;
/* waddch */ case 194:
R[Q++] = 814; K = 120; break;
case 814:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"addch(S[--P]);";
S[P++] = 14;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 815; K = 18; break;
case 815:
K = R[--Q]; break;
K = 784; break;
/* wcw */ case 195:
S[P] = S[P-1]; P++;
S[P++] = 119;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 816; break; }
R[Q++] = 817; K = 176; break;
case 817:
{ K = 818; break; }
case 816:
R[Q++] = 819; K = 175; break;
case 819:
case 818:
K = R[--Q]; break;
K = 784; break;
/* wcu */ case 196:
S[P] = S[P-1]; P++;
S[P++] = 117;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 820; break; }
R[Q++] = 821; K = 177; break;
case 821:
{ K = 822; break; }
case 820:
R[Q++] = 823; K = 195; break;
case 823:
case 822:
K = R[--Q]; break;
K = 784; break;
/* wcs */ case 197:
S[P] = S[P-1]; P++;
S[P++] = 115;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 824; break; }
R[Q++] = 825; K = 178; break;
case 825:
{ K = 826; break; }
case 824:
R[Q++] = 827; K = 196; break;
case 827:
case 826:
K = R[--Q]; break;
K = 784; break;
/* wcr */ case 198:
S[P] = S[P-1]; P++;
S[P++] = 114;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 828; break; }
R[Q++] = 829; K = 179; break;
case 829:
{ K = 830; break; }
case 828:
R[Q++] = 831; K = 197; break;
case 831:
case 830:
K = R[--Q]; break;
K = 784; break;
/* wcq */ case 199:
S[P] = S[P-1]; P++;
S[P++] = 113;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 832; break; }
R[Q++] = 833; K = 180; break;
case 833:
{ K = 834; break; }
case 832:
R[Q++] = 835; K = 198; break;
case 835:
case 834:
K = R[--Q]; break;
K = 784; break;
/* wco */ case 200:
S[P] = S[P-1]; P++;
S[P++] = 111;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 836; break; }
R[Q++] = 837; K = 181; break;
case 837:
{ K = 838; break; }
case 836:
R[Q++] = 839; K = 199; break;
case 839:
case 838:
K = R[--Q]; break;
K = 784; break;
/* wcm */ case 201:
S[P] = S[P-1]; P++;
S[P++] = 109;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 840; break; }
R[Q++] = 841; K = 182; break;
case 841:
{ K = 842; break; }
case 840:
R[Q++] = 843; K = 200; break;
case 843:
case 842:
K = R[--Q]; break;
K = 784; break;
/* wcl */ case 202:
S[P] = S[P-1]; P++;
S[P++] = 108;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 844; break; }
R[Q++] = 845; K = 183; break;
case 845:
{ K = 846; break; }
case 844:
R[Q++] = 847; K = 201; break;
case 847:
case 846:
K = R[--Q]; break;
K = 784; break;
/* wci */ case 203:
S[P] = S[P-1]; P++;
S[P++] = 105;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 848; break; }
R[Q++] = 849; K = 184; break;
case 849:
{ K = 850; break; }
case 848:
R[Q++] = 851; K = 202; break;
case 851:
case 850:
K = R[--Q]; break;
K = 784; break;
/* wcg */ case 204:
S[P] = S[P-1]; P++;
S[P++] = 103;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 852; break; }
R[Q++] = 853; K = 185; break;
case 853:
{ K = 854; break; }
case 852:
R[Q++] = 855; K = 203; break;
case 855:
case 854:
K = R[--Q]; break;
K = 784; break;
/* wce */ case 205:
S[P] = S[P-1]; P++;
S[P++] = 101;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 856; break; }
R[Q++] = 857; K = 186; break;
case 857:
{ K = 858; break; }
case 856:
R[Q++] = 859; K = 204; break;
case 859:
case 858:
K = R[--Q]; break;
K = 784; break;
/* wcd */ case 206:
S[P] = S[P-1]; P++;
S[P++] = 100;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 860; break; }
R[Q++] = 861; K = 187; break;
case 861:
{ K = 862; break; }
case 860:
R[Q++] = 863; K = 205; break;
case 863:
case 862:
K = R[--Q]; break;
K = 784; break;
/* wcc */ case 207:
S[P] = S[P-1]; P++;
S[P++] = 99;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 864; break; }
R[Q++] = 865; K = 188; break;
case 865:
{ K = 866; break; }
case 864:
R[Q++] = 867; K = 206; break;
case 867:
case 866:
K = R[--Q]; break;
K = 784; break;
/* wcb */ case 208:
S[P] = S[P-1]; P++;
S[P++] = 98;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 868; break; }
R[Q++] = 869; K = 189; break;
case 869:
{ K = 870; break; }
case 868:
R[Q++] = 871; K = 207; break;
case 871:
case 870:
K = R[--Q]; break;
K = 784; break;
/* wca */ case 209:
S[P] = S[P-1]; P++;
S[P++] = 97;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 872; break; }
R[Q++] = 873; K = 190; break;
case 873:
{ K = 874; break; }
case 872:
R[Q++] = 875; K = 208; break;
case 875:
case 874:
K = R[--Q]; break;
K = 784; break;
/* w3f */ case 210:
S[P] = S[P-1]; P++;
S[P++] = 63;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 876; break; }
R[Q++] = 877; K = 191; break;
case 877:
{ K = 878; break; }
case 876:
R[Q++] = 879; K = 209; break;
case 879:
case 878:
K = R[--Q]; break;
K = 784; break;
/* w2d */ case 211:
S[P] = S[P-1]; P++;
S[P++] = 45;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 880; break; }
R[Q++] = 881; K = 193; break;
case 881:
{ K = 882; break; }
case 880:
R[Q++] = 883; K = 210; break;
case 883:
case 882:
K = R[--Q]; break;
K = 784; break;
/* w2b */ case 212:
S[P] = S[P-1]; P++;
S[P++] = 43;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 884; break; }
R[Q++] = 885; K = 192; break;
case 885:
{ K = 886; break; }
case 884:
R[Q++] = 887; K = 211; break;
case 887:
case 886:
K = R[--Q]; break;
K = 784; break;
/* winops */ case 213:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 33;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 888; break; }
R[Q++] = 889; K = 194; break;
case 889:
{ K = 890; break; }
case 888:
R[Q++] = 891; K = 212; break;
case 891:
case 890:
K = R[--Q]; break;
case 784:
K = 892; break;
/* fputc */ case 214:
R[Q++] = 893; K = 120; break;
case 893:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"fputc(S[--P], O)";
S[P++] = 16;
R[Q++] = 894; K = 91; break;
case 894:
K = R[--Q]; break;
K = 892; break;
/* fgetc */ case 215:
R[Q++] = 895; K = 120; break;
case 895:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = fgetc(I)";
S[P++] = 17;
R[Q++] = 896; K = 91; break;
case 896:
K = R[--Q]; break;
K = 892; break;
/* funget */ case 216:
R[Q++] = 897; K = 120; break;
case 897:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"ungetc(S[--P], I)";
S[P++] = 17;
R[Q++] = 898; K = 91; break;
case 898:
K = R[--Q]; break;
K = 892; break;
/* feof */ case 217:
R[Q++] = 899; K = 120; break;
case 899:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = !feof(I)";
S[P++] = 17;
R[Q++] = 900; K = 91; break;
case 900:
K = R[--Q]; break;
K = 892; break;
/* fflush */ case 218:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"fflush(O)";
S[P++] = 9;
R[Q++] = 901; K = 91; break;
case 901:
K = R[--Q]; break;
K = 892; break;
/* fbio */ case 219:
S[P++] = (cell)"A = ";
S[P++] = 4;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = (cell)"((B*)S[P-2], 1, S[P-1], ";
S[P++] = 24;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = (cell)"); P-=2";
S[P++] = 7;
R[Q++] = 902; K = 91; break;
case 902:
K = R[--Q]; break;
K = 892; break;
/* fread */ case 220:
R[Q++] = 903; K = 120; break;
case 903:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"I";
S[P++] = 1;
S[P++] = (cell)"fread";
S[P++] = 5;
R[Q++] = 904; K = 219; break;
case 904:
S[P++] = (cell)"S[P++] = A";
S[P++] = 10;
R[Q++] = 905; K = 91; break;
case 905:
K = R[--Q]; break;
K = 892; break;
/* fwrite */ case 221:
R[Q++] = 906; K = 120; break;
case 906:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"O";
S[P++] = 1;
S[P++] = (cell)"fwrite";
S[P++] = 6;
R[Q++] = 907; K = 219; break;
case 907:
K = R[--Q]; break;
K = 892; break;
/* ftest */ case 222:
R[Q++] = 908; K = 120; break;
case 908:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"A = testfile(); S[P++] = A";
S[P++] = 26;
R[Q++] = 909; K = 91; break;
case 909:
K = R[--Q]; break;
K = 892; break;
/* pcntd */ case 223:
S[P++] = 34;
fputc(S[--P], O);
S[P++] = (cell)"%ld";
S[P++] = 3;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = 34;
fputc(S[--P], O);
K = R[--Q]; break;
K = 892; break;
/* fputnum */ case 224:
R[Q++] = 910; K = 120; break;
case 910:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"fprintf(O, ";
S[P++] = 11;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 911; K = 223; break;
case 911:
S[P++] = (cell)", S[--P])";
S[P++] = 9;
R[Q++] = 912; K = 91; break;
case 912:
K = R[--Q]; break;
K = 892; break;
/* fgetnum */ case 225:
R[Q++] = 913; K = 120; break;
case 913:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"fscanf(I, ";
S[P++] = 10;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 914; K = 223; break;
case 914:
S[P++] = (cell)", &S[P++])";
S[P++] = 10;
R[Q++] = 915; K = 91; break;
case 915:
K = R[--Q]; break;
K = 892; break;
/* ferrout */ case 226:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"O = stderr";
S[P++] = 10;
R[Q++] = 916; K = 91; break;
case 916:
K = R[--Q]; break;
K = 892; break;
/* finput */ case 227:
R[Q++] = 917; K = 120; break;
case 917:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"inputfile()";
S[P++] = 11;
R[Q++] = 918; K = 91; break;
case 918:
K = R[--Q]; break;
K = 892; break;
/* foutput */ case 228:
R[Q++] = 919; K = 120; break;
case 919:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"outputfile()";
S[P++] = 12;
R[Q++] = 920; K = 91; break;
case 920:
K = R[--Q]; break;
K = 892; break;
/* fappnd */ case 229:
R[Q++] = 921; K = 120; break;
case 921:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"appendfile()";
S[P++] = 12;
R[Q++] = 922; K = 91; break;
case 922:
K = R[--Q]; break;
K = 892; break;
/* fsave */ case 230:
R[Q++] = 923; K = 120; break;
case 923:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = (cell)I";
S[P++] = 16;
R[Q++] = 924; K = 91; break;
case 924:
K = R[--Q]; break;
K = 892; break;
/* fresum */ case 231:
R[Q++] = 925; K = 120; break;
case 925:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"I = (FILE*)S[--P]";
S[P++] = 17;
R[Q++] = 926; K = 91; break;
case 926:
K = R[--Q]; break;
K = 892; break;
/* fdel */ case 232:
R[Q++] = 927; K = 120; break;
case 927:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"deletefile()";
S[P++] = 12;
R[Q++] = 928; K = 91; break;
case 928:
K = R[--Q]; break;
K = 892; break;
/* ferr */ case 233:
S[P++] = (cell)"f-op error";
S[P++] = 10;
R[Q++] = 929; K = 76; break;
case 929:
K = R[--Q]; break;
K = 892; break;
/* fcx */ case 234:
S[P] = S[P-1]; P++;
S[P++] = 120;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 930; break; }
R[Q++] = 931; K = 232; break;
case 931:
{ K = 932; break; }
case 930:
R[Q++] = 933; K = 233; break;
case 933:
case 932:
K = R[--Q]; break;
K = 892; break;
/* fcw */ case 235:
S[P] = S[P-1]; P++;
S[P++] = 119;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 934; break; }
R[Q++] = 935; K = 221; break;
case 935:
{ K = 936; break; }
case 934:
R[Q++] = 937; K = 234; break;
case 937:
case 936:
K = R[--Q]; break;
K = 892; break;
/* fct */ case 236:
S[P] = S[P-1]; P++;
S[P++] = 116;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 938; break; }
R[Q++] = 939; K = 222; break;
case 939:
{ K = 940; break; }
case 938:
R[Q++] = 941; K = 235; break;
case 941:
case 940:
K = R[--Q]; break;
K = 892; break;
/* fcr */ case 237:
S[P] = S[P-1]; P++;
S[P++] = 114;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 942; break; }
R[Q++] = 943; K = 220; break;
case 943:
{ K = 944; break; }
case 942:
R[Q++] = 945; K = 236; break;
case 945:
case 944:
K = R[--Q]; break;
K = 892; break;
/* fco */ case 238:
S[P] = S[P-1]; P++;
S[P++] = 111;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 946; break; }
R[Q++] = 947; K = 228; break;
case 947:
{ K = 948; break; }
case 946:
R[Q++] = 949; K = 237; break;
case 949:
case 948:
K = R[--Q]; break;
K = 892; break;
/* fci */ case 239:
S[P] = S[P-1]; P++;
S[P++] = 105;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 950; break; }
R[Q++] = 951; K = 227; break;
case 951:
{ K = 952; break; }
case 950:
R[Q++] = 953; K = 238; break;
case 953:
case 952:
K = R[--Q]; break;
K = 892; break;
/* fcf */ case 240:
S[P] = S[P-1]; P++;
S[P++] = 102;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 954; break; }
R[Q++] = 955; K = 218; break;
case 955:
{ K = 956; break; }
case 954:
R[Q++] = 957; K = 239; break;
case 957:
case 956:
K = R[--Q]; break;
K = 892; break;
/* fce */ case 241:
S[P] = S[P-1]; P++;
S[P++] = 101;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 958; break; }
R[Q++] = 959; K = 226; break;
case 959:
{ K = 960; break; }
case 958:
R[Q++] = 961; K = 240; break;
case 961:
case 960:
K = R[--Q]; break;
K = 892; break;
/* fca */ case 242:
S[P] = S[P-1]; P++;
S[P++] = 97;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 962; break; }
R[Q++] = 963; K = 229; break;
case 963:
{ K = 964; break; }
case 962:
R[Q++] = 965; K = 241; break;
case 965:
case 964:
K = R[--Q]; break;
K = 892; break;
/* f40 */ case 243:
S[P] = S[P-1]; P++;
S[P++] = 64;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 966; break; }
R[Q++] = 967; K = 217; break;
case 967:
{ K = 968; break; }
case 966:
R[Q++] = 969; K = 242; break;
case 969:
case 968:
K = R[--Q]; break;
K = 892; break;
/* f3f */ case 244:
S[P] = S[P-1]; P++;
S[P++] = 63;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 970; break; }
R[Q++] = 971; K = 215; break;
case 971:
{ K = 972; break; }
case 970:
R[Q++] = 973; K = 243; break;
case 973:
case 972:
K = R[--Q]; break;
K = 892; break;
/* f3e */ case 245:
S[P] = S[P-1]; P++;
S[P++] = 62;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 974; break; }
R[Q++] = 975; K = 230; break;
case 975:
{ K = 976; break; }
case 974:
R[Q++] = 977; K = 244; break;
case 977:
case 976:
K = R[--Q]; break;
K = 892; break;
/* f3c */ case 246:
S[P] = S[P-1]; P++;
S[P++] = 60;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 978; break; }
R[Q++] = 979; K = 231; break;
case 979:
{ K = 980; break; }
case 978:
R[Q++] = 981; K = 245; break;
case 981:
case 980:
K = R[--Q]; break;
K = 892; break;
/* f2c */ case 247:
S[P] = S[P-1]; P++;
S[P++] = 44;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 982; break; }
R[Q++] = 983; K = 224; break;
case 983:
{ K = 984; break; }
case 982:
R[Q++] = 985; K = 246; break;
case 985:
case 984:
K = R[--Q]; break;
K = 892; break;
/* f27 */ case 248:
S[P] = S[P-1]; P++;
S[P++] = 39;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 986; break; }
R[Q++] = 987; K = 225; break;
case 987:
{ K = 988; break; }
case 986:
R[Q++] = 989; K = 247; break;
case 989:
case 988:
K = R[--Q]; break;
K = 892; break;
/* f25 */ case 249:
S[P] = S[P-1]; P++;
S[P++] = 37;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 990; break; }
R[Q++] = 991; K = 216; break;
case 991:
{ K = 992; break; }
case 990:
R[Q++] = 993; K = 248; break;
case 993:
case 992:
K = R[--Q]; break;
K = 892; break;
/* fileops */ case 250:
S[P] = S[P-1]; P++;
S[P++] = 33;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 994; break; }
R[Q++] = 995; K = 214; break;
case 995:
{ K = 996; break; }
case 994:
R[Q++] = 997; K = 249; break;
case 997:
case 996:
K = R[--Q]; break;
case 892:
K = 998; break;
/* xerr */ case 251:
S[P++] = (cell)"\\-op error";
S[P++] = 10;
R[Q++] = 999; K = 76; break;
case 999:
K = R[--Q]; break;
K = 998; break;
/* xsaddto */ case 252:
R[Q++] = 1000; K = 120; break;
case 1000:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"*(cell*)S[P-1] += S[P-2]; P-=2";
S[P++] = 30;
R[Q++] = 1001; K = 91; break;
case 1001:
K = R[--Q]; break;
K = 998; break;
/* xqaddto */ case 253:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"*(cell*)";
S[P++] = 8;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1002; K = 126; break;
case 1002:
S[P++] = (cell)" += S[P-1]; --P";
S[P++] = 15;
R[Q++] = 1003; K = 91; break;
case 1003:
K = R[--Q]; break;
K = 998; break;
/* xaddto */ case 254:
S[P++] = *(cell*)M[17];
if (!S[--P]) { K = 1004; break; }
R[Q++] = 1005; K = 253; break;
case 1005:
{ K = 1006; break; }
case 1004:
R[Q++] = 1007; K = 252; break;
case 1007:
case 1006:
K = R[--Q]; break;
K = 998; break;
/* xdec */ case 255:
R[Q++] = 1008; K = 120; break;
case 1008:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P-1]--";
S[P++] = 8;
R[Q++] = 1009; K = 91; break;
case 1009:
K = R[--Q]; break;
K = 998; break;
/* xinc */ case 256:
R[Q++] = 1010; K = 120; break;
case 1010:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P-1]++";
S[P++] = 8;
R[Q++] = 1011; K = 91; break;
case 1011:
K = R[--Q]; break;
K = 998; break;
/* xshl */ case 257:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"<<";
S[P++] = 2;
R[Q++] = 1012; K = 131; break;
case 1012:
K = R[--Q]; break;
K = 998; break;
/* xshr */ case 258:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)">>";
S[P++] = 2;
R[Q++] = 1013; K = 131; break;
case 1013:
K = R[--Q]; break;
K = 998; break;
/* xquit */ case 259:
R[Q++] = 1014; K = 120; break;
case 1014:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"exit(0)";
S[P++] = 7;
R[Q++] = 1015; K = 91; break;
case 1015:
K = R[--Q]; break;
K = 998; break;
/* xstkl */ case 260:
R[Q++] = 1016; K = 120; break;
case 1016:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P] = P; P++";
S[P++] = 13;
R[Q++] = 1017; K = 91; break;
case 1017:
K = R[--Q]; break;
K = 998; break;
/* xcz */ case 261:
S[P] = S[P-1]; P++;
S[P++] = 113;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1018; break; }
R[Q++] = 1019; K = 259; break;
case 1019:
{ K = 1020; break; }
case 1018:
R[Q++] = 1021; K = 251; break;
case 1021:
case 1020:
K = R[--Q]; break;
K = 998; break;
/* xcs */ case 262:
S[P] = S[P-1]; P++;
S[P++] = 115;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1022; break; }
R[Q++] = 1023; K = 260; break;
case 1023:
{ K = 1024; break; }
case 1022:
R[Q++] = 1025; K = 261; break;
case 1025:
case 1024:
K = R[--Q]; break;
K = 998; break;
/* xcq */ case 263:
S[P] = S[P-1]; P++;
S[P++] = 113;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1026; break; }
R[Q++] = 1027; K = 259; break;
case 1027:
{ K = 1028; break; }
case 1026:
R[Q++] = 1029; K = 262; break;
case 1029:
case 1028:
K = R[--Q]; break;
K = 998; break;
/* x3e */ case 264:
S[P] = S[P-1]; P++;
S[P++] = 62;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1030; break; }
R[Q++] = 1031; K = 258; break;
case 1031:
{ K = 1032; break; }
case 1030:
R[Q++] = 1033; K = 263; break;
case 1033:
case 1032:
K = R[--Q]; break;
K = 998; break;
/* x3c */ case 265:
S[P] = S[P-1]; P++;
S[P++] = 60;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1034; break; }
R[Q++] = 1035; K = 257; break;
case 1035:
{ K = 1036; break; }
case 1034:
R[Q++] = 1037; K = 264; break;
case 1037:
case 1036:
K = R[--Q]; break;
K = 998; break;
/* x2d */ case 266:
S[P] = S[P-1]; P++;
S[P++] = 45;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1038; break; }
R[Q++] = 1039; K = 255; break;
case 1039:
{ K = 1040; break; }
case 1038:
R[Q++] = 1041; K = 265; break;
case 1041:
case 1040:
K = R[--Q]; break;
K = 998; break;
/* x2b */ case 267:
S[P] = S[P-1]; P++;
S[P++] = 43;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1042; break; }
R[Q++] = 1043; K = 256; break;
case 1043:
{ K = 1044; break; }
case 1042:
R[Q++] = 1045; K = 266; break;
case 1045:
case 1044:
K = R[--Q]; break;
K = 998; break;
/* extops */ case 268:
S[P] = S[P-1]; P++;
S[P++] = 33;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1046; break; }
R[Q++] = 1047; K = 254; break;
case 1047:
{ K = 1048; break; }
case 1046:
R[Q++] = 1049; K = 267; break;
case 1049:
case 1048:
K = R[--Q]; break;
case 998:
K = 1050; break;
/* rerr */ case 269:
S[P++] = (cell)"r-op error";
S[P++] = 10;
R[Q++] = 1051; K = 76; break;
case 1051:
K = R[--Q]; break;
K = 1050; break;
/* rfrom */ case 270:
R[Q++] = 1052; K = 120; break;
case 1052:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = R[--Q]";
S[P++] = 15;
R[Q++] = 1053; K = 91; break;
case 1053:
K = R[--Q]; break;
K = 1050; break;
/* rto */ case 271:
R[Q++] = 1054; K = 120; break;
case 1054:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"R[Q++] = S[--P]";
S[P++] = 15;
R[Q++] = 1055; K = 91; break;
case 1055:
K = R[--Q]; break;
K = 1050; break;
/* rstkl */ case 272:
R[Q++] = 1056; K = 120; break;
case 1056:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = Q";
S[P++] = 10;
R[Q++] = 1057; K = 91; break;
case 1057:
K = R[--Q]; break;
K = 1050; break;
/* rdrop */ case 273:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"--Q";
S[P++] = 3;
R[Q++] = 1058; K = 91; break;
case 1058:
K = R[--Q]; break;
K = 1050; break;
/* rcx */ case 274:
S[P] = S[P-1]; P++;
S[P++] = 120;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1059; break; }
R[Q++] = 1060; K = 273; break;
case 1060:
{ K = 1061; break; }
case 1059:
R[Q++] = 1062; K = 269; break;
case 1062:
case 1061:
K = R[--Q]; break;
K = 1050; break;
/* rcs */ case 275:
S[P] = S[P-1]; P++;
S[P++] = 115;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1063; break; }
R[Q++] = 1064; K = 272; break;
case 1064:
{ K = 1065; break; }
case 1063:
R[Q++] = 1066; K = 274; break;
case 1066:
case 1065:
K = R[--Q]; break;
K = 1050; break;
/* r3d */ case 276:
S[P] = S[P-1]; P++;
S[P++] = 62;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1067; break; }
R[Q++] = 1068; K = 271; break;
case 1068:
{ K = 1069; break; }
case 1067:
R[Q++] = 1070; K = 275; break;
case 1070:
case 1069:
K = R[--Q]; break;
K = 1050; break;
/* rstkops */ case 277:
S[P] = S[P-1]; P++;
S[P++] = 60;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1071; break; }
R[Q++] = 1072; K = 270; break;
case 1072:
{ K = 1073; break; }
case 1071:
R[Q++] = 1074; K = 276; break;
case 1074:
case 1073:
K = R[--Q]; break;
case 1050:
K = 1075; break;
/* berr */ case 278:
S[P++] = (cell)"b-op error";
S[P++] = 10;
R[Q++] = 1076; K = 76; break;
case 1076:
K = R[--Q]; break;
K = 1075; break;
/* bstor */ case 279:
R[Q++] = 1077; K = 120; break;
case 1077:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"BM[H++] = S[--P]; cm()";
S[P++] = 22;
R[Q++] = 1078; K = 91; break;
case 1078:
K = R[--Q]; break;
K = 1075; break;
/* bsave */ case 280:
R[Q++] = 1079; K = 120; break;
case 1079:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"*(B*)S[P-1] = S[P-2]; P-=2";
S[P++] = 26;
R[Q++] = 1080; K = 91; break;
case 1080:
K = R[--Q]; break;
K = 1075; break;
/* bretr */ case 281:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"*(B*)";
S[P++] = 5;
R[Q++] = 1081; K = 135; break;
case 1081:
K = R[--Q]; break;
K = 1075; break;
/* bcomp */ case 282:
R[Q++] = 1082; K = 120; break;
case 1082:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P-3] = memcmp((B*)S[P-2], ";
S[P++] = 28;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1083; K = 283; break;
case 1083:
K = R[--Q]; break;
K = 1075; break;
/* bcomp2 */ case 283:
S[P++] = (cell)"(B*)S[P-3], S[P-1]); P-=2";
S[P++] = 25;
R[Q++] = 1084; K = 91; break;
case 1084:
K = R[--Q]; break;
K = 1075; break;
/* bmove */ case 284:
R[Q++] = 1085; K = 120; break;
case 1085:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"memmove((B*)S[P-2], (B*)S[P-3], ";
S[P++] = 32;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1086; K = 285; break;
case 1086:
K = R[--Q]; break;
K = 1075; break;
/* bmove2 */ case 285:
S[P++] = (cell)"S[P-1]); P-=3";
S[P++] = 13;
R[Q++] = 1087; K = 91; break;
case 1087:
K = R[--Q]; break;
K = 1075; break;
/* bfill */ case 286:
R[Q++] = 1088; K = 120; break;
case 1088:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"memset((B*)S[P-3], S[P-1], S[P-2]);";
S[P++] = 35;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1089; K = 287; break;
case 1089:
K = R[--Q]; break;
K = 1075; break;
/* bfill2 */ case 287:
S[P++] = (cell)" P-=3";
S[P++] = 5;
R[Q++] = 1090; K = 91; break;
case 1090:
K = R[--Q]; break;
K = 1075; break;
/* bscan */ case 288:
R[Q++] = 1091; K = 120; break;
case 1091:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P-3] = bscan((B*)S[P-3], S[P-2], ";
S[P++] = 35;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1092; K = 289; break;
case 1092:
K = R[--Q]; break;
K = 1075; break;
/* bscan2 */ case 289:
S[P++] = (cell)"S[P-1]); P-=2";
S[P++] = 13;
R[Q++] = 1093; K = 91; break;
case 1093:
K = R[--Q]; break;
K = 1075; break;
/* bcs */ case 290:
S[P] = S[P-1]; P++;
S[P++] = 115;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1094; break; }
R[Q++] = 1095; K = 288; break;
case 1095:
{ K = 1096; break; }
case 1094:
R[Q++] = 1097; K = 278; break;
case 1097:
case 1096:
K = R[--Q]; break;
K = 1075; break;
/* bcm */ case 291:
S[P] = S[P-1]; P++;
S[P++] = 109;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1098; break; }
R[Q++] = 1099; K = 284; break;
case 1099:
{ K = 1100; break; }
case 1098:
R[Q++] = 1101; K = 290; break;
case 1101:
case 1100:
K = R[--Q]; break;
K = 1075; break;
/* bcf */ case 292:
S[P] = S[P-1]; P++;
S[P++] = 102;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1102; break; }
R[Q++] = 1103; K = 286; break;
case 1103:
{ K = 1104; break; }
case 1102:
R[Q++] = 1105; K = 291; break;
case 1105:
case 1104:
K = R[--Q]; break;
K = 1075; break;
/* bcc */ case 293:
S[P] = S[P-1]; P++;
S[P++] = 99;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1106; break; }
R[Q++] = 1107; K = 282; break;
case 1107:
{ K = 1108; break; }
case 1106:
R[Q++] = 1109; K = 292; break;
case 1109:
case 1108:
K = R[--Q]; break;
K = 1075; break;
/* b3f */ case 294:
S[P] = S[P-1]; P++;
S[P++] = 63;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1110; break; }
R[Q++] = 1111; K = 281; break;
case 1111:
{ K = 1112; break; }
case 1110:
R[Q++] = 1113; K = 293; break;
case 1113:
case 1112:
K = R[--Q]; break;
K = 1075; break;
/* b2c */ case 295:
S[P] = S[P-1]; P++;
S[P++] = 44;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1114; break; }
R[Q++] = 1115; K = 279; break;
case 1115:
{ K = 1116; break; }
case 1114:
R[Q++] = 1117; K = 294; break;
case 1117:
case 1116:
K = R[--Q]; break;
K = 1075; break;
/* blkops */ case 296:
S[P] = S[P-1]; P++;
S[P++] = 33;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1118; break; }
R[Q++] = 1119; K = 280; break;
case 1119:
{ K = 1120; break; }
case 1118:
R[Q++] = 1121; K = 295; break;
case 1121:
case 1120:
K = R[--Q]; break;
case 1075:
K = 1122; break;
/* p20 */ case 297:
--P;
S[P++] = fgetc(I);
K = R[--Q]; break;
K = 1122; break;
/* p21 */ case 298:
R[Q++] = 1123; K = 98; break;
case 1123:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"*(cell*)";
S[P++] = 8;
R[Q++] = 1124; K = 139; break;
case 1124:
K = R[--Q]; break;
K = 1122; break;
/* p22 */ case 299:
R[Q++] = 1125; K = 98; break;
case 1125:
R[Q++] = 1126; K = 120; break;
case 1126:
--P;
S[P++] = fgetc(I);
R[Q++] = 1127; K = 81; break;
case 1127:
S[P++] = (cell)"S[P++] = ";
S[P++] = 9;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P] = S[P-1]; P++;
A=S[P-1]; S[P-1]=S[P-2]; S[P-2]=S[P-3]; S[P-3]=A;
R[Q++] = 1128; K = 83; break;
case 1128:
S[P++] = (cell)"";
S[P++] = 0;
R[Q++] = 1129; K = 91; break;
case 1129:
R[Q++] = 1130; K = 116; break;
case 1130:
S[P++] = fgetc(I);
K = R[--Q]; break;
K = 1122; break;
/* p23 */ case 300:
R[Q++] = 1131; K = 98; break;
case 1131:
R[Q++] = 1132; K = 120; break;
case 1132:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 35;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1133; break; }
--P;
S[P++] = fgetc(I);
R[Q++] = 1134; K = 111; break;
case 1134:
{ K = 1135; break; }
case 1133:
S[P++] = (cell)"!";
S[P++] = 1;
R[Q++] = 1136; K = 92; break;
case 1136:
case 1135:
K = R[--Q]; break;
K = 1122; break;
/* p24 */ case 301:
R[Q++] = 1137; K = 98; break;
case 1137:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"/* ";
S[P++] = 3;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = (cell)&BM[H];
S[P++] = 64;
R[Q++] = 1138; K = 19; break;
case 1138:
S[P++] = (cell)&BM[H];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
S[P++] = (cell)" */";
S[P++] = 3;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1139; K = 18; break;
case 1139:
--P;
S[P++] = fgetc(I);
K = R[--Q]; break;
K = 1122; break;
/* p25 */ case 302:
R[Q++] = 1140; K = 98; break;
case 1140:
R[Q++] = 1141; K = 120; break;
case 1141:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"-";
S[P++] = 1;
R[Q++] = 1142; K = 92; break;
case 1142:
K = R[--Q]; break;
K = 1122; break;
/* p26 */ case 303:
R[Q++] = 1143; K = 98; break;
case 1143:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"&";
S[P++] = 1;
R[Q++] = 1144; K = 131; break;
case 1144:
K = R[--Q]; break;
K = 1122; break;
/* p27 */ case 304:
R[Q++] = 1145; K = 98; break;
case 1145:
R[Q++] = 1146; K = 120; break;
case 1146:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = (cell)"([{";
S[P++] = 3;
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P-3] = bscan((B*)S[P-3], S[P-2], S[P-1]); P-=2;
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] < 0;
if (!S[--P]) { K = 1147; break; }
--P;
R[Q++] = 1148; K = 84; break;
case 1148:
R[Q++] = 1149; K = 116; break;
case 1149:
{ K = 1150; break; }
case 1147:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
R[Q++] = 1151; K = 85; break;
case 1151:
case 1150:
K = R[--Q]; break;
K = 1122; break;
/* p28 */ case 305:
R[Q++] = 1152; K = 98; break;
case 1152:
R[Q++] = 1153; K = 120; break;
case 1153:
--P;
S[P++] = fgetc(I);
S[P++] = *(cell*)M[12];
if (!S[--P]) { K = 1154; break; }
S[P++] = (cell)"nested (";
S[P++] = 8;
R[Q++] = 1155; K = 76; break;
case 1155:
{ K = 1156; break; }
case 1154:
R[Q++] = 1157; K = 107; break;
case 1157:
case 1156:
K = R[--Q]; break;
K = 1122; break;
/* p29 */ case 306:
R[Q++] = 1158; K = 98; break;
case 1158:
R[Q++] = 1159; K = 120; break;
case 1159:
--P;
S[P++] = fgetc(I);
S[P++] = *(cell*)M[12];
if (!S[--P]) { K = 1160; break; }
R[Q++] = 1161; K = 109; break;
case 1161:
{ K = 1162; break; }
case 1160:
S[P++] = (cell)"missing (";
S[P++] = 9;
R[Q++] = 1163; K = 76; break;
case 1163:
case 1162:
K = R[--Q]; break;
K = 1122; break;
/* p2a */ case 307:
R[Q++] = 1164; K = 98; break;
case 1164:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"*";
S[P++] = 1;
R[Q++] = 1165; K = 131; break;
case 1165:
K = R[--Q]; break;
K = 1122; break;
/* p2b */ case 308:
R[Q++] = 1166; K = 98; break;
case 1166:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"+";
S[P++] = 1;
R[Q++] = 1167; K = 131; break;
case 1167:
K = R[--Q]; break;
K = 1122; break;
/* p2c */ case 309:
R[Q++] = 1168; K = 98; break;
case 1168:
R[Q++] = 1169; K = 120; break;
case 1169:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"align(); M[H/Z] = S[--P]; H+=Z; cm()";
S[P++] = 36;
R[Q++] = 1170; K = 91; break;
case 1170:
K = R[--Q]; break;
K = 1122; break;
/* p2d */ case 310:
R[Q++] = 1171; K = 98; break;
case 1171:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"-";
S[P++] = 1;
R[Q++] = 1172; K = 131; break;
case 1172:
K = R[--Q]; break;
K = 1122; break;
/* p2e */ case 311:
--P;
S[P++] = fgetc(I);
K = R[--Q]; break;
K = 1122; break;
/* p2f */ case 312:
R[Q++] = 1173; K = 98; break;
case 1173:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 120;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1174; break; }
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"/";
S[P++] = 1;
R[Q++] = 1175; K = 131; break;
case 1175:
{ K = 1176; break; }
case 1174:
R[Q++] = 1177; K = 313; break;
case 1177:
case 1176:
K = R[--Q]; break;
K = 1122; break;
/* p2f2 */ case 313:
R[Q++] = 1178; K = 120; break;
case 1178:
S[P++] = (cell)"A = S[P-2] % S[P-1]; ";
S[P++] = 21;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1179; K = 314; break;
case 1179:
K = R[--Q]; break;
K = 1122; break;
/* p2f3 */ case 314:
S[P++] = (cell)"S[P-2] = S[P-2] / S[P-1]; S[P-1] = A";
S[P++] = 36;
R[Q++] = 1180; K = 91; break;
case 1180:
K = R[--Q]; break;
K = 1122; break;
/* newlab */ case 315:
R[Q++] = 1181; K = 95; break;
case 1181:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 1182; K = 157; break;
case 1182:
*(cell*)S[P-1] = S[P-2]; P-=2;
K = R[--Q]; break;
K = 1122; break;
/* setlab */ case 316:
R[Q++] = 1183; K = 163; break;
case 1183:
S[P] = S[P-1]; P++;
S[P++] = M[26];
R[Q++] = 1184; K = 166; break;
case 1184:
if (!S[--P]) { K = 1185; break; }
S[P] = S[P-1]; P++;
R[Q++] = 1186; K = 315; break;
case 1186:
{ K = 1187; break; }
case 1185:
S[P] = S[P-1]; P++;
S[P++] = M[26];
R[Q++] = 1188; K = 164; break;
case 1188:
case 1187:
K = R[--Q]; break;
K = 1122; break;
/* p3a */ case 317:
R[Q++] = 1189; K = 99; break;
case 1189:
R[Q++] = 1190; K = 121; break;
case 1190:
--P;
S[P++] = fgetc(I);
R[Q++] = 1191; K = 316; break;
case 1191:
R[Q++] = 1192; K = 94; break;
case 1192:
S[P++] = 1;
*(cell*)M[8] = S[--P];
S[P++] = 0;
*(cell*)M[9] = S[--P];
R[Q++] = 1193; K = 122; break;
case 1193:
K = R[--Q]; break;
K = 1122; break;
/* p3b */ case 318:
R[Q++] = 1194; K = 98; break;
case 1194:
R[Q++] = 1195; K = 120; break;
case 1195:
--P;
S[P++] = fgetc(I);
R[Q++] = 1196; K = 95; break;
case 1196:
S[P] = S[P-1]; P++;
R[Q++] = 1197; K = 102; break;
case 1197:
S[P++] = *(cell*)M[11];
R[Q++] = 1198; K = 93; break;
case 1198:
*(cell*)M[11] = S[--P];
K = R[--Q]; break;
K = 1122; break;
/* p3c */ case 319:
R[Q++] = 1199; K = 98; break;
case 1199:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 35;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1200; break; }
--P;
S[P++] = fgetc(I);
S[P++] = (cell)">=";
S[P++] = 2;
R[Q++] = 1201; K = 131; break;
case 1201:
{ K = 1202; break; }
case 1200:
S[P++] = (cell)"<";
S[P++] = 1;
R[Q++] = 1203; K = 131; break;
case 1203:
case 1202:
K = R[--Q]; break;
K = 1122; break;
/* p3d */ case 320:
R[Q++] = 1204; K = 98; break;
case 1204:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 35;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1205; break; }
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"!=";
S[P++] = 2;
R[Q++] = 1206; K = 131; break;
case 1206:
{ K = 1207; break; }
case 1205:
S[P++] = (cell)"==";
S[P++] = 2;
R[Q++] = 1208; K = 131; break;
case 1208:
case 1207:
K = R[--Q]; break;
K = 1122; break;
/* p3e */ case 321:
R[Q++] = 1209; K = 98; break;
case 1209:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 35;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1210; break; }
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"<=";
S[P++] = 2;
R[Q++] = 1211; K = 131; break;
case 1211:
{ K = 1212; break; }
case 1210:
S[P++] = (cell)">";
S[P++] = 1;
R[Q++] = 1213; K = 131; break;
case 1213:
case 1212:
K = R[--Q]; break;
K = 1122; break;
/* p3f */ case 322:
R[Q++] = 1214; K = 98; break;
case 1214:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"*(cell*)";
S[P++] = 8;
R[Q++] = 1215; K = 135; break;
case 1215:
K = R[--Q]; break;
K = 1122; break;
/* p40 */ case 323:
R[Q++] = 1216; K = 98; break;
case 1216:
R[Q++] = 1217; K = 120; break;
case 1217:
--P;
S[P++] = fgetc(I);
R[Q++] = 1218; K = 167; break;
case 1218:
S[P] = S[P-1]; P++;
R[Q++] = 1219; K = 157; break;
case 1219:
S[P-1] = *(cell*)S[P-1];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 1220; K = 158; break;
case 1220:
S[P-1] = *(B*)S[P-1];
S[P++] = 58;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1221; break; }
R[Q++] = 1222; K = 116; break;
case 1222:
{ K = 1223; break; }
case 1221:
R[Q++] = 1224; K = 117; break;
case 1224:
case 1223:
K = R[--Q]; break;
K = 1122; break;
/* p5b */ case 324:
R[Q++] = 1225; K = 98; break;
case 1225:
R[Q++] = 1226; K = 120; break;
case 1226:
--P;
S[P++] = fgetc(I);
S[P++] = *(cell*)M[11];
if (!S[--P]) { K = 1227; break; }
S[P++] = (cell)"nested [";
S[P++] = 8;
R[Q++] = 1228; K = 76; break;
case 1228:
{ K = 1229; break; }
case 1227:
R[Q++] = 1230; K = 103; break;
case 1230:
case 1229:
K = R[--Q]; break;
K = 1122; break;
/* p5c */ case 325:
R[Q++] = 1231; K = 98; break;
case 1231:
--P;
S[P++] = fgetc(I);
R[Q++] = 1232; K = 268; break;
case 1232:
K = R[--Q]; break;
K = 1122; break;
/* p5d */ case 326:
R[Q++] = 1233; K = 98; break;
case 1233:
R[Q++] = 1234; K = 120; break;
case 1234:
--P;
S[P++] = fgetc(I);
S[P++] = *(cell*)M[11];
if (!S[--P]) { K = 1235; break; }
R[Q++] = 1236; K = 104; break;
case 1236:
{ K = 1237; break; }
case 1235:
S[P++] = (cell)"missing [";
S[P++] = 9;
R[Q++] = 1238; K = 76; break;
case 1238:
case 1237:
K = R[--Q]; break;
K = 1122; break;
/* p5e */ case 327:
R[Q++] = 1239; K = 98; break;
case 1239:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"^";
S[P++] = 1;
R[Q++] = 1240; K = 131; break;
case 1240:
K = R[--Q]; break;
K = 1122; break;
/* p5f */ case 328:
R[Q++] = 1241; K = 98; break;
case 1241:
R[Q++] = 1242; K = 120; break;
case 1242:
--P;
S[P++] = fgetc(I);
R[Q++] = 1243; K = 167; break;
case 1243:
S[P] = S[P-1]; P++;
R[Q++] = 1244; K = 157; break;
case 1244:
S[P-1] = *(cell*)S[P-1];
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
R[Q++] = 1245; K = 158; break;
case 1245:
S[P-1] = *(B*)S[P-1];
S[P++] = 58;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1246; break; }
R[Q++] = 1247; K = 100; break;
case 1247:
{ K = 1248; break; }
case 1246:
R[Q++] = 1249; K = 117; break;
case 1249:
case 1248:
K = R[--Q]; break;
K = 1122; break;
/* p60 */ case 329:
R[Q++] = 1250; K = 98; break;
case 1250:
R[Q++] = 1251; K = 120; break;
case 1251:
--P;
S[P++] = fgetc(I);
S[P++] = *(cell*)M[14];
if (!S[--P]) { K = 1252; break; }
S[P++] = (cell)"nested `";
S[P++] = 8;
R[Q++] = 1253; K = 76; break;
case 1253:
{ K = 1254; break; }
case 1252:
R[Q++] = 1255; K = 105; break;
case 1255:
case 1254:
K = R[--Q]; break;
K = 1122; break;
/* pca */ case 330:
R[Q++] = 1256; K = 98; break;
case 1256:
R[Q++] = 1257; K = 120; break;
case 1257:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"H += S[--P]; cm()";
S[P++] = 17;
R[Q++] = 1258; K = 91; break;
case 1258:
K = R[--Q]; break;
K = 1122; break;
/* pcb */ case 331:
R[Q++] = 1259; K = 98; break;
case 1259:
--P;
S[P++] = fgetc(I);
R[Q++] = 1260; K = 296; break;
case 1260:
K = R[--Q]; break;
K = 1122; break;
/* pcc */ case 332:
R[Q++] = 1261; K = 98; break;
case 1261:
--P;
S[P++] = fgetc(I);
R[Q++] = 1262; K = 141; break;
case 1262:
K = R[--Q]; break;
K = 1122; break;
/* pcd */ case 333:
R[Q++] = 1263; K = 98; break;
case 1263:
R[Q++] = 1264; K = 120; break;
case 1264:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P] = S[P-1]; P++";
S[P++] = 18;
R[Q++] = 1265; K = 91; break;
case 1265:
K = R[--Q]; break;
K = 1122; break;
/* pce */ case 334:
R[Q++] = 1266; K = 98; break;
case 1266:
R[Q++] = 1267; K = 120; break;
case 1267:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"R[Q++] = ";
S[P++] = 9;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1268; K = 95; break;
case 1268:
S[P] = S[P-1]; P++;
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"; K = S[--P];";
S[P++] = 13;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1269; K = 335; break;
case 1269:
K = R[--Q]; break;
K = 1122; break;
/* pce2 */ case 335:
S[P++] = (cell)" break";
S[P++] = 6;
R[Q++] = 1270; K = 91; break;
case 1270:
R[Q++] = 1271; K = 93; break;
case 1271:
K = R[--Q]; break;
K = 1122; break;
/* pcf */ case 336:
R[Q++] = 1272; K = 98; break;
case 1272:
--P;
S[P++] = fgetc(I);
R[Q++] = 1273; K = 250; break;
case 1273:
K = R[--Q]; break;
K = 1122; break;
/* pcg */ case 337:
--P;
S[P++] = fgetc(I);
case 1274:
S[P++] = fgetc(I);
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] != S[P-1]; --P;
S[P++] = !feof(I);
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 1275; break; }
{ K = 1274; break; }
case 1275:
--P;
S[P++] = fgetc(I);
K = R[--Q]; break;
K = 1122; break;
/* pch */ case 338:
R[Q++] = 1276; K = 98; break;
case 1276:
R[Q++] = 1277; K = 120; break;
case 1277:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = (cell)&BM[H]";
S[P++] = 21;
R[Q++] = 1278; K = 91; break;
case 1278:
K = R[--Q]; break;
K = 1122; break;
/* pci */ case 339:
R[Q++] = 1279; K = 98; break;
case 1279:
R[Q++] = 1280; K = 120; break;
case 1280:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = R[Q-1]";
S[P++] = 15;
R[Q++] = 1281; K = 91; break;
case 1281:
K = R[--Q]; break;
K = 1122; break;
/* pcj */ case 340:
R[Q++] = 1282; K = 98; break;
case 1282:
R[Q++] = 1283; K = 120; break;
case 1283:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = R[Q-2]";
S[P++] = 15;
R[Q++] = 1284; K = 91; break;
case 1284:
K = R[--Q]; break;
K = 1122; break;
/* pck */ case 341:
R[Q++] = 1285; K = 98; break;
case 1285:
R[Q++] = 1286; K = 120; break;
case 1286:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = ";
S[P++] = 9;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
fprintf(O, "%ld", S[--P]);
S[P++] = fgetc(I);
S[P++] = (cell)"";
S[P++] = 0;
R[Q++] = 1287; K = 91; break;
case 1287:
K = R[--Q]; break;
K = 1122; break;
/* pcl */ case 342:
K = R[--Q]; break;
K = 1122; break;
/* pcm */ case 343:
R[Q++] = 1288; K = 98; break;
case 1288:
R[Q++] = 1289; K = 120; break;
case 1289:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"A = getarg(S[--P]); S[P++] = A";
S[P++] = 30;
R[Q++] = 1290; K = 91; break;
case 1290:
K = R[--Q]; break;
K = 1122; break;
/* pcn */ case 344:
R[Q++] = 1291; K = 98; break;
case 1291:
R[Q++] = 1292; K = 120; break;
case 1292:
--P;
S[P++] = fgetc(I);
R[Q++] = 1293; K = 163; break;
case 1293:
S[P++] = (cell)"M[";
S[P++] = 2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1294; K = 157; break;
case 1294:
S[P-1] = *(cell*)S[P-1];
S[P-1] = S[P-1] / M[1];
fprintf(O, "%ld", S[--P]);
S[P++] = (cell)"] = S[--P]";
S[P++] = 10;
R[Q++] = 1295; K = 91; break;
case 1295:
K = R[--Q]; break;
K = 1122; break;
/* pco */ case 345:
R[Q++] = 1296; K = 98; break;
case 1296:
R[Q++] = 1297; K = 120; break;
case 1297:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P] = S[P-2]; P++";
S[P++] = 18;
R[Q++] = 1298; K = 91; break;
case 1298:
K = R[--Q]; break;
K = 1122; break;
/* pcp */ case 346:
R[Q++] = 1299; K = 98; break;
case 1299:
R[Q++] = 1300; K = 120; break;
case 1300:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P-1] = S[P-S[P-1]-1]";
S[P++] = 22;
R[Q++] = 1301; K = 91; break;
case 1301:
K = R[--Q]; break;
K = 1122; break;
/* pcq */ case 347:
R[Q++] = 1302; K = 98; break;
case 1302:
R[Q++] = 1303; K = 120; break;
case 1303:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"exit(0)";
S[P++] = 7;
R[Q++] = 1304; K = 91; break;
case 1304:
K = R[--Q]; break;
K = 1122; break;
/* pcr */ case 348:
R[Q++] = 1305; K = 98; break;
case 1305:
--P;
S[P++] = fgetc(I);
R[Q++] = 1306; K = 277; break;
case 1306:
K = R[--Q]; break;
K = 1122; break;
/* pcs */ case 349:
R[Q++] = 1307; K = 98; break;
case 1307:
R[Q++] = 1308; K = 120; break;
case 1308:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A";
S[P++] = 39;
R[Q++] = 1309; K = 91; break;
case 1309:
K = R[--Q]; break;
K = 1122; break;
/* pct */ case 350:
R[Q++] = 1310; K = 98; break;
case 1310:
R[Q++] = 1311; K = 120; break;
case 1311:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 116;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1312; break; }
--P;
S[P++] = fgetc(I);
R[Q++] = 1313; K = 113; break;
case 1313:
{ K = 1314; break; }
case 1312:
R[Q++] = 1315; K = 112; break;
case 1315:
case 1314:
K = R[--Q]; break;
K = 1122; break;
/* pcu */ case 351:
R[Q++] = 1316; K = 98; break;
case 1316:
R[Q++] = 1317; K = 120; break;
case 1317:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"S[P++] = rand()";
S[P++] = 15;
R[Q++] = 1318; K = 91; break;
case 1318:
K = R[--Q]; break;
K = 1122; break;
/* pcv */ case 352:
R[Q++] = 1319; K = 98; break;
case 1319:
R[Q++] = 1320; K = 120; break;
case 1320:
--P;
S[P++] = fgetc(I);
R[Q++] = 1321; K = 163; break;
case 1321:
S[P++] = (cell)"align(); M[";
S[P++] = 11;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1322; K = 157; break;
case 1322:
S[P-1] = *(cell*)S[P-1];
S[P-1] = S[P-1] / M[1];
fprintf(O, "%ld", S[--P]);
R[Q++] = 1323; K = 353; break;
case 1323:
K = R[--Q]; break;
K = 1122; break;
/* pcv2 */ case 353:
S[P++] = (cell)"] = (cell)&M[H/Z]";
S[P++] = 17;
R[Q++] = 1324; K = 91; break;
case 1324:
S[P++] = (cell)"M[H/Z] = S[--P]; H+=Z; cm()";
S[P++] = 27;
R[Q++] = 1325; K = 91; break;
case 1325:
K = R[--Q]; break;
K = 1122; break;
/* pcw */ case 354:
R[Q++] = 1326; K = 98; break;
case 1326:
S[P++] = *(cell*)M[16];
if (!S[--P]) { K = 1327; break; }
R[Q++] = 1328; K = 213; break;
case 1328:
{ K = 1329; break; }
case 1327:
R[Q++] = 1330; K = 77; break;
case 1330:
case 1329:
K = R[--Q]; break;
K = 1122; break;
/* pcx */ case 355:
R[Q++] = 1331; K = 98; break;
case 1331:
R[Q++] = 1332; K = 120; break;
case 1332:
--P;
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 120;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1333; break; }
--P;
S[P++] = fgetc(I);
R[Q++] = 1334; K = 114; break;
case 1334:
{ K = 1335; break; }
case 1333:
S[P++] = (cell)"--P";
S[P++] = 3;
R[Q++] = 1336; K = 91; break;
case 1336:
case 1335:
K = R[--Q]; break;
K = 1122; break;
/* pcy */ case 356:
R[Q++] = 1337; K = 98; break;
case 1337:
R[Q++] = 1338; K = 120; break;
case 1338:
--P;
S[P++] = fgetc(I);
S[P++] = *(cell*)M[8];
R[Q++] = 1339; K = 101; break;
case 1339:
*(cell*)M[8] = S[--P];
K = R[--Q]; break;
K = 1122; break;
/* pcz */ case 357:
R[Q++] = 1340; K = 98; break;
case 1340:
R[Q++] = 1341; K = 120; break;
case 1341:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"P = 0";
S[P++] = 5;
R[Q++] = 1342; K = 91; break;
case 1342:
K = R[--Q]; break;
K = 1122; break;
/* p7b */ case 358:
R[Q++] = 1343; K = 98; break;
case 1343:
R[Q++] = 1344; K = 120; break;
case 1344:
--P;
S[P++] = fgetc(I);
R[Q++] = 1345; K = 95; break;
case 1345:
S[P] = S[P-1]; P++;
*(cell*)M[15] = S[--P];
S[P++] = (cell)"if (!S[--P]) ";
S[P++] = 13;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1346; K = 102; break;
case 1346:
K = R[--Q]; break;
K = 1122; break;
/* p7c */ case 359:
R[Q++] = 1347; K = 98; break;
case 1347:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"|";
S[P++] = 1;
R[Q++] = 1348; K = 131; break;
case 1348:
K = R[--Q]; break;
K = 1122; break;
/* p7d */ case 360:
R[Q++] = 1349; K = 98; break;
case 1349:
R[Q++] = 1350; K = 120; break;
case 1350:
--P;
S[P++] = fgetc(I);
S[P++] = *(cell*)M[15];
S[P-1] = S[P-1] != 0;
S[P++] = *(cell*)M[14];
S[P-1] = S[P-1] != 0;
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 1351; break; }
R[Q++] = 1352; K = 106; break;
case 1352:
{ K = 1353; break; }
case 1351:
S[P++] = (cell)"missing ` or {";
S[P++] = 14;
R[Q++] = 1354; K = 76; break;
case 1354:
case 1353:
K = R[--Q]; break;
K = 1122; break;
/* p7e */ case 361:
R[Q++] = 1355; K = 98; break;
case 1355:
R[Q++] = 1356; K = 120; break;
case 1356:
--P;
S[P++] = fgetc(I);
S[P++] = (cell)"~";
S[P++] = 1;
R[Q++] = 1357; K = 92; break;
case 1357:
K = R[--Q]; break;
case 1122:
S[P++] = 0;
align(); M[27] = (cell)&M[H/Z];
M[H/Z] = S[--P]; H+=Z; cm();
R[Q++] = 1359; K = 15; break;
case 1359:
S[P++] = 297;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 298;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 299;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 300;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 301;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 302;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 303;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 304;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 305;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 306;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 307;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 308;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 309;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 310;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 311;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 312;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 77;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 317;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 318;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 319;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 320;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 321;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 322;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 323;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 330;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 331;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 332;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 333;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 334;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 336;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 337;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 338;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 339;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 340;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 341;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 342;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 343;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 344;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 345;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 346;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 347;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 348;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 349;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 350;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 351;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 352;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 354;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 355;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 356;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 357;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 324;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 325;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 326;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 327;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 328;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 329;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 330;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 331;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 332;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 333;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 334;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 336;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 337;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 338;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 339;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 340;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 341;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 342;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 343;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 344;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 345;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 346;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 347;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 348;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 349;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 350;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 351;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 352;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 354;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 355;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 356;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 357;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 358;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 359;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 360;
align(); M[H/Z] = S[--P]; H+=Z; cm();
S[P++] = 361;
align(); M[H/Z] = S[--P]; H+=Z; cm();
K = 1358; break;
/* fdisp */ case 362:
S[P] = S[P-1]; P++;
S[P++] = 32;
S[P++] = 126;
R[Q++] = 1360; K = 8; break;
case 1360:
if (!S[--P]) { K = 1361; break; }
S[P] = S[P-1]; P++;
S[P++] = 32;
S[P-2] = S[P-2] - S[P-1]; --P;
S[P-1] *= Z;
S[P-1] = S[P-1] + M[27];
S[P-1] = *(cell*)S[P-1];
R[Q++] = 1362; K = S[--P]; break;
case 1362:
{ K = 1363; break; }
case 1361:
R[Q++] = 1364; K = 77; break;
case 1364:
case 1363:
K = R[--Q]; break;
case 1358:
K = 1365; break;
/* oper */ case 363:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] == 10;
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] == 13;
S[P-2] = S[P-2] | S[P-1]; --P;
if (!S[--P]) { K = 1366; break; }
--P;
S[P++] = fgetc(I);
R[Q++] = 1367; K = 101; break;
case 1367:
{ K = 1368; break; }
case 1366:
R[Q++] = 1369; K = 362; break;
case 1369:
case 1368:
K = R[--Q]; break;
K = 1365; break;
/* cklen */ case 364:
S[P-1] = S[P-1] > 61;
if (!S[--P]) { K = 1370; break; }
S[P++] = (cell)"file name too long";
S[P++] = 18;
R[Q++] = 1371; K = 77; break;
case 1371:
case 1370:
K = R[--Q]; break;
K = 1365; break;
/* outfile */ case 365:
S[P++] = *(cell*)M[2];
A = getarg(S[--P]); S[P++] = A;
S[P] = S[P-1]; P++;
if (!S[--P]) { K = 1372; break; }
S[P] = S[P-1]; P++;
R[Q++] = 1373; K = 364; break;
case 1373:
S[P++] = M[3];
R[Q++] = 1374; K = 44; break;
case 1374:
S[P++] = (cell)".c";
S[P++] = 2;
R[Q++] = 1375; K = 45; break;
case 1375:
outputfile();
case 1372:
K = R[--Q]; break;
K = 1365; break;
/* compile */ case 366:
R[Q++] = 1376; K = 71; break;
case 1376:
inputfile();
S[P++] = fgetc(I);
case 1377:
S[P++] = !feof(I);
if (!S[--P]) { K = 1378; break; }
R[Q++] = 1379; K = 363; break;
case 1379:
{ K = 1377; break; }
case 1378:
R[Q++] = 1380; K = 120; break;
case 1380:
S[P++] = (cell)"";
S[P++] = 0;
inputfile();
--P;
K = R[--Q]; break;
K = 1365; break;
/* pass2 */ case 367:
R[Q++] = 1381; K = 365; break;
case 1381:
R[Q++] = 1382; K = 148; break;
case 1382:
R[Q++] = 1383; K = 366; break;
case 1383:
R[Q++] = 1384; K = 153; break;
case 1384:
S[P++] = (cell)"";
S[P++] = 0;
outputfile();
K = R[--Q]; break;
case 1365:
K = 1385; break;
/* skipstr */ case 368:
S[P++] = 0;
case 1386:
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 34;
S[P-2] = S[P-2] != S[P-1]; --P;
S[P++] = !feof(I);
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 1387; break; }
S[P] = S[P-1]; P++;
fputc(S[--P], O);
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] + M[3];
*(B*)S[P-1] = S[P-2]; P-=2;
S[P-1]++;
{ K = 1386; break; }
case 1387:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
*(cell*)M[4] = S[--P];
K = R[--Q]; break;
K = 1385; break;
/* skipsym */ case 369:
S[P++] = 0;
case 1388:
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
R[Q++] = 1389; K = 79; break;
case 1389:
S[P++] = !feof(I);
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 1390; break; }
S[P] = S[P-1]; P++;
fputc(S[--P], O);
S[P] = S[P-2]; P++;
S[P-1] = S[P-1] + M[3];
*(B*)S[P-1] = S[P-2]; P-=2;
S[P-1]++;
{ K = 1388; break; }
case 1390:
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
*(cell*)M[4] = S[--P];
K = R[--Q]; break;
K = 1385; break;
/* skipeol */ case 370:
case 1391:
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] != 10;
S[P++] = !feof(I);
S[P-2] = S[P-2] & S[P-1]; --P;
if (!S[--P]) { K = 1392; break; }
fputc(S[--P], O);
S[P++] = fgetc(I);
{ K = 1391; break; }
case 1392:
K = R[--Q]; break;
K = 1385; break;
/* loadlib */ case 371:
S[P++] = *(cell*)M[5];
S[P++] = (cell)I;
S[P++] = M[3];
S[P++] = *(cell*)M[4];
inputfile();
R[Q++] = 1393; K = 387; break;
case 1393:
S[P++] = (cell)"";
S[P++] = 0;
inputfile();
I = (FILE*)S[--P];
R[Q++] = 1394; K = 174; break;
case 1394:
K = R[--Q]; break;
K = 1385; break;
/* declrtn */ case 372:
fputc(S[--P], O);
S[P++] = fgetc(I);
S[P++] = 58;
R[Q++] = 1395; K = 95; break;
case 1395:
R[Q++] = 1396; K = 169; break;
case 1396:
R[Q++] = 1397; K = 370; break;
case 1397:
K = R[--Q]; break;
K = 1385; break;
/* declnam */ case 373:
fputc(S[--P], O);
S[P++] = fgetc(I);
S[P++] = 110;
R[Q++] = 1398; K = 96; break;
case 1398:
R[Q++] = 1399; K = 169; break;
case 1399:
K = R[--Q]; break;
K = 1385; break;
/* declvar */ case 374:
R[Q++] = 1400; K = 373; break;
case 1400:
K = R[--Q]; break;
K = 1385; break;
/* dostrng */ case 375:
fputc(S[--P], O);
R[Q++] = 1401; K = 368; break;
case 1401:
fputc(S[--P], O);
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = 108;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1402; break; }
--P;
S[P++] = (cell)"xx";
S[P++] = 2;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1403; K = 18; break;
case 1403:
R[Q++] = 1404; K = 371; break;
case 1404:
S[P++] = fgetc(I);
case 1402:
K = R[--Q]; break;
K = 1385; break;
/* skipcom */ case 376:
S[P++] = (cell)")]}";
S[P++] = 3;
--P;
S[P-2] = S[P-2] + S[P-1]; --P;
S[P-1] = *(B*)S[P-1];
case 1405:
S[P++] = fgetc(I);
S[P] = S[P-2]; P++;
S[P-2] = S[P-2] != S[P-1]; --P;
if (!S[--P]) { K = 1406; break; }
{ K = 1405; break; }
case 1406:
fputc(S[--P], O);
S[P++] = fgetc(I);
K = R[--Q]; break;
K = 1385; break;
/* maybcmnt */ case 377:
fputc(S[--P], O);
S[P++] = fgetc(I);
S[P] = S[P-1]; P++;
S[P++] = (cell)"([{";
S[P++] = 3;
A=S[P-3]; S[P-3]=S[P-2]; S[P-2]=S[P-1]; S[P-1]=A;
S[P-3] = bscan((B*)S[P-3], S[P-2], S[P-1]); P-=2;
S[P] = S[P-1]; P++;
S[P-1] = S[P-1] >= 0;
if (!S[--P]) { K = 1407; break; }
S[P] = S[P-2]; P++;
fputc(S[--P], O);
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
R[Q++] = 1408; K = 376; break;
case 1408:
{ K = 1409; break; }
case 1407:
--P;
case 1409:
K = R[--Q]; break;
K = 1385; break;
/* dcl5f */ case 378:
S[P] = S[P-1]; P++;
S[P++] = 95;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1410; break; }
fputc(S[--P], O);
R[Q++] = 1411; K = 369; break;
case 1411:
{ K = 1412; break; }
case 1410:
fputc(S[--P], O);
S[P++] = fgetc(I);
case 1412:
K = R[--Q]; break;
K = 1385; break;
/* dcl40 */ case 379:
S[P] = S[P-1]; P++;
S[P++] = 64;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1413; break; }
fputc(S[--P], O);
R[Q++] = 1414; K = 369; break;
case 1414:
{ K = 1415; break; }
case 1413:
R[Q++] = 1416; K = 378; break;
case 1416:
case 1415:
K = R[--Q]; break;
K = 1385; break;
/* dcl3a */ case 380:
S[P] = S[P-1]; P++;
S[P++] = 58;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1417; break; }
R[Q++] = 1418; K = 372; break;
case 1418:
{ K = 1419; break; }
case 1417:
R[Q++] = 1420; K = 379; break;
case 1420:
case 1419:
K = R[--Q]; break;
K = 1385; break;
/* dcln */ case 381:
S[P] = S[P-1]; P++;
S[P++] = 110;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1421; break; }
R[Q++] = 1422; K = 373; break;
case 1422:
{ K = 1423; break; }
case 1421:
R[Q++] = 1424; K = 380; break;
case 1424:
case 1423:
K = R[--Q]; break;
K = 1385; break;
/* dclv */ case 382:
S[P] = S[P-1]; P++;
S[P++] = 118;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1425; break; }
R[Q++] = 1426; K = 374; break;
case 1426:
{ K = 1427; break; }
case 1425:
R[Q++] = 1428; K = 381; break;
case 1428:
case 1427:
K = R[--Q]; break;
K = 1385; break;
/* dclg */ case 383:
S[P] = S[P-1]; P++;
S[P++] = 103;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1429; break; }
R[Q++] = 1430; K = 337; break;
case 1430:
{ K = 1431; break; }
case 1429:
R[Q++] = 1432; K = 382; break;
case 1432:
case 1431:
K = R[--Q]; break;
K = 1385; break;
/* dclpf */ case 384:
S[P] = S[P-1]; P++;
S[P++] = (cell)"bfkrw\\";
S[P++] = 6;
R[Q++] = 1433; K = 33; break;
case 1433:
if (!S[--P]) { K = 1434; break; }
fputc(S[--P], O);
S[P++] = fgetc(I);
fputc(S[--P], O);
S[P++] = fgetc(I);
{ K = 1435; break; }
case 1434:
R[Q++] = 1436; K = 383; break;
case 1436:
case 1435:
K = R[--Q]; break;
K = 1385; break;
/* dcl27 */ case 385:
S[P] = S[P-1]; P++;
S[P++] = 39;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1437; break; }
R[Q++] = 1438; K = 377; break;
case 1438:
{ K = 1439; break; }
case 1437:
R[Q++] = 1440; K = 384; break;
case 1440:
case 1439:
K = R[--Q]; break;
K = 1385; break;
/* decl */ case 386:
S[P] = S[P-1]; P++;
S[P++] = 34;
S[P-2] = S[P-2] == S[P-1]; --P;
if (!S[--P]) { K = 1441; break; }
R[Q++] = 1442; K = 375; break;
case 1442:
{ K = 1443; break; }
case 1441:
R[Q++] = 1444; K = 385; break;
case 1444:
case 1443:
K = R[--Q]; break;
K = 1385; break;
/* decls */ case 387:
S[P++] = fgetc(I);
case 1445:
S[P++] = !feof(I);
if (!S[--P]) { K = 1446; break; }
R[Q++] = 1447; K = 386; break;
case 1447:
{ K = 1445; break; }
case 1446:
--P;
K = R[--Q]; break;
K = 1385; break;
/* ldulib */ case 388:
S[P++] = *(cell*)M[5];
S[P++] = (cell)I;
R[Q++] = 1448; K = 72; break;
case 1448:
R[Q++] = 1449; K = 89; break;
case 1449:
R[Q++] = 1450; K = 387; break;
case 1450:
S[P++] = (cell)"";
S[P++] = 0;
inputfile();
I = (FILE*)S[--P];
R[Q++] = 1451; K = 174; break;
case 1451:
K = R[--Q]; break;
K = 1385; break;
/* marker */ case 389:
S[P++] = (cell)"$cSTART";
S[P++] = 7;
A = fwrite((B*)S[P-2], 1, S[P-1], O); P-=2;
R[Q++] = 1452; K = 18; break;
case 1452:
K = R[--Q]; break;
K = 1385; break;
/* pass1 */ case 390:
S[P++] = *(cell*)M[2];
A = getarg(S[--P]); S[P++] = A;
inputfile();
R[Q++] = 1453; K = 71; break;
case 1453:
outputfile();
R[Q++] = 1454; K = 388; break;
case 1454:
R[Q++] = 1455; K = 389; break;
case 1455:
R[Q++] = 1456; K = 387; break;
case 1456:
S[P++] = (cell)"";
S[P++] = 0;
outputfile();
S[P++] = (cell)"";
S[P++] = 0;
inputfile();
K = R[--Q]; break;
case 1385:
K = 1457; break;
/* comp */ case 391:
R[Q++] = 1458; K = 390; break;
case 1458:
R[Q++] = 1459; K = 367; break;
case 1459:
R[Q++] = 1460; K = 172; break;
case 1460:
R[Q++] = 1461; K = 173; break;
case 1461:
R[Q++] = 1462; K = 71; break;
case 1462:
deletefile();
K = R[--Q]; break;
case 1457:
S[P++] = 0;
A = getarg(S[--P]); S[P++] = A;
S[P++] = (cell)"-w";
S[P++] = 2;
R[Q++] = 1464; K = 31; break;
case 1464:
if (!S[--P]) { K = 1465; break; }
S[P++] = 1;
*(cell*)M[16] = S[--P];
S[P++] = 1;
*(cell*)M[2] = S[--P];
case 1465:
R[Q++] = 1466; K = 391; break;
case 1466:
S[P++] = 0;
A = getarg(S[--P]); S[P++] = A;
A = S[P-2]; S[P-2] = S[P-1]; S[P-1] = A;
--P;
S[P-1] = !S[P-1];
if (!S[--P]) { K = 1467; break; }
exit(0);
case 1467:
exit(0);
default: wrongip(); break;
}
}
