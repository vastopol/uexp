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
/**
 * @file symbol.h
 *
 * This file contains header information about the symbol table structure.
 */

#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Symbol table structure.
 *
 * The symbol table allows the ability to add symbols to the symbol table,
 * as well as perform forward and reverse lookups.
 *
 * The current implementation is not very efficient for large symbol tables,
 * but as soon as I have time I will make it a hash table.
 *
 * @see symt_init
 * @see symt_destroy
 * @see symt_copy
 * @see symt_add
 * @see symt_lookup
 * @see symt_getname
 * @see symt_read
 * @see symt_from_file
 * @see SymTable
 */
struct symt
{
    /** This is the head symbol in the (ick) list of symbols. */
    struct symbol *head;
    int case_sensitive;
};

/**
 * Make a blank symbol table.
 * @param table - table to initialize
 * @param case_sensitive - whether it should be case sensitive
 */
void symt_init(struct symt *table, int case_sensitive);
/**
 * Free up the symbol table's entries.
 * @param table - table to free up resources for (does not free table itself)
 */
void symt_destroy(struct symt *table);
/**
 * Clones a symbol table.
 *
 * @return dst on success, NULL on error.
 */
struct symt *symt_copy(struct symt *dst, const struct symt *src);
/**
 * Add an entry to the symbol table, or change the binding if the name
 * already exists in the table.
 *
 * @param table - symbol table to add entry to
 * @param name - name to associate
 * @param addr - the address to associate the name with
 * @return 0 on success, negative on failure, 1 on replacing pre-existing
 */
int symt_add(struct symt *table, const char *name, int addr);
/**
 * Look up the address of a symbol.
 *
 * @param table - the symbol table to use
 * @param name - the symbol's name
 * @return -1 on error, otherwise the symbol's address.
 */
int symt_lookup(const struct symt *table, const char *name);
/**
 * Get the name from an address.
 *
 * @param table - the symbol table to use to find the address
 * @param addr - the address to find a symbol for
 * @return NULL on error, otherwise the symbol name.
 */
const char *symt_getname(const struct symt *table, int addr);
/**
 * Add all symbol tables from one symbol table to another.
 * @param table - table to add stuff to
 * @param other - the table to get the symbols from
 * @return -1 on error, otherwise zero
 */
int symt_addall(struct symt *table, const struct symt *other);
/**
 * Read an LC-3 symbol file.
 *
 * @param table - the symbol table to read symbols into
 * @param symfile - symbol file open for reading
 * @return 0 on success, negative on failure
 */
int symt_read(struct symt *table, FILE *symfile);
/**
 * Write an LC-3 symbol file.
 *
 * @param table - the symbol table to write symbols into
 * @param symfile - symbol file open for writing
 * @return 0 on success, negative on failure
 */
int symt_write(const struct symt *table, FILE *symfile);
/**
 * Create, whole 9 yards, from a file.
 */
struct symt *symt_from_file(FILE *symfile);

#ifdef __cplusplus
}; /* extern "C" */
/**
 * This is a friendly abstraction of the symbol table structure.
 *
 * Symbols can be added to the table, along with symbol->address
 * lookup as well as address->symbol lookup.
 */
class SymTable : public symt
{
 public:
    /** Create a symbol table. */
    SymTable(bool caseSensitive = true) { symt_init(this, caseSensitive); }
    /** Free up resources associated with the symbol table. */
    ~SymTable() { symt_destroy(this); }
    /**
     * Make a duplicate symbol table.
     * @param o - other symbol table to duplicate.
     */
    SymTable(const SymTable &o) { Exception::when(!symt_copy(this, &o)); }
    
    /**
     * Set a symbol table equal to another symbol table.
     * @param o - another symbol table to copy
     * @return *this
     */
    SymTable& operator = (const SymTable &o)
     { if (this != &o)
        { symt_destroy(this); Exception::when(!symt_copy(this, &o)); }
       return *this; }
    
    /**
     * Add a symbol to the table.
     * @param name - the name of the symbol
     * @param addr - the address to be associated with that symbol
     */
    void add(const char *name, int addr)
     { Exception::when(symt_add(this, name, addr) < 0); }
    
    /**
     * Get a symbol from the address.
     * @param name - the symbol name
     * @return the address of the symbol
     */
    int lookup(const char *name) const
     { return symt_lookup(this, name); }
    /**
     * Get the symbol name from an address.
     * This picks the first symbol found.
     * @param addr - the address of the symbol
     * @return the name of the symbol (or NULL if not found)
     */
    const char *getname(int addr) const
     { return symt_getname(this, addr); }
    
    /**
     * Add all associations from the other table.
     * @params o - stuff
     */
    void addAll(const SymTable& o)
     { symt_addall(this, &o); }
    
    /**
     * Read the symbol table from an LC-3 sym file.
     * @param symfile - a symbol file open for reading
     * @return negative on error, non-negative on success
     */
    int read(FILE *symfile)
     { return symt_read(this, symfile); }
    
    /**
     * Clear all symbols.
     */
    void clear()
     { symt_destroy(this); symt_init(this, this->case_sensitive); }
    
    int write(FILE *symfile) const
     { return symt_write(this, symfile); }
};

#endif

#endif /*SYMBOL_H_INCLUDED*/
