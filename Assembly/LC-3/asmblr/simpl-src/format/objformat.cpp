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
#include "objformat.h"

// TODO: Inform users of where their errors are!

int ObjFormat::readOne(FILE* f, Reporter* reporter, GVal* pVal, int bits) const
{
    int bytes = (bits + 7) / 8;
    int retval = 0;
    GVal val = 0;
    
    for (int i = 0; i < bytes; i++)
    {
        int c = fgetc(f);
        
        if (c == EOF)
        {
            if (i == 0) retval = -1;
            else
            {
                reporter->err("Object file has an uneven number of bytes.");
                retval = -2;
            }
            
            break;
        }
        
        val = (val << 8) | c;
    }
    
    *pVal = val;
    
    return retval;
}

SuccessT ObjFormat::writeOne(FILE* f, GAddr UNUSED(addr), GVal val, int bits)
    const
{
    int bitPos = ((bits + 7) / 8) * 8;
    
    for (; bitPos >= 8;)
    {
        bitPos -= 8;
        if (fputc((val >> bitPos) & 0xff, f) < 0)
            return SuccessFail;
    }
    
    return SuccessPass;
}
