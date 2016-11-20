/* lib.c */


static X
fail(char *msg, ...)
{
  va_list va;

  va_start(va, msg);
  vsnprintf(message_buffer, MESSAGE_BUFFER_SIZE, msg, va);
  va_end(va);
  ____5fthrow(sequence(2, ERROR, string(message_buffer)));
}


static X
failx(X x, char *msg, ...)
{
  va_list va;
  X s;

  save(x);
  va_start(va, msg);
  vsnprintf(message_buffer, MESSAGE_BUFFER_SIZE, msg, va);
  va_end(va);
  s = string(message_buffer);
  ____5fthrow(sequence(3, ERROR, s, restore()));
}


static X
uncaught(X x)
{
  fputs("\nUncaught:\n", error_output);
  dump(x, error_output, 1);
  dumptrace(error_output);
  exit(EXIT_FAILURE);
  return F;
}


#ifndef UNSAFE
static long
check_N(X x, char *loc)
{
  CHECK_FPTR(x);

  if(!IS_N(x))
    failx(x, "bad argument type in `%s' - not a number", loc);

  return N_VALUE(x);
}


static char *
check_A(X x, char *loc)
{
  CHECK_FPTR(x);

  if(!IS_A(x))
    failx(x, "bad argument type in `%s' - not an atom", loc);

  return A_STRING(x);
}


static X *
check_S(X x, char *loc)
{
  CHECK_FPTR(x);

  if(!IS_S(x))
    failx(x, "bad argument type in `%s' - not a sequence", loc);

  return S_DATA(x);
}


static X *
check_Sn(X x, unsigned long len, char *loc)
{
  CHECK_FPTR(x);

  if(!IS_S(x))
    failx(x, "bad argument type in `%s' - not a sequence", loc);

  if(S_LENGTH(x) != len)
    failx(x, "bad argument type in `%s' - not a sequence of length %d", loc, len);

  return S_DATA(x);
}


static X *
check_Smin(X x, unsigned long minlen, char *loc)
{
  CHECK_FPTR(x);

  if(!IS_S(x))
    failx(x, "bad argument type in `%s' - not a sequence", loc);

  if(S_LENGTH(x) < minlen)
    failx(x, "bad argument type in `%s' - not a sequence of length %d or greater", 
	 loc, minlen);

  return S_DATA(x);
}
#endif


static char *
resize_buffer(int len)
{
  if(len > buffer_size) {
    buffer = (char *)realloc(buffer, len);

    if(buffer == NULL)
      fail("can not re-allocate temporary buffer from %d to %d bytes",
	   buffer_size, len);

    buffer_size = len;
  }

  return buffer;
}


static char *
check_string(X x, char *loc, int nl)
{
  CHECK_FPTR(x);

  if(IS_A(x)) return A_STRING(x);
  else if(is_string(x, nl)) {
    int i, len = S_LENGTH(x);
    char *ptr = resize_buffer(len);

    for(i = 0; i < len; ++i) {
      X e = S_DATA(x)[ i ];
      *(ptr++) = N_VALUE(e);
    }

    *ptr = '\0';
    return buffer;
  }
  else {
    failx(x, "bad argument type in `%s' - not a string or atom", loc);
    return NULL;
  }
}


X
fpointer(void *fptr)
{
  unsigned long addr = (unsigned long)fptr;

  return sequence(3, FPTR, TO_N(addr >> 16), TO_N(addr & 0xffff));
}


#define op2(name, op) \
  OP2DEFINE(___ ## name) { \
    return TO_N(check_N(x, #name) op check_N(y, #name)); \
  } \
  DEFINE(___ ## name) { \
    X *a = check_Sn(x, 2, #name); \
    return ___ ## name ## _5fop2(a[ 0 ], a[ 1 ]); \
  }

op2(add, +)
op2(sub, -)
op2(mul, *)
op2(band, &)
op2(bor, |)
op2(bxor, ^)
op2(bshl, <<)
op2(bshr, >>)


OP2DEFINE(___div)
{
  long q = check_N(y, "div");

  if(N_VALUE(q) == 0)
    failx(y, "division by zero", "div");

  return TO_N(check_N(x, "div") / q);
}

DEFINE(___div) {
  X *a = check_Sn(x, 2, "div");
  return ___div_5fop2(a[ 0 ], a[ 1 ]);
}  


OP2DEFINE(___mod)
{
  long q = check_N(y, "mod");

  if(N_VALUE(q) == 0)
    failx(y, "division by zero", "mod");

  return TO_N(check_N(x, "mod") % q);
}

DEFINE(___mod) {
  X *a = check_Sn(x, 2, "mod");
  return ___mod_5fop2(a[ 0 ], a[ 1 ]);
}  


DEFINE(___bnot) { return TO_N(~check_N(x, "bnot")); }
DEFINE(___id) { return x; }


static int 
eq1(X x, X y) 
{
 loop:
  if(x == y) return 1;

  if(IS_S(x) && IS_S(y)) {
    int i, len = S_LENGTH(x);

    if(S_LENGTH(y) != len) return 0;
    else if(len == 0) return 1;

    --len;

    for(i = 0; i < len; ++i) {
      if(!eq1(S_DATA(x)[ i ] , S_DATA(y)[ i ])) return 0;
    }

    x = S_DATA(x)[ i ];
    y = S_DATA(y)[ i ];
    goto loop;
  }

  return 0;
}


DEFINE(___eq)
{ 
  X *s = check_Sn(x, 2, "eq"); 
  return TO_B(eq1(s[ 0 ], s[ 1 ]));
}


static int
compare(X x, X y)
{
  if(IS_N(x)) {
    if(IS_N(y)) {
      if(x == y) return 0;	/* N1 <=> N2 */
      else if(N_VALUE(x) > N_VALUE(y)) return 1;
      else return -1;
    }
    else return -1;
  }
  else if(IS_A(x)) {
    if(IS_N(y)) return 1;	/* A <=> N */
    else if(IS_A(y)) return strcmp(A_STRING(x), A_STRING(y)); /* A1 <=> A1 */
    else return -1;					      /* A <=> S */
  }
  else if(!IS_S(y)) return 1;	/* S <=> A/N */
  else {
    int i, r;			/* S1 <=> S2 */

    for(i = 0; i < S_LENGTH(x); ++i) {
      if(i >= S_LENGTH(y)) return 1;

      r = compare(S_DATA(x)[ i ], S_DATA(y)[ i ]);

      if(r != 0) return r;
    }

    return 0;
  }
}


OP2DEFINE(___cmp)
{
  return TO_N(compare(x, y));
}

DEFINE(___cmp)
{
  X *s = check_Sn(x, 2, "cmp");
  return TO_N(compare(s[ 0 ], s[ 1 ]));
}


DEFINE(____5f)
{ 
  fputs("\n_|_:\n", error_output);
  dump(x, error_output, 1);
  dumptrace(error_output);
  exit(EXIT_FAILURE);
  return F;
}


DEFINE(____25trace)
{
  X *s = check_Sn(x, 2, "%trace");
  fprintf(error_output, "| %s: ", check_A(s[ 0 ], "%trace"));

  if(IS_S(s[ 1 ])) fputc('\n', error_output);

  dump(s[ 1 ], error_output, 1);
  return s[ 1 ];
}


DEFINE(____5fin)
{
  char *fn = check_string(x, "_in", 0);
  struct stat sb;
  int len, i;
  X buf, *ptr;
  FILE *fp;

  if(stat(fn, &sb) != 0) 
    fail("can not read file `%s': %s", fn, strerror(errno));

  if((sb.st_mode & S_IFMT) == S_IFDIR) 
    fail("can not read from directory `%s'", fn);

  len = sb.st_size;
  buf = allocate(len);
  ptr = S_DATA(buf);
  
  if((fp = fopen(fn, "rb")) == NULL)
    fail("can not open file `%s': %s", fn, strerror(errno));

  while(len > 0) {
    int n = len < buffer_size ? len : buffer_size;

    if(fread(buffer, n, 1, fp) != 1)
      fail("error while reading `%s': %s", fn, strerror(errno));

    len -= n;
    
    for(i = 0; i < n; ++i)
      *(ptr++) = TO_N(buffer[ i ]);
  }

  fclose(fp);
  return buf;
}


DEFINE(___al)
{
  X *a = check_Sn(x, 2, "al");
  X *b = check_S(a[ 1 ],  "al");
  int len = S_LENGTH(a[ 1 ]);
  X s;
  int i;

  save(a[ 0 ]);
  save(a[ 1 ]);
  s = allocate(len + 1);
  b = S_DATA(restore());
  
  for(i = 0; i < len; ++i)
    S_DATA(s)[ i + 1 ] = b[ i ];

  S_DATA(s)[ 0 ] = restore();
  return s;
}
  

DEFINE(___ar)
{
  X *a = check_Sn(x, 2, "ar");
  X *b = check_S(a[ 0 ],  "ar");
  int len = S_LENGTH(a[ 0 ]);
  X s;
  int i;

  save(a[ 1 ]);
  save(a[ 0 ]);
  s = allocate(len + 1);
  b = S_DATA(restore());
  
  for(i = 0; i < len; ++i)
    S_DATA(s)[ i ] = b[ i ];

  S_DATA(s)[ i ] = restore();
  return s;
}


DEFINE(___tl)
{
  X *a = check_Smin(x, 1, "tl");
  X s;
  int len = S_LENGTH(x);
  int i;

  save(x);
  s = allocate(len - 1);
  x = restore();
  
  for(i = 1; i < len; ++i)
    S_DATA(s)[ i - 1 ] = S_DATA(x)[ i ];

  return s;
}


DEFINE(___cat)
{
  X *a = check_S(x, "cat");
  X s, s2;
  int len = S_LENGTH(x);
  int i, j, len2, slen = 0;

  for(i = 0; i < len; ++i) {
    check_S(a[ i ], "cat");
    slen += S_LENGTH(a[ i ]);
  }

  save(x);
  s = allocate(slen);
  x = restore();
  slen = 0;

  for(i = 0; i < len; ++i) {
    s2 = S_DATA(x)[ i ];
    len2 = S_LENGTH(s2);

    for(j = 0; j < len2; ++j)
      S_DATA(s)[ slen++ ] = S_DATA(s2)[ j ];
  }

  return s;
}


DEFINE(___len) { check_S(x, "len"); return TO_N(S_LENGTH(x)); }
DEFINE(___atom) { return TO_B(!IS_S(x)); }
DEFINE(___num) { return TO_B(IS_N(x)); }


DEFINE(___tos)
{
  X s;
  char *ptr;
  int i, len;

  if(IS_S(x)) return x;

  if(IS_A(x)) ptr = A_STRING(x);
  else {
    snprintf(buffer, buffer_size, "%d", N_VALUE(x));
    ptr = buffer;
  }

  len = strlen(ptr);
  s = allocate(len);
  
  for(i = 0; i < len; ++i)
    S_DATA(s)[ i ] = TO_N(*(ptr++));
  
  return s;
}


DEFINE(___ton)
{
  int i, len;
  X e;
  char *ptr;

  if(IS_N(x)) return x;

  if(IS_A(x)) return TO_N(atol(A_STRING(x)));

  len = S_LENGTH(x);
  ptr = buffer;

  if(len > buffer_size) len = buffer_size - 1;

  for(i = 0; i < len; ++i) {
    e = S_DATA(x)[ i ];

    if(!IS_N(e) || N_VALUE(e) >= 0x100 || !isdigit(N_VALUE(e))) break;
    else *(ptr++) = N_VALUE(e);
  }

  *ptr = '\0';
  return TO_N(atol(buffer));
}


DEFINE(___toa)
{
  int i, len;
  char *ptr;
  X e;

  if(IS_A(x)) return x;

  if(IS_N(x)) 
    snprintf(buffer, buffer_size, "%d", N_VALUE(x));
  else {
    len = S_LENGTH(x);
    ptr = resize_buffer(len);

    for(i = 0; i < len; ++i) {
      e = S_DATA(x)[ i ];
      
      if(!IS_N(e))
	failx(x, "bad argument type in `toa' - not a string");
      else *(ptr++) = N_VALUE(e);
    }

    *ptr = '\0';
  }

  return intern(buffer);
}


DEFINE(____5femit)
{
  if(IS_N(x)) putchar(N_VALUE(x));
  else if(IS_A(x)) fputs(A_STRING(x), stdout);
  else {
    int i, len = S_LENGTH(x);

    for(i = 0; i < len; ++i) 
      ____5femit(S_DATA(x)[ i ]);
  }

  fflush(stdout);
  return x;
}


DEFINE(____5fget)
{
  int i, n = check_N(x, "_get");
  char *ptr = buffer;

  if(n >= buffer_size)
    failx(x, "can not `get' more than %d bytes at once", buffer_size);

  for(i = 0; i < n; ++i) {
    int c = getchar();

    if(c == EOF) { 
      n = i;
      break;
    }
    else *(ptr++) = getchar();  
  }

  x = allocate(n);

  for(i = 0; i < n; ++i)
    S_DATA(x)[ i ] = TO_N(buffer[ i ]);

  return x;
}


DEFINE(____5fout)
{
  X *s = check_Sn(x, 2, "_out");
  FILE *fp;
  char *name = check_string(S_DATA(x)[ 0 ], "_out", 0);
  X *data = check_S(S_DATA(x)[ 1 ], "_out");
  int len;

  check_S(data, "_out");
  len = S_LENGTH(S_DATA(x)[ 1 ]);

  if((fp = fopen(name, "wb")) == NULL)
    fail("can not open file `%s' for writing in `_out'", name);

  while(len--) {
    X x = *(data++);

    if(IS_N(x)) fputc(N_VALUE(x), fp);
    else failx(x, "data contains non-numeric value in `_out'");
  }

  fclose(fp);
  return x;
}


DEFINE(____5fshow)
{
  dump(x, stdout, 0);
  return x;
}


DEFINE(____5fsystem)
{
  return TO_N(system(check_string(x, "_system", 1)));
}


static X
select1(X index, X seq)
{
  int n = check_N(index, "select");
  X *b = check_S(seq, "select");

  if(n < 1 || n > S_LENGTH(seq))
    failx(seq, "index of %d out of bounds", n);

  return b[ n - 1 ];
}


DEFINE(___select)
{
  X *a = check_Sn(x, 2, "select");
  return select1(a[ 0 ], a[ 1 ]);
}


DEFINE(____5fenv)
{
  char *v = check_A(x, "_env");
  v = getenv(v);
  return v == NULL ? F : intern(v);
}


DEFINE(____5fthrow)
{
  save(x);
  longjmp(*catcher, 1);
}


DEFINE(____5frnd)
{
  return TO_N(rand() % check_N(x, "_rnd"));
}


DEFINE(___rev)
{
  X *s = check_S(x, "rev");
  int len;
  X s2;

  save(x);
  s2 = allocate(len = S_LENGTH(x));
  x = restore();
  s = S_DATA(x);
  
  while(--len >= 0)
    S_DATA(s2)[ len ] = *(s++);

  return s2;
}


DEFINE(___string)
{
  return TO_B(is_string(x, 1));
}


DEFINE(____5fgc)
{
  reclaim();
  return x;
}


DEFINE(___app)
{
  X *a = check_Sn(x, 2, "app");
  X *fptr = check_Sn(a[ 0 ], 3, "app");
  X (*func)(X);
  unsigned long addr;

  if(fptr[ 0 ] != FPTR)
    failx(fptr[ 0 ], "bad argument type in `app' - not a function pointer");

  addr = ((unsigned long)N_VALUE(fptr[ 1 ]) << 16) | (unsigned long)N_VALUE(fptr[ 2 ]);
  func = (X (*)(X))((void *)addr);
  return func(a[ 1 ]);
}


DEFINE(___subseq)
{
  X *a = check_Sn(x, 3, "subseq");
  int i = check_N(a[ 0 ], "subseq") - 1;
  int j = check_N(a[ 1 ], "subseq");
  X *s = check_S(a[ 2 ], "subseq");
  int len = S_LENGTH(a[ 2 ]);
  X s2;
  int k;

  if(len == 0 || i >= j || i >= len) 
    return EMPTY;

  if(i < 0) i = 0;

  if(i >= len) i = len - 1;

  if(j < 0) j = 0;

  if(j > len) j = len;

  save(a[ 2 ]);
  s2 = allocate(j - i);
  s = S_DATA(restore());

  for(k = 0; i < j; ++k, ++i)
    S_DATA(s2)[ k ] = s[ i ];

  return s2;
}


DEFINE(____5fiostep)
{
  static int iostep = 0;
  X *a = check_Sn(x, 2, "_iostep");
  X *b = check_Sn(a[ 0 ], 2, "_iostep");
  char *loc = check_A(a[ 1 ], "_iostep");
  
  if(b[ 0 ] != IO || !IS_N(b[ 1 ]))
    failx(b[ 0 ], "bad argument type in `%s' - not a valid I/O tag", loc);

  if(N_VALUE(b[ 1 ]) != iostep)
    failx(a[ 0 ], "I/O operation in `%s' out of order", loc);

  return sequence(2, IO, TO_N(++iostep));
}


DEFINE(___make)
{
  X *a = check_Sn(x, 2, "make");
  int i, len;
  X s;
  
  len = check_N(a[ 0 ], "make");

  if(len < 0)
    failx(a[ 0 ], "invalid length given to `make'");

  save(a[ 1 ]);
  s = allocate(len);
  x = restore();

  for(i = 0; i < len; ++i)
    S_DATA(s)[ i ] = x;

  return s;
}


DEFINE(___iota)
{
  int n = check_N(x, "iota");
  int i;
  X s;

  if(n < 0)
    failx(x, "invalid length given to `iota'");

  s = allocate(n);

  for(i = 1; i <= n; ++i)
    S_DATA(s)[ i - 1 ] = TO_N(i);

  return s;
}


DEFINE(___dl)
{
  X *a = check_Sn(x, 2, "dl");
  X r, p;
  int i, len;

  check_S(a[ 1 ], "dl");
  len = S_LENGTH(a[ 1 ]);
  save(a[ 0 ]);
  save(a[ 1 ]);
  r = allocate(len);
  save(r);

  for(i = 0; i < len; ++i) {
    p = sequence(2, pick(3), S_DATA(pick(2))[ i ]);
    S_DATA(top())[ i ] = p;
  }

  r = restore();
  restore();
  restore();
  return r;
}


DEFINE(___dr)
{
  X *a = check_Sn(x, 2, "dr");
  X s;
  X r, p;
  int i, len;

  check_S(a[ 0 ], "dr");
  len = S_LENGTH(a[ 0 ]);
  save(a[ 1 ]);
  save(a[ 0 ]);
  r = allocate(len);
  save(r);

  for(i = 0; i < len; ++i) {
    p = sequence(2, S_DATA(pick(2))[ i ], pick(3));
    S_DATA(top())[ i ] = p;
  }

  r = restore();
  restore();
  restore();
  return r;
}
