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

#include "format.h"
#include "ui/simview.h"
#include "asm/asmrule.h"
#include "model/machine.h"
#include <ctype.h>

Format::Format(const Arch* inArch, const char *inPanelLayout)
 : mArch(inArch)
 , mPanelLayout(inPanelLayout)
{
    new(mLineOptions.make_back())Option(
        ViewColor, "Display &color", 'K', 0, true);
}

String Format::format(const Context& context, const LocID& location) const
{
    return format(context, location, context.machine().get(location));
}

String Format::format(
    const Context& context, const LocID& location,
    GVal value, int flags) const
{
    int bits = mArch->bitsOf(location);
    
    if ((flags&(DisplayHex|DisplayDec|DisplaySym)) == 0)
    {
        if (location.type() == LocID::PC)
        {
            flags |= DisplayHex|DisplaySym;
        }
        else
        {
            flags |= DisplayHex;
        }
    }
    
    return formatVal(value, context.symTable(), flags, bits);
}

String Format::formatVal(GVal val,
    const SymTable& table, int flags, int bits)
{
    const char* foundSym;
    
    if ((flags&DisplaySym) && (foundSym = table.getname(val)))
    {
        return String(foundSym);
    }
    else if ((flags&DisplayDec))
    {
        return formatDec(val, bits);
    }
    else
    {
        return "x" + formatHex(val, bits);
    }
}

String Format::formatDec(GVal val, int bits)
{
    char buf[32];
    char *start = buf + sizeof(buf) - 1;
    int negative = 0;
    
    if (bits != 0)
    {
        if (val & Bits::signMask<GVal>(bits))
        {
            negative = 1;
            val = -val;
        }
        
        val = Bits::truncate(val, bits);
    }
    
    *start = '\0';
    
    do {
        *--start = '0' + (val % 10);
        val /= 10;
    } while (val);
    
    if (negative)
    {
        *--start = '-';
    }
    
    return String(start);
}

int Format::calcDigits(GVal val, int base)
{
    int i = 0;
    
    while (val)
    {
        val /= base;
        i++;
    }
    
    return i;
}

String Format::formatHex(GVal val, int bits)
{
    if (bits == 0) bits = 32;
    
    int numDigits = (bits + 3) / 4;
    String result(numDigits);
    
    for (int pos = numDigits - 1; pos >= 0; --pos)
    {
        int digit = val & 0xF;
        char c;
        
        if (digit < 10) c = '0' + digit;
        else c = 'A' + digit - 10;
        
        result[pos] = c;
        
        val >>= 4;
    }
    
    result.terminate();
    
    return result;
}

SuccessT Format::parse(
    const char *str, Reporter* reporter,
    const Context& context,
    const LocID& location, GVal* pVal) const
{
    SuccessT suc = SuccessFail;
    bool tryAgain = true;
    
    if (location.type() == LocID::MemInstr)
    {
        GInstr instr;
        AsmTokenizer tokenizer(str, 0, reporter);
        
        if (mArch->isTokenReserved(tokenizer.peek()))
        {
            suc = asmRule().assemble(
                AsmRule::AsmContext(location.id(), &context.symTable(), mArch),
                &tokenizer, &instr);
            if (!Failed(suc))
            {
                *pVal = GVal(instr);
                if (tokenizer.remaining())
                    tokenizer.warn("Remaining tokens left over.");
            }
            else if (!tokenizer.hasErrors())
            {
                tokenizer.err("Parse error.");
            }
            
            tryAgain = false;
        }
    }
    
    if (suc == SuccessFail && tryAgain)
    {
        suc = parseVal(str, reporter, context, pVal,
           mArch->bitsOf(location),
           location.type() == LocID::PC);
    }
    
    
    return suc;
}

// TODO: Separate memory instructions, etc.
// Depends on a more sophisticated parser

String Format::formatDest(const Context& context, const LocID& dest) const
{
    switch(dest.type())
    {
      case LocID::PC:
        return "pc";
      case LocID::MemInstr:
        return "^" + format(context, LocID::PC, dest.id());
      case LocID::MemWord:
        return "*" + format(context, LocID::PC, dest.id());
      case LocID::MemByte:
        return "@" + format(context, LocID::PC, dest.id());
      case LocID::Reg:
        return mArch->regName(dest.id());
      default:
        return "<invalid>";
    }
}

LocID Format::parseDest(
    const char *str, Reporter* reporter, const Context& context) const
{
    ExprParser parser(&context.symTable(), &context.machine());
    AsmTokenizer tokenizer(str, 0, reporter);
    LocID locid(parser.parseLocation(&tokenizer, true));
    
    if (tokenizer.remaining())
    {
        tokenizer.err("Cannot parse beyond this point.");
        tokenizer.err("Be careful about parentheses, especially when dereferencing.");
        locid = LocID::Invalid;
    }
    
    return locid;
}

SuccessT Format::parseVal(const char *s, Reporter* reporter,
    const Context& context, GVal* pVal, int bits, bool isAddress) const
{
    ExprParser parser(&context.symTable(), &context.machine());
    AsmTokenizer tokenizer(s, 0, reporter);
    SuccessT suc = parser.parseExpr(&tokenizer, pVal);
    
    if (!Failed(suc) && Bits::hasSignedOverflow(*pVal, bits))
    {
        int badVal = *pVal;
        *pVal &= Bits::mask<GVal>(bits);
        tokenizer.setPos(0);
        tokenizer.warn("Value 0x%X (%d) too large; truncated to x%X to fit.",
            unsigned(badVal), int(badVal), unsigned(*pVal));
    }
    
    return suc;
}

void Format::color(bool useColor, String* str, const char* colorstr)
{
    if (useColor)
    {
        (*str) += "\033[";
        (*str) += colorstr;
        (*str) += 'm';
    }
}

String Format::formatLine(const Context& context, const LocID& location,
    int flags) const
{
    GVal val = context.machine().get(location);
    int bits = mArch->bitsOf(location);
    String result;
    const char *symname = context.symTable().getname(location.id());
    bool useColor = (flags & ViewColor) != 0;
    GAddr addr = location.id();
    
    if (location.type() != LocID::MemInstr)
    {
        if (symname)
        {
            color(useColor, &result, "1;35");
            result += String(symname).padRight('.', 11);
        }
        else
        {
            result += String(11, ' ');
        }
    }
    
    if (location.type() != LocID::MemInstr
        || mArch->instrBits() <= 16)
    {
        color(useColor, &result, "0;32");
        result += formatHex(addr, mArch->addrBits());
        color(useColor, &result, "0");
        result += ": ";
    }
    
    color(useColor, &result, "1;34");
    result += formatHex(val, bits);
    result += ' ';
    
    if (location.type() != LocID::MemInstr)
    {
        for (int i = bits; i--;)
        {
            if (i % 8 == 7) color(useColor, &result, "0;30;46");
            else if (i % 8 == 3) color(useColor, &result, "1;37;44");
            result += ('0' + ((val >> i) & 1));
        }
        color(useColor, &result, "0");
        result += ' ';
    }
    
    if (location.type() != LocID::MemInstr
        || mArch->instrBits() <= 16)
    {
        color(useColor, &result, "0;36");
        result += formatDec(val, bits).padRight(
            ' ', calcDigits(Bits::mask<GVal>(bits), 10) + 1);
        color(useColor, &result, "0");
        result += ' ';
    }
    
    if (location.type() == LocID::MemInstr)
    {
        result += formatInstrBits(val, useColor);
        result += ' ';
        
        if (symname)
        {
            color(useColor, &result, "1;35");
            if (context.machine().pc() == addr)
                result += String(symname).padRight(' ', 11) + "->";
            else
                result += String(symname).padRight(' ', 12) + " " ;
        }
        else
        {
            color(useColor, &result, "0;35");
            result +=
                formatHex(addr, mArch->addrBits())
                .padCenter(' ', 11);
            result += ((context.machine().pc() == addr) ? "->" : "  ");
        }
        color(useColor, &result, "0");
        result += disassemble(context, addr, flags);
    }
    else
    {
        if (val < 128 && isgraph(val))
        {
            color(useColor, &result, "0;33");
            result += '\'';
            result += char(val);
            result += "' ";
        }
        else if ((symname = context.symTable().getname(val)))
        {
            color(useColor, &result, "0;34");
            result += symname;
        }
    }
    
    color(useColor, &result, "0");
    
    return result;
}

String Format::disassemble(const Context& context,
    GAddr addr, int flags) const
{
    return asmRule().disassemble(
        AsmRule::AsmContext(addr, &context.symTable(), mArch),
        context.machine().memInstr(addr));
}

String Format::formatInstrBits(GInstr instr, bool useColor)
    const
{
    static const char * const colors[3] =
        { "0;30;46", "1;37;44", "0;37;40" };
    int bits = mArch->instrBits();
    Array<const AsmRule*> rules(bits, static_cast<const AsmRule*>(0));
    
    asmRule().claimBits(instr, rules.begin());
    
    String result;
    int col = 1;
    bool allsame = true;
    
    for (int i = bits; --i;)
    {
        if (rules[i] != rules[i-1])
        {
            allsame = false;
            break;
        }
    }
    
    for (int i = bits; i--;)
    {
        if (useColor && (i == bits-1 || rules[i] != rules[i+1]
            || (allsame && (i % 4 == 3))))
        {
            const char *colstr;
            
            if (!rules[i] || dynamic_cast<const MatchRule*>(rules[i]))
            {
                colstr = colors[2];
            }
            else
            {
                col ^= 1;
                colstr = colors[col];
            }
            
            color(true, &result, colstr);
        }
        
        result += char(((instr >> i) & 1) + '0');
    }
    
    if (useColor) result += "\033[0m";
    
    return result;
}

String Format::dumpLine(const Context& context, GAddr startAddr, int flags,
    GAddr highlightAddr) const
{
    String line;
    GAddr count = calcDumpBytes();
    GAddr i;
    bool useColor = (flags & ViewColor) != 0;
    
    startAddr &= mArch->addrMask();
    
    color(useColor, &line, "0;32");
    line += " ";
    line += Format::formatHex(startAddr, mArch->addrBits());
    color(useColor, &line, "0");
    line += ":";
    color(useColor, &line, "1;34");
    
    for (i = startAddr; (i - startAddr) < count; i++)
    {
        if (i == highlightAddr) color(useColor, &line, "1;36");
        line += " ";
        line += Format::formatHex(context.machine().memByte(i),
            mArch->byteBits());
        if (i == highlightAddr) color(useColor, &line, "1;34");
    }
    
    line += "  ";
    
    for (i = startAddr; (i - startAddr) < count; i++)
    {
        dumpChar(useColor, &line, context.machine().memByte(i));
    }
    
    if (mArch->byteBits() == 16)
    {
        line += "  ";
        
        for (i = startAddr; (i - startAddr) < count; i++)
        {
            int v = context.machine().memByte(i);
            dumpChar(useColor, &line, v&255);
            dumpChar(useColor, &line, v>>8);
        }
    }
    
    color(useColor, &line, "0");
    
    return line;
}

int Format::calcDumpBytes() const
{
    return 128 / mArch->byteBits();
}

void Format::dumpChar(bool useColor, String* pStr, int v)
{
    if (v < 128 && isgraph(v))
    {
        color(useColor, pStr, "0;33");
        (*pStr) += v;
    }
    else if (v < 128 && isspace(v))
    {
        int c;
        color(useColor, pStr, "0;36");
        switch(v)
        {
         case '\n': c = '/'; break;
         case '\r': c = '\\'; break;
         case ' ': c = '_'; break;
         default: c = '.'; break;
        }
        (*pStr) += c;
    }
    else if (v == 0)
    {
        color(useColor, pStr, "0;34");
        (*pStr) += '0';
    }
    else if (v < 32)
    {
        color(useColor, pStr, "0;34");
        (*pStr) += v+64;
    }
    else
    {
        color(useColor, pStr, "0");
        (*pStr) += '.';
    }
}

const AsmRule& Format::asmRule() const
{
    return mArch->asmRule();
}
