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
#include "lineformat.h"

SuccessT LineFormat::read(FILE* f,
    CodeEmitter* emitter, Reporter* reporter) const
{
    int origBits = emitter->bitsOf(LocID::PC);
    int lineBits = emitter->bitsOf(mLineType);
    GVal lineVal;
    SuccessT retval;
    int result;
    GVal origin = 0;
    
    if (mUseOrigin && readOne(f, reporter, &origin, origBits) < 0)
    {
        reporter->err("Could not read in origin.");
        retval = SuccessFail;
    }
    else
    {
        emitter->setAddr(origin);
        
        while ((result = readOne(f, reporter, &lineVal, lineBits)) >= 0)
        {
            emitter->emitCode(mLineType, lineVal);
        }
        
        if (result == -1)
        {
            retval = SuccessPass;
        }
        else
        {
            reporter->err(
                "Error occured for value %d in sequence (address 0x%04X).",
                int(emitter->curAddr() - origin),
                emitter->curAddr());
            retval = SuccessFail;
        }
    }
    
    return retval;
}

SuccessT LineFormat::write(FILE* f,
    const Machine& machine, GAddr origin, size_t length) const
{
    size_t lineBytes = machine.bytesOf(mLineType);
    int lineBits = machine.bitsOf(mLineType);
    SuccessT result = SuccessPass;
    
    if (mUseOrigin
        && writeOne(f, origin, origin, machine.bitsOf(LocID::PC))
           == SuccessFail)
    {
        result = SuccessFail;
    }
    else
    {
        if (!mUseOrigin)
        {
            length += origin;
            origin = 0;
        }
        
        for (size_t i = 0; i < length; i += lineBytes)
        {
            GAddr addr = machine.wrapAddr(origin + i);
            result = writeOne(f, addr, machine.get(LocID(mLineType, addr)),
                lineBits);
            if (result == SuccessFail) break;
        }
    }
    return result;
}

