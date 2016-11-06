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

#include "archgt16.h"
#include "asmformatgt16.h"

SuccessT AsmFormatGT16::preprocessInstr(AsmTokenizer* tokenizer,
    CodeEmitter* emitter) const
{
    if (tokenizer->peek() == "la")
    {
        emitter->increment(LocID::MemInstr, 4);
        return SuccessPass;
    }
    else
    {
        return AsmFormat::preprocessInstr(tokenizer, emitter);
    }
}

SuccessT AsmFormatGT16::assembleInstr(AsmTokenizer* tokenizer,
    CodeEmitter* emitter) const
{
    SuccessT retval = SuccessFail;
    
    if (tokenizer->match("la"))
    {
        int regno;
        
        if ((regno = ArchGT16::instance().lookupReg(tokenizer->peek())) < 0)
        {
            tokenizer->err("Invalid register for la.\n");
        }
        else
        {
            tokenizer->gobble();
            void(tokenizer->match(","));
            ImmRule addrParser(16, Format::DisplayHex,
                ImmRule::Unsigned, 0, 16);
            GInstr address = 0;
            
            if (addrParser.assemble(context(*emitter), tokenizer, &address)
                == SuccessFail)
            {
                tokenizer->err("Invalid address.\n");
            }
            else
            {
                #if 1
                // Short version, but requires jalr $x, $x to be executed
                // in the order write-then-jump.
                
                // jalr $regno, $regno      (load PCi into $regno)
                emitter->emitCode(LocID::MemInstr,
                    0xC000 | (regno << 9) | (regno << 5));
                // lw $regno, 2($regno)     (load the inlined value)
                emitter->emitCode(LocID::MemInstr,
                    0x6002 | (regno << 9) | (regno << 5));
                // beq $zero, $zero, PCi+1  (skip the inlined value)
                emitter->emitCode(LocID::MemInstr,
                    0xA001);
                // .fill value              (the inlined value)
                emitter->emitCode(LocID::MemInstr,
                    address);
                
                #else
                // Arithmetic version -- really long
                
                // Load bits 11, 12, 13, 14, 15
                emitter->emitCode(LocID::MemInstr,
                    0x4000 | (regno << 9) | ((address >> 11) & 0x1F));
                // Shift left by five
                for (int i = 0; i < 5; i++)
                {
                    emitter->emitCode(LocID::MemInstr,
                        0x0000 | (regno << 9) | (regno << 5) | (regno << 1));
                }
                // Add bits 7, 8, 9, 10
                emitter->emitCode(LocID::MemInstr, ((address >> 7) & 0xf))
                    | 0x4000 | (regno << 9) | (regno << 5);
                // Shift left by five
                for (int i = 0; i < 5; i++)
                {
                    emitter->emitCode(LocID::MemInstr,
                        0x0000 | (regno << 9) | (regno << 5) | (regno << 1));
                }
                // Add bits 3, 4, 5, 6
                emitter->emitCode(LocID::MemInstr, ((address >> 3) & 0xf))
                    | 0x4000 | (regno << 9) | (regno << 5);
                // Shift left 3
                for (int i = 0; i < 3; i++)
                {
                    emitter->emitCode(LocID::MemInstr,
                        0x0000 | (regno << 9) | (regno << 5) | (regno << 1));
                }
                // Add bits 0, 1, 2
                emitter->emitCode(LocID::MemInstr, ((address >> 0) & 0x7))
                    | 0x4000 | (regno << 9) | (regno << 5);
                #endif
                
                retval = SuccessPass;
            }
        }
    }
    else
    {
        retval = AsmFormat::assembleInstr(tokenizer, emitter);
    }
    
    return retval;
}

