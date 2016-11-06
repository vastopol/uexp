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


#include "machinelc3.h"
#include "formatlc3.h"
#include "lc3.h"
#include <ctype.h>

FormatLC3::FormatLC3()
 : Format(&ArchLC3::instance(), "3:4:0+,1+,2+,3+|4+,5+,6+,7+|9+,p+,w,e")
{
    new(mLineOptions.make_back())Option(
        DisassembleSymbolic, "&Symbolic disassembly", 'S', 0, true);
}

String FormatLC3::disassemble(const Context& context,
    GAddr addr, int flags) const
{
    if (flags & DisassembleSymbolic)
    {
        String buf(512);
        lc_symbolic_str(&MachineLC3::getState(context.machine()),
            buf.begin(), addr);
        buf.fixLength();
        return buf;
    }
    else
    {
        return Format::disassemble(context, addr, flags);
    }
}

String FormatLC3::formatState(const Context& context, int flags) const
{
    String str(1024);
    int lc3flags = 0;
    if (flags & Format::ViewColor) lc3flags |= LC_PA_COLOR;
    if (flags & DisassembleSymbolic) lc3flags |= LC_PA_SYMB;
    lc_print_state_str(&MachineLC3::getState(context.machine()),
        str.begin(), lc3flags, NULL);
    str.fixLength();
    return str;
}

String FormatLC3::format(
        const Context& context, const LocID& location,
        GVal value, int flags) const
{
    if ((flags & Format::DisplaySym) && location.isReg(LC_REG_PSR))
    {
        String buf = "";
        if (value & LC_FL_N) buf += 'n';
        if (value & LC_FL_Z) buf += 'z';
        if (value & LC_FL_P) buf += 'p';
        return buf;
    }
    else
    {
        return Format::format(context, location, value, flags);
    }
}

SuccessT FormatLC3::parse(
    const char *str, Reporter* reporter,
    const Context& context, const LocID& location,
    GVal* pVal) const
{
    SuccessT success;
    if (location.isReg(LC_REG_PSR) && isalpha(*str))
    {
        GVal val = 0;
        success = SuccessPass;
        for (; *str; str++)
        {
            switch(*str)
            {
             case 'n': val |= LC_FL_N; break;
             case 'z': val |= LC_FL_Z; break;
             case 'p': val |= LC_FL_P; break;
             default:
                success = SuccessFail;
                reporter->err("Uknown control code char: '%c'.", *str);
            }
        }
        *pVal = val;
    }
    else
    {
        success = Format::parse(str, reporter, context, location, pVal);
    }
    return success;
}

