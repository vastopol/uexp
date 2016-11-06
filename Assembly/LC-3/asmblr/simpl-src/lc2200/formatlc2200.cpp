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
#include "machinelc2200.h"
#include "formatlc2200.h"
#include "model/model.h"
#include <ctype.h>

FormatLC2200::FormatLC2200()
 : Format(&ArchLC2200::instance(),
     "4:5:3+,6+,9+,2+,1+|4+,7+,10+,13+,12+|5+,8+,11+,15+,14+|p+,,,w,e")
{
}

String FormatLC2200::formatState(const Context& context, int flags) const
{
    char buf[1024];
    char *s = buf;
    
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            int regnum = c * 8 + r;
            GReg val = context.machine().reg(regnum);
            s += sprintf(s, " %s: x%02X %s",
                String(mArch->regName(regnum))(0, 2).c_str(), val,
                formatVal(val, context.symTable(), DisplayDecSym)
                    .padRight(' ', 12).c_str());
        }
        s += sprintf(s, "\n");
    }
    
    s += sprintf(s, " Now at x%04X: %s\n", context.machine().pc(),
        disassemble(context, context.machine().pc()).c_str());
    
    return String(buf);
}
