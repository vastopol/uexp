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


#include "asmformat.h"
#include "asmrule.h"
#include <ctype.h>

bool AsmFormat::readLine(FILE* f, String* line) const
{
    // TODO: Replace with old loop
    int c;
    
    (*line) = "";
    
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '\r')
        {
            c = fgetc(f);
            if (c != '\n' && c != EOF)
            {
                ungetc(c, f);
            }
            break;
        }
        else if (c == '\n')
        {
            break;
        }
        
        (*line) += char(c);
    }
    
    return !(c == EOF && (*line).length() == 0);
}

LocID::Type AsmFormat::findDirectiveType(char c) const
{
    LocID::Type type;
    
    switch(c)
    {
      case 'i':
        type = LocID::MemInstr;
        break;
      case '\0':
      case 'w':
      case 'W':
        type = LocID::MemWord;
        break;
      case 'b':
      case 'B':
        type = LocID::MemByte;
        break;
      default:
        type = LocID::Invalid;
    }
    
    return type;
}

SuccessT AsmFormat::preprocessInstr(AsmTokenizer* tokenizer, CodeEmitter* emitter)
    const
{
    emitter->increment(LocID::MemInstr);
    return SuccessPass;
}

SuccessT AsmFormat::preprocessDirective(AsmTokenizer* tokenizer,
    CodeEmitter* emitter) const
{
    String directive = tokenizer->peek().toLower();
    AsmRule::AsmContext curContext = context(*emitter);
    int addrBits = mArch->addrBits();
    SuccessT suc = SuccessPass;
    
    
    if (directive == ".byte") directive = ".fillb";
    else if (directive == ".word") directive = ".fillw";
    
    // TODO(garryb): validation from parseExpr does not prohibit "junk" tokens
    //  - Perhaps this is handled in a later stage?
    
    if (directive == ".align")
    {
        GVal width;
        
        tokenizer->gobble();
        
        if (Failed(ExprParser().parseExpr(tokenizer, &width)))
        {
            tokenizer->err("Invalid expression specified for .align");
            suc = SuccessFail;
        }
        else if (width > mArch->lastAddress())
        {
            tokenizer->err("Alignment size is way too large (%u bytes).",
                unsigned(width));
            suc = SuccessFail;
        }
        else
        {
            emitter->setAddr((emitter->curAddr()+width-1) / width * width);
        }
    }
    else if (directive.startsWith(".fill") && directive.length() <= 6)
    {
        LocID::Type type = findDirectiveType(directive[5]);
        if (type != LocID::Invalid)
        {
            emitter->increment(type);
        }
        else
        {
            tokenizer->err("Only support .fill(%d), .fillw(%d), .fillb(%d), and .filli(%d).",
                mArch->wordBytes(),
                mArch->wordBytes(),
                1,
                mArch->instrBytes());
            suc = SuccessFail;
        }
    }
    else if (directive.startsWith(".blk") && directive.length() <= 5)
    {
        LocID::Type type = findDirectiveType(directive[4]);
        GVal count;
        
        tokenizer->gobble();
        if (type == LocID::Invalid)
        {
            tokenizer->err("Only support .blk(%d), .blkw(%d), .blkb(%d), and .blki(%d).",
                mArch->wordBytes(),
                mArch->wordBytes(),
                1,
                mArch->instrBytes());
            suc = SuccessFail;
        }
        else if (Failed(ExprParser().parseExpr(tokenizer, &count)))
        {
            tokenizer->err("Invalid constant specified for .blk count.");
            suc = SuccessFail;
        }
        else if (!emitter->canIncrement(type, count))
        {
            tokenizer->err("Cannot increment by %u bytes.",
                unsigned(mArch->bytesOf(type) * count));
            suc = SuccessFail;
        }
        else
        {
            emitter->increment(type, count);
        }
    }
    else if (directive == ".orig" || directive == ".org")
    {
        GInstr newAddr;
        ImmRule addrParser(addrBits, Format::DisplayHex, ImmRule::Unsigned, 0,
            addrBits);

        tokenizer->gobble();
        suc = addrParser.assemble(curContext, tokenizer, &newAddr);
        
        if (suc != SuccessFail)
        {
            emitter->setAddr(GAddr(newAddr));
        }
        else
        {
            tokenizer->err("Invalid constant specified for .orig.");
            suc = SuccessFail;
        }
    }
    else if (directive == ".stringz" || directive == ".asciiz"
        || directive == ".ascii")
    {
        int pos = tokenizer->pos();
        
        tokenizer->gobble();
        if (tokenizer->isString())
        {
            int extra = (directive(-1) == 'z') ? 1 : 0;
            emitter->increment(LocID::MemByte,
                tokenizer->peekString().length() + extra);
        }
        else
        {
            tokenizer->err(".stringz needs a valid string, but got [%s].",
                tokenizer->peek().c_str());
            suc = SuccessFail;
        }
        
        tokenizer->setPos(pos);
    }
    else if (directive == ".end")
    {
        // ignored for now
    }
    else if (directive == ".text")
    {
        tokenizer->warn("Temporarily unsupported directive: %s.", directive.c_str());
    }
    else if (directive == ".data")
    {
        tokenizer->warn("Temporarily unsupported directive: %s.", directive.c_str());
    }
    else
    {
        tokenizer->warn("Invalid assembler directive, ignoring.");
        tokenizer->clear();
        suc = SuccessFail;
    }
    
    return suc;
}

SuccessT AsmFormat::assembleInstr(AsmTokenizer* tokenizer,
    CodeEmitter* emitter) const
{
    GInstr val;
    SuccessT suc = mRule->assemble(context(*emitter), tokenizer, &val);
    emitter->emitCode(LocID::MemInstr, val);
    return suc;
}

SuccessT AsmFormat::assembleLine(AsmTokenizer* tokenizer,
    CodeEmitter* emitter) const
{
    SuccessT suc;
    
    if (tokenizer->remaining())
    {
        int pos = tokenizer->pos();
        GInstr val = 0xBAAD;
        
        if (tokenizer->peek()[0] == '.')
        {
            String directive = tokenizer->peek().toLower();
            
            tokenizer->gobble();
            
            if (directive == ".byte") directive = ".fillb";
            else if (directive == ".word") directive = ".fillw";
            
            if (directive.startsWith(".fill"))
            {
                LocID::Type type = findDirectiveType(directive[5]);
                int bits = mArch->bitsOf(type);
                ImmRule parser(bits, Format::DisplayHex,
                    ImmRule::Signed, 0, bits);
                
                suc = parser.assemble(context(*emitter), tokenizer, &val);
                emitter->emitCode(type, val);
            }
            else if (directive == ".stringz" || directive == ".asciiz"
                || directive == ".asciii")
            {
                String str = tokenizer->peekString();
                size_t nullbytes = (directive(-1)=='z') ? 1 : 0;
                size_t numchar = str.length() + nullbytes;
                
                for (size_t i = 0; i != numchar; i++)
                {
                    emitter->emitCode(LocID::MemByte, str[i]);
                }
                
                tokenizer->gobble();
                
                suc = SuccessPass;
            }
            else
            {
                suc = SuccessPass;
            }
        }
        else
        {
            suc = assembleInstr(tokenizer, emitter);
        }
        
        if (suc != SuccessFail)
        {
            if (tokenizer->remaining())
            {
                tokenizer->err("Unknown text left over at end of line.");
                suc = SuccessFail;
            }
        }
        else if (!tokenizer->hasErrors())
        {
            if (tokenizer->pos() == pos)
            {
                tokenizer->err("Unrecognized opcode (instruction) specified.");
            }
            else
            {
                tokenizer->err("Parse error starting here.");
            }
        }
    }
    else
    {
        suc = SuccessPass;
    }
    
    return suc;
}

bool AsmFormat::isIdentifier(const char* str) const
{
    const char *s = str;
    if (isalpha(*s) || *s == '_') while (isalnum(*++s) || *s == '_') {}
    return s != str && *s == '\0';
}

SuccessT AsmFormat::read(FILE* f,
    CodeEmitter* emitter, Reporter* reporter) const
{
    SuccessT suc = SuccessPass;
    Array<AsmTokenizer> lines;
    
    /* Phase 1: Read in lines */
    /* Phase 2: Scan for labels, determine sizes (combined with phase 1) */
    String line;
    Array<GAddr> addresses;
    
    while (readLine(f, &line))
    {
        int lineNum = lines.size() + 1;
        AsmTokenizer* tokenizer =
            new(lines.make_back())AsmTokenizer(line, lineNum, reporter);
        
        addresses.push_back(emitter->curAddr());
        
        // Label at the beginning?  Must be identifier, and not an opcode.
        if (tokenizer->remaining() && isIdentifier(tokenizer->peek())
            && !mArch->isTokenReserved(tokenizer->peek()))
        {
            String symname = tokenizer->peek();
            
            if (mSymTable->lookup(symname) >= 0)
            {
                suc = min(suc, SuccessFail);
                tokenizer->err(
                    "Label '%s' redefined, or unknown instruction.",
                    symname.c_str());
            }
            
            tokenizer->gobble();
            // TODO(gboyer): Should '=' be processed in phase 2 so future
            // labels can be used, or in phase 1 so future "=" can be used?
            if (tokenizer->match("="))
            {
                // sllow setting of variables to constants
                GVal addr;
                if (Failed(ExprParser(mSymTable).parseExpr(tokenizer, &addr)))
                {
                    tokenizer->err("Invalid expression for '=' macro.");
                    suc = min(suc, SuccessFail);
                }
                else
                {
                    mSymTable->add(symname, addr);
                }
            }
            else
            {
                void(tokenizer->match(":"));
                mSymTable->add(symname, emitter->curAddr());
            }
        }
        
        // Now, find how many bytes this line takes up (if any).
        if (tokenizer->remaining())
        {
            SuccessT result;
            
            if (tokenizer->peek()[0] == '.')
            {
                result = preprocessDirective(tokenizer, emitter);
            }
            else
            {
                result = preprocessInstr(tokenizer, emitter);
            }
            
            suc = min(suc, result);
        }
    }
    
    /* Phase 3: Parse and compute values */
    
    for (size_t i = 0; i < lines.size(); i++)
    {
        emitter->setAddr(addresses[i]);
        SuccessT result = assembleLine(&lines[i], emitter);
        suc = min(suc, result);
    }
    
    return suc;
}

void AsmFormat::writeLine(FILE* f, GAddr addr, GInstr instr) const
{
    int instrBits = mArch->instrBits();
    String line = mRule->disassemble(
        AsmRule::AsmContext(addr, mSymTable, mArch), instr);
    int len = line.length();
    
    // tab the line up in order to print out the address of the instruction
    while (len < 48)
    {
        len += 8;
        line += '\t';
    }

    fprintf(f, "\t%s; x%04X: x%s\n",
        line.c_str(), addr, Format::formatHex(instr, instrBits).c_str());
}

void AsmFormat::writeZeros(FILE* f, int numZeros, GAddr finalAddr) const
{
    if (numZeros)
    {
        // prefer literal for singles, .blk for short runs, .orig otherwise
        if (numZeros == 1)
        {
            writeLine(f, finalAddr - mArch->instrBytes(), 0);
        }
        else if (numZeros < 64)
        {
            fprintf(f, "\t.blkb #%d\n", numZeros * mArch->instrBytes());
        }
        else
        {
            fprintf(f, "\t.orig 0x%X\n", finalAddr);
        }
    }
}

SuccessT AsmFormat::write(FILE* f,
    const Machine& machine, GAddr origin, size_t length) const
{
    int instrBytes = mArch->instrBytes();
    GAddr addrMask = mArch->addrMask();
    int zeros = 999999;
    
    // Disassemble the code.
    // This loop could be quite simple (using only a loop of writeLine);
    // however, care is taken to avoid spitting out giant lengths of
    // zeros.
    
    for (size_t i = 0; i < length; i += instrBytes)
    {
        GAddr addr = (origin + i) & addrMask;
        GInstr instr = machine.memInstr(addr);
        const char *sym;
        
        if ((sym = mSymTable->getname(addr)))
        {
            writeZeros(f, zeros, addr);
            zeros = 0;
            fprintf(f, "%s:\n", sym);
        }
        
        if (instr == 0)
        {
            zeros++;
        }
        else
        {
            writeZeros(f, zeros, addr);
            zeros = 0;
            writeLine(f, addr, instr);
        }
    }
    
    writeZeros(f, zeros, (origin + length) & addrMask);
    
    return SuccessPass;
}
