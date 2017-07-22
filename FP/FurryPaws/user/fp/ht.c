/* ht.c - atom table */


#include <string.h>
#include <stdlib.h>


static unsigned int
hash(char *str)
{
  unsigned int key = 0;
  int len = strlen(str);

  if(len > MAX_HASH_COUNT)
    len = MAX_HASH_COUNT;

  while(len--) key += (key << 5) + *(str++);

  return key;
}


void
init_atoms(unsigned int size)
{
  int i;

  atom_table = (struct bucket **)malloc(sizeof(struct bucket *) * size);

  if(atom_table == NULL)
    bomb("out of memory - can not allocate atom table of size %l", size);

  for(i = 0; i < size; ++i)
    atom_table[ i ] = NULL;

  atom_table_size = size;
  atom_table_count = 0;
  T = intern("T");
  F = intern("F");
  ERROR = intern("ERROR");
  UNDEFINED = intern("UNDEFINED");
  FPTR = intern("FPTR");
  IO = intern("IO");
}


X
intern(char *str)
{
  unsigned int key = hash(str) % atom_table_size;
  struct bucket *b;

  for(b = atom_table[ key ]; b != NULL; b = b->next)
    if(!strcmp(b->str, str)) return TO_A(b->str);

  b = (struct bucket *)malloc(sizeof(struct bucket));

  if(b == NULL) 
    bomb("out of memory - can not allocate bucket");

  b->str = strdup(str);
  b->next = atom_table[ key ];
  atom_table[ key ] = b;
  ++atom_table_count;
  return TO_A(b->str);
}
