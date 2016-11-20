/* rt.c */


FILE *
ensure_error_output()
{
  static FILE *err = NULL;
  
  if(log_errors) {
    if(err == NULL)
      err = fopen(ERROR_FILE, "w");

    return err;
  }

  return stderr;
}


void
dribble(char *msg, ...)
{
  va_list va;

  if(debug_flag) {
    va_start(va, msg);
    vfprintf(error_output, msg, va);
    fflush(error_output);
    va_end(va);
  }
}


X
bomb(char *msg, ...)
{
  va_list va;

  va_start(va, msg);
  fputs("\n_|_\n", error_output);
  vfprintf(error_output, msg, va);
  fputc('\n', error_output);
  va_end(va);
  exit(EXIT_FAILURE);
}


static int
argument_value(char *arg)
{
  int len = strlen(arg);

  if(len > 0) {
    switch(arg[ len - 1 ]) {
    case 'k':
    case 'K':
      return atoi(arg) * 1024;

    case 'm':
    case 'M':
      return atoi(arg) * 1024 * 1024;

    case 'g':
    case 'G':
      return atoi(arg) * 1024 * 1024 * 1024;

    default:
      return atoi(arg);
    }
  }

  return 0;
}


int
init(int argc, char *argv[], X (*main)(X), void (*conslf)())
{
  unsigned int atoms = DEFAULT_ATOM_TABLE_SIZE;
  int i, c = argc - 1, j, ct = DEFAULT_CALL_TRACE_LENGTH;
  X args, x;
  jmp_buf exit_jmp;
  unsigned int heap = DEFAULT_HEAP_SIZE;
#ifdef PRECISE_CLOCK
  struct rusage startup_time;
  getrusage(RUSAGE_SELF, &startup_time);
#else
  clock_t startup_time = clock();
#endif
  int status = 0;

  catcher = &exit_jmp;
  srand(time(NULL));

  if(setjmp(*catcher)) {
    x = restore();

    if(IS_S(x) && S_LENGTH(x) == 2 && S_DATA(x)[ 0 ] == intern("EXIT")) {
      status = N_VALUE(S_DATA(x)[ 1 ]);
      goto finished;
    }
    else uncaught(x);
  }

  for(i = 1; i < argc; ++i) {
    if(*argv[ i ] == '-' && argv[ i ][ 1 ] == ':') {
      --c;

      switch(argv[ i ][ 2 ]) {
      case 'h':
	heap = argument_value(argv[ i ] + 3);
	break;

      case 'a':
	atoms = argument_value(argv[ i ] + 3);
	break;

      case 'd':
	debug_flag = 1;
	break;

      case 't':
	ct = atoi(argv[ i ] + 3);
	break;

      case '?':
        printf("%s [-:?] [-:hSIZE] [-:aSIZE] [-:d] [-:s] [-:e] [-:tLENGTH] "
	       "ARGUMENT ...\n", argv[ 0 ]);
	exit(0);

      case 's':
	no_strings_flag = 1;
	break;

      case 'e':
	log_errors = 1;
	break;

      default:
	bomb("invalid runtime option");
      }
    }
  }

  buffer = (char *)malloc(DEFAULT_HANDY_BUFFER_SIZE);

  if(buffer == NULL)
    fail("can not allocate temporary buffer of size %d", DEFAULT_HANDY_BUFFER_SIZE);

  buffer_size = DEFAULT_HANDY_BUFFER_SIZE;
  init_heap(heap);
  init_atoms(atoms);
  EMPTY = sequence(0);
  calltrace = (char **)malloc(sizeof(char *) * ct);

  if(calltrace == NULL)
    fail("can not allocate trace buffer");

  memset(calltrace, 0, sizeof(char *) * ct);
  calltracetop = calltrace;
  calltraceend = calltrace + ct;
  dribble("[creating literals]\n");
  conslf();
  lf_created = 1;
  heap = shrink_heap(heap);
 
  if(main != NULL) {
    args = allocate(c);
    j = 0;

    for(i = 1; i < argc; ++i) {
      if(*argv[ i ] != '-' || argv[ i ][ 1 ] != ':') {
	save(args);
	x = intern(argv[ i ]);
	args = restore();
	S_DATA(args)[ j++ ] = x;
      }
    }

    dribble("[start]\n");
    char unused;
    stack_base = &unused;
    max_stack_top = stack_base;
    x = main(args);

    if(savedp != saved + ROOT_SLOTS)
      fail("(internal) %d items on temporary stack left", savedp - saved);

  finished:    
    dribble("[%d atoms, %dk of %dk heap used, %d collections]\n", 
	    atom_table_count,
	    (sizeof(X) * (fromspace_top - fromspace_start)) / 1000,
	    heap / 2000, gc_count);
#ifndef NDEBUG
    dribble("[max stack depth: %d]\n", (char *)(&heap) - max_stack_top);
#endif
#ifdef RCACHE
    dribble("[%d r-cache hit%s]\n", rcache_hits, rcache_hits != 1 ? "s" : "");
#endif
#ifdef PRECISE_CLOCK
    struct rusage t2;
    getrusage(RUSAGE_SELF, &t2);
    dribble("[execution took %g seconds CPU time]\n", 
	    (double)(t2.ru_utime.tv_sec - startup_time.ru_utime.tv_sec) +
	    (double)(t2.ru_utime.tv_usec - startup_time.ru_utime.tv_usec) / 1000000);
#else
    dribble("[execution took %g seconds wall-clock time]\n", 
	    (double)(clock() - startup_time) / CLOCKS_PER_SEC);
#endif
    dribble("[exit status is %d]\n", status);
  }

  return status;
}


X
string(char *str)
{
  int i, len = strlen(str);
  X s = allocate(len);

  for(i = 0; *str; ++i)
    S_DATA(s)[ i ] = TO_N(*(str++));

  return s;
}


static int
printable_char(int c, int nl)
{
  return (c < 0x100 && isgraph(c)) 
    || c == ' ' || (nl && (c == '\n' || c == '\r' || c == '\t'));
}


int
is_string(X x, int nl)
{
  int i, len, c;
  X e;
  
  if(!IS_S(x)) return 0;

  len = S_LENGTH(x);

  for(i = 0; i < len; ++i) {
    e = S_DATA(x)[ i ];
    
    if(!IS_N(e)) return 0;
    
    c = N_VALUE(e);

    if(!printable_char(c, nl)) return 0;
  }

  return 1;
}


static int dump_count;

static void
dump1(X x, int ind, int indf, FILE *fp, int limit)
{
  int len, i;
  int s = 1, p = 0, comma = 0, ind2, ind2f;
  X e;

  if(indf) {
    fputc('\n', fp);

    for(i = 0; i < ind; ++i) fputc(' ', fp);
  }

  if(IS_N(x)) fprintf(fp, "%d", N_VALUE(x));
  else if(IS_A(x)) fprintf(fp, "%s", A_STRING(x));
  else {
    len = S_LENGTH(x);

    if(!no_strings_flag) {
      for(i = 0; i < len; ++i) {
	int c;
	e = S_DATA(x)[ i ];
	
	if(IS_N(e)) {
	  c = N_VALUE(e);
	  
	  if(!printable_char(c, 0)) s = 0;
	}
	else if(IS_A(e)) s = 0;
	else if(is_string(e, 0)) s = 0;
	else {
	  p = 1;
	  s = 0;
	}
      }
    }
    else s = 0;

    if(s) s = len > 0;

    if(!s) fputc('<', fp);
    else fputc('\"', fp);

    ind2 = ind + 1;

    for(i = 0; i < len && dump_count++ < limit; ++i) {
      ind2f = p && i > 0;

      if(comma && !s) fputs(", ", fp);
      
      comma = 1;
      e = S_DATA(x)[ i ];

      if(IS_N(e)) {
	if(s) fputc(N_VALUE(e), fp);
	else dump1(e, ind2, ind2f, fp, limit);
      }
      else if(IS_A(e)) dump1(e, ind2, ind2f, fp, limit);
      else dump1(e, ind2, ind2f, fp, limit);
    }

    if(i < len) fputs(", ...", fp);

    if(!s) fputc('>', fp);
    else fputc('\"', fp);
  }
}


void
dump(X x, FILE *fp, int limited)
{
  dump_count = 0;
  dump1(x, 0, 0, fp, !limited ? 0x0fffffff : DUMP_ELEMENT_LIMIT);
  fputc('\n', fp);
  fflush(fp);
}


#ifndef NDEBUG
void
tracecall(char *loc)
{
  char dummy;

  if(calltracetop >= calltraceend) calltracetop = calltrace;

  *(calltracetop++) = loc;

  if(&dummy < max_stack_top)
    max_stack_top = &dummy;
}


void
dumptrace(FILE *fp)
{
  char **ptr = calltracetop + 1; 

  while(ptr != calltracetop) {
    if(ptr >= calltraceend) ptr = calltrace;
    else {
      if(*ptr != NULL) 
	fprintf(fp, "\n  %s", *ptr);

      ++ptr;
    }
  }

  fputs("\t\t<--\n", fp);
}
#endif
