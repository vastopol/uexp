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
#include "numformat.h"

// TODO: Inform users of where their errors are!

int NumFormat::readOne(FILE* f, Reporter* reporter, GVal* pVal, int bits) const
{
    int bitsRead = 0;
    GVal val = 0;
    int c;
    int retval = 10;
    
    while (retval == 10)
    {
        int i;
        
        c = fgetc(f);
        
        if (c >= '0' && c <= '9') i = c - '0';
        else if (c >= 'A' && c <= 'Z') i = c - 'A' + 10;
        else if (c >= 'a' && c <= 'z') i = c - 'a' + 10;
        else i = -1;
        
        if (i >= 0 && i < (1 << mBitWidth))
        {
            bitsRead += mBitWidth;
            val <<= mBitWidth;
            val |= i;
        }
        else if (c == EOF)
        {
            if (bitsRead == 0) retval = -1;
            else if (bitsRead == bits) retval = 0;
            else retval = -2;
        }
        else if (c == ';' || c == '\r' || c == '\n')
        {
            if (c == ';')
            {
                do {
                    c = fgetc(f);
                } while (c != EOF && c != '\r' && c != '\n');
            }
            if (c == '\r')
            {
                c = fgetc(f);
                if (c != '\n') ungetc(c, f);
            }
            
            if (bitsRead == bits) retval = 0;
            else if (bitsRead != 0) retval = -2;
        }
        else if (!strchr(mAllowed, c))
        {
            reporter->err(
                "Character '%c' (0x%02X) not allowed for base-%d constant.",
                c, unsigned(c), (1 << mBitWidth));
            retval = -2;
        }
    }
    
    if (retval == 0)
    {
        *pVal = val;
    }
    
    return retval;
}

SuccessT NumFormat::writeOne(FILE* f, GAddr addr, GVal val, int bits) const
{
    int bitsLeft = bits;
    SuccessT retval = SuccessPass;
    
    while (bitsLeft && retval == SuccessPass)
    {
        int i;
        char c;
        
        bitsLeft -= mBitWidth;
        i = (val >> bitsLeft) & ((1 << mBitWidth) - 1);
        
        if (i <= 9) c = i + '0';
        else c = i - 10 + 'A';
        
        if (fputc(c, f) < 0) retval = SuccessFail;
    }
    
    if (retval == SuccessPass)
    {
        if (fprintf(f, " ;x%04X\n", addr) < 0) retval = SuccessFail;
    }
    
    return retval;
}

