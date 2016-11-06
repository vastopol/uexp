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

#include "archlc2200.h"
#include "asmformatlc2200.h"

SuccessT AsmFormatLC2200::preprocessInstr(AsmTokenizer* tokenizer,
    CodeEmitter* emitter) const
{
    if (tokenizer->peek() == "la")
    {
        emitter->increment(LocID::MemInstr, 5);
        return SuccessPass;
    }
    else
    {
        return AsmFormat::preprocessInstr(tokenizer, emitter);
    }
}

SuccessT AsmFormatLC2200::assembleInstr(AsmTokenizer* tokenizer,
    CodeEmitter* emitter) const
{
    SuccessT retval = SuccessFail;
    
    // FIXME: "la" is broken
    if (!tokenizer->match("la"))
    {
        retval = AsmFormat::assembleInstr(tokenizer, emitter);
    }
    else
    {
        int regno;
        
        if ((regno = ArchLC2200::instance().lookupReg(tokenizer->peek())) < 0)
        {
            tokenizer->err("Invalid register for la.\n");
        }
        else
        {
            tokenizer->gobble();
            void(tokenizer->match(","));
            ImmRule addrParser(8, Format::DisplayHex, ImmRule::Signed, 0, 12);
            GInstr address = 0;
            
            if (addrParser.assemble(context(*emitter), tokenizer, &address)
                == SuccessFail)
            {
                tokenizer->err("Invalid address.\n");
            }
            else
            {
                // Load high 4 bits into page register
                //emitter->emitCode(LocID::MemInstr,
                //    0x4000 | (14 << 9) | ((address >> 8) & 0xF));
                // Load middle 5 bits
                emitter->emitCode(LocID::MemInstr,
                    0x4000 | (regno << 9) | ((address >> 3) & 0x1F));
                // Shift left by three
                emitter->emitCode(LocID::MemInstr,
                    0x0000 | (regno << 9) | (regno << 5) | (regno << 1));
                emitter->emitCode(LocID::MemInstr,
                    0x0000 | (regno << 9) | (regno << 5) | (regno << 1));
                emitter->emitCode(LocID::MemInstr,
                    0x0000 | (regno << 9) | (regno << 5) | (regno << 1));
                // Load low bits
                emitter->emitCode(LocID::MemInstr,
                    0x4000 | (regno << 9) | (regno << 5) | (address & 0x7));
                retval = SuccessPass;
            }
        }
    }
    return retval;
}

