/*
Copyright (c) 2005, Garrett Franklin Boyer
 All rights reserved.
 
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
Neither the name of the Georgia Institute of Technology nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "symbol.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int strcasecmp(const char* s1, const char* s2);

struct symbol
{
    struct symbol *next;
    int addr;
    char name[1]; /* malloc hack */
};

int symt_symcmp(const struct symt* table, const char *a, const char *b)
{
    if (table->case_sensitive) return strcmp(a, b);
    else return strcasecmp(a, b);
}

void symt_init(struct symt *table, int case_sensitive)
{
    table->head = NULL;
    table->case_sensitive = case_sensitive;
}

void symt_destroy(struct symt *table)
{
    struct symbol *sym;
    struct symbol *next;
    
    for (sym = table->head; sym; sym = next)
    {
        next = sym->next;
        free(sym);
    }
    
    table->head = NULL;
}

int symt_add(struct symt *table, const char *name, int addr)
{
    struct symbol *sym;
    int ret = 0;
    
    for (sym = table->head; sym; sym = sym->next)
        if (symt_symcmp(table, sym->name, name) == 0)
        {
            ret = 1;
            break;
        }
    
    if (sym || (sym = malloc(sizeof(*sym) + strlen(name))))
    {
        strcpy(sym->name, name);
        if (ret == 0)
        {
            sym->next = table->head;
            table->head = sym;
        }
        sym->addr = addr;
    }
    else
        ret = -1;
    
    return ret;
}

int symt_lookup(const struct symt *table, const char *name)
{
    struct symbol *sym;
    
    for (sym = table->head; sym; sym = sym->next)
        if (symt_symcmp(table, sym->name, name) == 0) return sym->addr;
    
    return -1;
}

const char *symt_getname(const struct symt *table, int addr)
{
    struct symbol *sym;
    
    for (sym = table->head; sym; sym = sym->next)
        if (addr == sym->addr) return sym->name;
    
    return NULL;
}

const char *symt_bestname(const struct symt *table, int addr,
    int *actual)
{
    struct symbol *sym;
    struct symbol *best = table->head;
    
    if (!best) return NULL;
    
    for (sym = table->head; sym; sym = sym->next)
        if (addr > sym->addr && addr < best->addr)
            best = sym;
    
    *actual = best->addr;
    
    return best->name;
}

int symt_addall(struct symt *table, const struct symt *other)
{
    struct symbol *sym;
    
    for (sym = other->head; sym; sym = sym->next)
    {
        if (symt_add(table, sym->name, sym->addr) < 0) return -1;
    }
    
    return 0;
}

int symt_read(struct symt *table, FILE *symfile)
{
    char buf[255];
    char *s;
    
    while (fgets(buf, 255, symfile) && buf[3] != '-')
     { }
    
    while (fgets(buf, 255, symfile))
    {
        if (buf[0] != '/' || buf[1] != '/' || buf[2] != '\t')
            continue;
        s = buf + 3; /* skip double-slash and tab */
        while (*s && !isspace((int)*s)) s++;
        if (!*s) continue;
        *s = '\0';
        while (isspace((int)*++s)) { }
        if (symt_add(table, buf + 3, strtoul(s, NULL, 16)) < 0)
            return -1;
        /*printf("Adding %s x%04lX\n", buf+3, strtoul(s, NULL, 16));*/
    }
    return 0;
}

int symt_write(const struct symt *table, FILE *symfile)
{
    struct symbol *sym;
    
    fprintf(symfile, "// Symbol Table\n");
    fprintf(symfile, "// Scope level 0:\n");
    fprintf(symfile, "//\tSymbol Name       Page Address\n");
    fprintf(symfile, "//\t----------------  ------------\n");
    
    for (sym = table->head; sym; sym = sym->next)
    {
        fprintf(symfile, "//\t%-16s  %04X\n", sym->name, sym->addr);
    }
    
    return 0;
}


struct symt *symt_from_file(FILE *symfile)
{
    struct symt *table;
    
    if ((table = malloc(sizeof(*table))) != NULL)
    {
        symt_init(table, 1);
        if (symt_read(table, symfile) < 0)
        {
            free(table);
            table = NULL;
        }
    }
    
    return table;
}

static struct symbol *symt_dup_sym(struct symbol *orig)
{
    struct symbol *sym;
    
    sym = malloc(sizeof(struct symbol) + strlen(orig->name));
    
    if (sym) strcpy(sym->name, orig->name);
    
    return sym;
}

struct symt *symt_copy(struct symt *dst, const struct symt *src)
{
    if (dst)
    {
        struct symbol *iter;
        
        if (!src->head)
        {
            dst->head = NULL;
        }
        else
        {
            struct symbol *tail = dst->head = symt_dup_sym(src->head);
            
            if (!tail)
            {
                dst = NULL;
            }
            else
            {
                for (iter = src->head->next; iter; iter = iter->next)
                {
                    tail = tail->next = symt_dup_sym(iter);
                    if (!tail)
                    {
                        symt_destroy(dst);
                        dst = NULL;
                        break;
                    }
                }
            }
        }
    }
    
    return dst; /* FIXME! */
}

