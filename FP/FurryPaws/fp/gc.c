/* gc.c - simple cheney-style collector */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


#ifdef CAREFUL
INLINE void
check_saved(X *p)
{
  if(p != savedp)
    bomb("save/restore mismatch");
}
#endif


INLINE void
save(X x)
{
#ifdef CAREFUL
  if(savedp < saved + ROOT_SLOTS) 
    bomb("saved area underflow");
#endif

  *(savedp++) = x;
}


INLINE X
restore()
{
#ifdef CAREFUL
  if(savedp <= saved + ROOT_SLOTS) 
    bomb("saved area underflow");
#endif

  return *(--savedp);
}


INLINE X 
top()
{
#ifdef CAREFUL
  if(savedp <= saved + ROOT_SLOTS) 
    bomb("saved area underflow");
#endif

  return *(savedp - 1);
}


INLINE X
pick(int n)			/* 1-based! */
{
#ifdef CAREFUL
  if(savedp - n < saved + ROOT_SLOTS)
    bomb("saved area underflow");
#endif

  return *(savedp - n);
}


X
check_fptr(X x)
{
  if(IS_S(x)) {
    if((S_LENGTH(x) & GCMARK_BIT) != 0)
      bomb("forwarding pointer in sequence header");
#ifdef EXTRA_CAREFUL
    /* check all elements for being forwarding pointers */
    else {
      int i, len = S_LENGTH(x);
      X *a = S_DATA(x);
      
      for(i = 0; i < len; ++i) {
	if(IS_S(x) && (S_LENGTH(x) & GCMARK_BIT) != 0)
	  bomb("forwarding pointer in sequence header (item #%d of sequence %p)", i, x);
      }
    }
#endif
  }

  return x;
}


void
init_heap(unsigned int size)
{
  int i;

  dribble("[allocating heap of size %u]\n", size);
  size /= 2;
  fromspace_start = (X *)malloc(size);

  if(fromspace_start == NULL)
    bomb("out of memory - can not allocate fromspace of size %u", size);

  fromspace_end = (X *)((char *)fromspace_start + size);
  tospace_start = (X *)malloc(size);

  if(tospace_start == NULL)
    bomb("out of memory - can not allocate tospace of size %u", size);

  tospace_end = (X *)((char *)tospace_start + size);
  fromspace_top = fromspace_start;
  dribble("[  fromspace: %p - %p]\n", fromspace_start, fromspace_end);
  dribble("[    tospace: %p - %p]\n", tospace_start, tospace_end);
  savedp = saved + ROOT_SLOTS;

  for(i = 0; i < ROOT_SLOTS; ++i)
    saved[ i ] = F;
}


unsigned int
shrink_heap(unsigned int size)
{
  unsigned int lsize = (fromspace_end - fromspace_top) * sizeof(X);

  dribble("[shrinking fromspace of size %u to %u]\n", size / 2, lsize);

  fromspace_start = fromspace_top;
  tospace_start = (X *)realloc(tospace_start, lsize);
  
  if(tospace_start == NULL)
    bomb("out of memory - can not shrink tospace to size %u", lsize);

  tospace_end = (X *)((char *)tospace_start + lsize);
  dribble("[  fromspace: %p - %p]\n", fromspace_start, fromspace_end);
  dribble("[    tospace: %p - %p]\n", tospace_start, tospace_end);
  return lsize;
}


static void
mark(X *xp)
{
  X x = *xp;

  if(((long)x & NONSEQ_MASK) == 0) { /* if not a sequence, ignore */
    if(x >= (X)fromspace_start && x < (X)fromspace_end) { /* ignore pointers outside of heap */
      unsigned long len = S_LENGTH(x);

      if((len & GCMARK_BIT) != 0) /* if already marked, use forwarding pointer */
	*xp = (X)((len & ~GCMARK_BIT) << 1);
      else {			/* otherwise copy to tospace */
	int bytes = len * sizeof(X) + sizeof(unsigned long);
	memcpy(tospace_top, x, bytes);
	S_LENGTH(x) = GCMARK_BIT | ((unsigned long)tospace_top >> 1);
	*xp = (X)tospace_top;
	tospace_top += len + 1;
      }
    }
  }
}


unsigned int
reclaim()
{
  X *scan;
  unsigned int r;

  if(!lf_created)
    bomb("GC while creating literal frame - out of space");

  ++gc_count;
  //dribble("[GC #%d: starting, saved: %d]\n", gc_count, savedp - saved + ROOT_SLOTS);

  tospace_top = tospace_start;

  for(scan = saved; scan < savedp; ++scan)
    mark(scan);

  scan = tospace_start;

  while(scan < tospace_top) {
    unsigned long len = S_LENGTH((X)scan);

    while(len--)
      mark(++scan);

    ++scan;
  }

  scan = fromspace_start;
  fromspace_start = tospace_start;
  tospace_start = scan;
  scan = fromspace_end;
  fromspace_end = tospace_end;
  tospace_end = scan;
  fromspace_top = tospace_top;
  r = (fromspace_end - fromspace_top) * sizeof(X);

  if(debug_flag) {
    unsigned int p = (r * 100) / (fromspace_end - fromspace_start) * sizeof(X);
    
    if(p <= HEAP_CRITICAL_SIZE)
      dribble("[GC: heap low, remaining: %d%%]\n", p);
  }

  return r;
}


X
allocate(unsigned long length)
{
  S *s;
  X *p;
  unsigned long size;

#ifndef STRESS_GC
  if(fromspace_top + length + 1 > fromspace_end) {
#else 
  if(lf_created) {
#endif
    unsigned long space = reclaim();

    if(length + 1 > space / sizeof(X))
      bomb("heap size exceeded - can not allocate sequence of size %u", length);
  }

  s = (S *)fromspace_top;
  fromspace_top += length + 1;
  s->length = length;
  
  for(p = s->data; length--; ++p)
    *p = F;

  return (X)s;
}


X
sequence(unsigned long length, ...)
{
  va_list va;
  int i;
  X s;

  va_start(va, length);

  for(i = 0; i < length; ++i) 
    save(va_arg(va, X));

  s = allocate(length);

  for(i = length - 1; i >= 0; --i)
    S_DATA(s)[ i ] = restore();

  va_end(va);
  return s;
}
