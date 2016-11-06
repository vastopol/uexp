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


#include "archmips.h"
#include "machinemips.h"
#include "formatmips.h"
#include "model/model.h"
#include <ctype.h>

FormatMips::FormatMips()
 : Format(&ArchMips::instance(),
     "9:4:0,8,16,24|1,9,17,25|2,10,18,26|3,11,19,27|4,12,20,28|5,13,21,29|6,14,22,30|7,15,23,31|p,,w,e")
{
}

String FormatMips::formatState(const Context& context, int flags) const
{
    char buf[4096];
    char *s = buf;
    
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            int regnum = c * 8 + r;
            GReg val = context.machine().reg(regnum);
            s += sprintf(s, " %s: %08X ",
                String(mArch->regName(regnum))(1, 2).c_str(), val);
        }
        s += sprintf(s, "\n");
    }
    
    s += sprintf(s, " Now at x%08X: %s\n", context.machine().pc(),
        disassemble(context, context.machine().pc()).c_str());
    
    return String(buf);
}
