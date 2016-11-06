/*
Copyright (c) 2006, Garrett Franklin Boyer
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

#ifndef ASMFORMAT_H
#define ASMFORMAT_H

#include "format/fileformat.h"
#include "format/codeemitter.h"
#include "asm/asmrule.h"

/**
 * AsmFormat is the FileFormat subclass used to load assembly files.
 *
 * One may want to extend this class to add new psuedo operations,
 * especially ones that take multiple instructions.
 * However, to add new instructions, one simply has to create a different
 * AsmRule to handle the instructions.
 */
class AsmFormat
 : public FileFormat
{
 public:
    /**
     * Construct an assembly-file-format class given a particular symbol
     * table and a root rule.
     */
    AsmFormat(SymTable* inSymTable, const Arch* inArch)
     : mSymTable(inSymTable)
     , mArch(inArch)
     , mRule(&mArch->asmRule())
     { }
    
    /**
     * Read data from the stream f into the machine, setting the origin and
     * length appropriately.
     *
     * @param f - the stream to read from
     * @param machine - machine to read data to
     * @param outOrigin - where to place the origin read, or NULL
     * @param outLength - where to place the number of bytes read
     * @return the success of reading the entire file
     */
    SuccessT read(FILE* f, CodeEmitter* emitter, Reporter* reporter) const;
    /**
     * Write data from the state back into a file.
     * @param f - output stream
     * @param machine - machine to get the state from
     * @param origin - the origin to start writing
     * @param length - number of bytes to write
     * @return the success of reading the entire file
     */
    SuccessT write(FILE* f,
        const Machine& machine, GAddr origin, size_t length) const;
    
 protected:
    LocID::Type findDirectiveType(char c) const;
    SuccessT preprocessDirective(AsmTokenizer* tokenizer,
        CodeEmitter* emitter) const;
    virtual SuccessT preprocessInstr(AsmTokenizer* tokenizer,
        CodeEmitter* emitter) const;
    virtual SuccessT assembleInstr(AsmTokenizer* tokenizer,
        CodeEmitter* emitter) const;
    SuccessT assembleLine(AsmTokenizer* tokenizer, CodeEmitter* emitter) const;
    bool isIdentifier(const char* s) const;
    
    AsmRule::AsmContext context(const CodeEmitter& emitter) const
     { return AsmRule::AsmContext(emitter.curAddr(), mSymTable, mArch); } 
    
 private:
    bool readLine(FILE* f, String* line) const;
    void writeLine(FILE* f, GAddr addr, GInstr instr) const;
    void writeZeros(FILE* f, int numZeros, GAddr addr) const;
    
 protected:
    /// The symbol table being used for this particular instance.
    SymTable* mSymTable;
    /// The architecture being assembled.
    const Arch* mArch;
    /// The root assembly rule being used.
    const AsmRule* mRule;
};

#endif
