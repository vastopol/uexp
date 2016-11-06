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


//
// TODO:
//  - Handling \033 kinds of literals.

#include "format/format.h"
#include "asmrule.h"
#include "model/machine.h"
#include "model/model.h"
#include <ctype.h>
#include <string.h>

AsmTokenizer::AsmTokenizer(const String& inFullLine, int inLineNum,
    Reporter* inExternalReporter)
 : mHasErrors(false)
 , mFullLine(inFullLine)
 , mLineIndex(0)
 , mLineNum(inLineNum)
 , mExternalReporter(inExternalReporter)
{
    grabToken();
}

void AsmTokenizer::msg(Reporter::MsgType type, const char* s)
{
    char lineStr[10];
    
    if (type == Reporter::Error)
    {
        mHasErrors = true;
    }
    
    if (mLineNum == 0) strcpy(lineStr, ":");
    else sprintf(lineStr, "%d", mLineNum);
    
    mExternalReporter->note("%s: %s -->%s<-- %s",
        lineStr,
        mFullLine.left(pos()).trim().c_str(),
        mCurrentToken.c_str(),
        mFullLine(mLineIndex, -1).trim().c_str());
    
    mExternalReporter->msg(type, String(" `-> ") + s);
}

bool AsmTokenizer::match(const char* str)
{
    if (mCurrentToken == str)
    {
        gobble();
        return true;
    }
    else
    {
        return false;
    }
}

void AsmTokenizer::gobble()
{
    grabToken();
}

const char* AsmTokenizer::processChar(const char* str, char* pCh)
{
    char c = *str;
    
    if (c == '\\')
    {
        switch(*++str)
        {
         case 'a': c = '\a'; break;
         case 'b': c = '\b'; break;
         case 'e': c = '\e'; break;
         case 'f': c = '\f'; break;
         case 'n': c = '\n'; break;
         case 'r': c = '\r'; break;
         case 't': c = '\t'; break;
         default: c = *str;
        }
    }
    else if (!c)
    {
        str--;
    }
    
    if (pCh) *pCh = c;
    
    return str + 1;
}

void AsmTokenizer::grabToken()
{
    const char* start;
    const char* errStr = 0;
    
    start = &mFullLine[mLineIndex];
    
    while (isspace(*start))
    {
        start++;
    }
    
    if (*start == '\0' || *start == ';' || *start == '!')
    {
        mRemaining = false;
        mCurrentToken = "";
    }
    else
    {
        const char* pos = start;
        
        if (isIdentifierPunctuation(*pos) || isalnum(*pos))
        {
            /*
             * Normal tokens begin with an underscore, dot, dollar-sign, or
             * percent-sign, and are followed by any number of underscores
             * or alpha-numeric characters.
             */
            pos++;
            while (*pos == '_' || isalnum(*pos))
            {
                pos++;
            }
        }
        else if (*pos == '"')
        {
            /*
             * String tokens begin with a double-quote.
             */
            for (pos++; *pos && *pos != '"'; pos = processChar(pos, NULL))
             { }
            
            if (*pos != '"') errStr = "Unterminated string.";
            else pos++;
        }
        else if (*pos == '\'')
        {
            /*
             * Single-character constant.
             */
            pos++;
            pos = processChar(pos, NULL);
            if (*pos != '\'') 
            {
                errStr = "Character constant must have only one character.";
            }
            if (*pos) pos++;
        }
        else
        {
            /*
             * Other punctuation marks are returned by themselves.
             */
            pos++;
        }
        
        mCurrentToken = String(start, pos);
        
        mLineIndex = pos - &mFullLine[0];
        mRemaining = true;
        
        if (errStr) err(errStr);
    }
}

String AsmTokenizer::peekString() const
{
    String result;
    const char *pos = mCurrentToken.begin();
    
    if (*pos == '"')
    {
        for (pos++; *pos && *pos != '"';)
        {
            char ch;
            pos = processChar(pos, &ch);
            result += ch;
        }
    }
    
    return result;
}

int AsmTokenizer::peekChar() const
{
    char c;
    processChar(mCurrentToken.begin() + 1, &c);
    return int(c) & 0xff;
}

void AsmTokenizer::clear()
{
    mRemaining = false;
    mCurrentToken = "";
    mFullLine = "";
    mLineIndex = 0;
}

//---------------------------------------------------------------------------

bool AsmRule::matches(GInstr instr) const
{
    return true;
}

void AsmRule::claimBits(GInstr instr, AsmRule const ** bitArray) const
{
}

String AsmRule::disassemble(const AsmContext& context, GInstr instr) const
{
    return "";
}

SuccessT AsmRule::assemble(const AsmContext& context,
    AsmTokenizer* tokenizer, GInstr* pInstr) const
{
    *pInstr = 0; return SuccessPass;
}

void AsmRule::extractTokens(SymTable* table) const
{
}

//---------------------------------------------------------------------------

bool TableRule::matches(GInstr instr) const
{
    const char* name =
        mSymTable->getname(Bits::extract(instr, mShift, mWidth));
    
    return name != 0;
}

void TableRule::claimBits(GInstr instr, AsmRule const ** bitArray) const
{
    for (int i = 0; i < mWidth; i++) bitArray[mShift + i] = this;
}

String TableRule::disassemble(const AsmContext&UNUSED(c), GInstr instr) const
{
    const char* name =
        mSymTable->getname(Bits::extract(instr, mShift, mWidth));
    return String(name ? name : "<error>");
}

SuccessT TableRule::assemble(const AsmContext& context,
    AsmTokenizer* tokenizer, GInstr *pInstr) const
{
    int val = mSymTable->lookup(tokenizer->peek());
    
    if (val >= 0 && val <= Bits::mask<int>(mWidth))
    {
        //printf("TableRule successful on '%s'\n", tokenizer->peek().c_str());
        *pInstr = val << mShift;
        tokenizer->gobble();
        return SuccessPass;
    }
    else
    {
        //printf("TableRule unsuccessful on '%s'\n", tokenizer->peek().c_str());
        return SuccessFail;
    }
}

void TableRule::extractTokens(SymTable* table) const
{
    table->addAll(*mSymTable);
}

//---------------------------------------------------------------------------

bool ImmRule::matches(GInstr instr) const
{
    return true;
}

void ImmRule::claimBits(GInstr instr, AsmRule const ** bitArray) const
{
    for (int i = 0; i < mWidth; i++) bitArray[mShift + i] = this;
}

long ImmRule::extend(GInstr instr) const
{
    long val;
    
    if (mDecType & Signed)
        val = Bits::extractSigned<long>(long(instr), mShift, mWidth);
    else
        val = long(Bits::extract(instr, mShift, mWidth));
    
    return val;
}

String ImmRule::disassemble(const AsmContext& context, GInstr instr) const
{
    long value;
    long realValue;
    
    value = extend(instr);
    //printf("val1=%d\n", int(value));
    value *= mFactor;
    //printf("val2=%d\n", int(value));
    
    realValue = value;
    if (mDecType & PCOff) realValue += context.addr();
    realValue += mOffset;
    //printf("real=%d\n", int(realValue));
    
    const char* name;
    
    if (mDisplayType & Format::DisplaySym
        && (name = context.getSymbolName(realValue)))
    {
        return name;
    }
    else if (mDisplayType & Format::DisplayHex)
    {
        return "x" + Format::formatHex(
            value & Bits::mask<GVal>(mDestBits), mDestBits);
    }
    else if (mDecType & Signed)
    {
        return "#" + Format::formatDec(value, mWidth);
    }
    else
    {
        return "#" + Format::formatDec(value, 0);
    }
}

SuccessT ImmRule::assemble(const AsmContext& context,
    AsmTokenizer* tokenizer, GInstr *pInstr) const
{
    GVal val;
    SuccessT suc;
    int oldPos = tokenizer->pos();
    ExprParser parser(context);
    
    if (mDecType & PCOff)
        parser.setSymbolOffset(mOffset + context.addr());
    else
        parser.setSymbolOffset(mOffset);
    
    if (!Failed(suc = parser.parseExpr(tokenizer, &val)))
    {
        bool overflow;
        int lastPos = tokenizer->pos();
        tokenizer->setPos(oldPos); // for error reporting
        
        if (val % mFactor != 0)
        {
            tokenizer->warn("Constant 0x%X not evenly divisible by %d; we will 'round towards zero'.\n",
                val, mFactor);
        }
        
        if (mDecType & Signed)
        {
            val /= mFactor;
            overflow = Bits::hasSignedOverflow(val, mWidth, mDestBits);
        }
        else
        {
            val = GVal(SVal(val) / mFactor); /* signed division */
            overflow = Bits::hasUnsignedOverflow(val, mWidth);
        }
        
        if (overflow)
        {
            tokenizer->err(
                (mDecType & Signed) ?
                "Constant %d overflows a signed %d-bit field."
                : "Constant 0x%X too large for unsigned %d-bit field.",
                int(val), mWidth);
            suc = SuccessFail;
        }
        else
        {
            *pInstr = (val & Bits::mask<GVal>(mWidth)) << mShift;
            suc = SuccessPass;
        }
        
        tokenizer->setPos(lastPos);
    }
    
    return suc;
}

//---------------------------------------------------------------------------

SuccessT ExprParser::parseExpr(AsmTokenizer* tokenizer, GVal *pVal) const
{
    SuccessT suc = SuccessPass;
    char op = '+';
    *pVal = 0;
    
    do {
        GVal tempVal;
        suc = min(suc, parseTerm(tokenizer, &tempVal));
        if (Failed(suc)) return SuccessFail;
        
        switch (op)
        {
         case '+': *pVal += tempVal; break;
         case '-': *pVal -= tempVal; break;
         default: abort();
        }
        
        op = tokenizer->peek()[0];
    } while (tokenizer->match("+") || tokenizer->match("-"));
    
    return suc;
}

SuccessT ExprParser::parseTerm(AsmTokenizer* tokenizer, GVal *pVal) const
{
    SuccessT suc = SuccessPass;
    char op = '*';
    *pVal = 1;
    
    do {
        GVal tempVal;
        
        suc = min(suc, parseFactor(tokenizer, &tempVal));
        if (suc == SuccessFail) return SuccessFail;
        
        switch (op)
        {
         case '*': *pVal *= tempVal; break;
         case '/': *pVal /= tempVal; break;
         default: abort();
        }
        
        op = tokenizer->peek()[0];
    } while (tokenizer->match("*") || tokenizer->match("/"));
    
    return suc;
}

LocID ExprParser::parseLocation(AsmTokenizer* tokenizer, bool mustParse) const
{
    char op = tokenizer->peek()[0];
    LocID::Type type = LocID::Invalid;
    long id = -1;
    bool wasmatch;
    
    if ((id = mMachine->arch().lookupReg(tokenizer->peek())) >= 0)
    {
        tokenizer->gobble();
        type = LocID::Reg;
    }
    else if (tokenizer->match(".") || tokenizer->match("pc"))
    {
        type = LocID::PC;
    }
    else
    {
        wasmatch = (tokenizer->match("@") || tokenizer->match("*")
            || tokenizer->match("^"));
        if (wasmatch || mustParse)
        {
            GVal val = 0;
            SuccessT suc;
            
            if (wasmatch) suc = parseFactor(tokenizer, &val);
            else suc = parseExpr(tokenizer, &val);
            
            if (suc != SuccessFail)
            {
                if (op == '@') type = LocID::MemByte;
                else if (op == '^') type = LocID::MemInstr;
                else type = LocID::MemWord;
                
                if (val > mMachine->lastAddress() - mMachine->bytesOf(type) + 1)
                {
                    tokenizer->err("Memory address 0x%04X is too large. (x%04X, x%04X, x%04X, %d)",
                        val, mMachine->lastAddress(),
                        mMachine->lastAddress() - mMachine->bytesOf(type) + 1,
                        mMachine->bytesOf(type),
                        type);
                    type = LocID::Invalid;
                }
                else
                {
                    id = val;
                }
            }
        }
    }
    
    return LocID(type, id);
}

SuccessT ExprParser::parseFactor(AsmTokenizer* tokenizer, GVal *pVal) const
{
    SuccessT suc = SuccessPass;
    
    if (mMachine)
    {
        LocID loc = parseLocation(tokenizer, false);
        
        if (loc.type() != LocID::Invalid)
        {
            *pVal = mMachine->get(loc);
            pVal = 0;
        }
    }
    
    if (tokenizer->hasErrors())
    {
        suc = SuccessFail;
    }
    else if (!pVal)
    {
    }
    else if (tokenizer->match("("))
    {
        suc = parseExpr(tokenizer, pVal);
        if (!tokenizer->match(")")) suc = SuccessFail;
    }
    else if (tokenizer->match("-"))
    {
        suc = parseFactor(tokenizer, pVal);
        *pVal = -(*pVal);
    }
    else if (tokenizer->match("~"))
    {
        suc = parseFactor(tokenizer, pVal);
        *pVal = ~(*pVal);
    }
    else if (tokenizer->match("#"))
    {
        bool negative = tokenizer->match("-");
        suc = parseNumber(tokenizer, tokenizer->peek().c_str(), 10, pVal);
        if (negative) *pVal = -(*pVal);
        tokenizer->gobble();
    }
    else if (mMachine && tokenizer->match("$"))
    {
        suc = parseExpr(tokenizer, pVal);
        if (*pVal > mMachine->lastAddress())
        {
            tokenizer->err("Memory address 'x%04X' too large.", *pVal);
        }
    }
    else
    {
        long val;
        const char* s = tokenizer->peek().c_str();
        
        if ((s[0] == 'x' && isxdigit(s[1])) || (s[0] == '0' && s[1] == 'x'))
        {
            s++;
            if (s[0] == 'x') s++; // 0x case
            suc = parseNumber(tokenizer, s, 16, pVal);
        }
        else if (s[0] >= '0' && s[0] <= '9')
        {
            if (s[0] == '0') suc = parseNumber(tokenizer, s, 8, pVal);
            else if (s[0] == 'b') suc = parseNumber(tokenizer, s, 2, pVal);
            else suc = parseNumber(tokenizer, s, 10, pVal);
        }
        else if (s[0] == '\'')
        {
            *pVal = tokenizer->peekChar();
        }
        else if (mSymTable && (val = mSymTable->lookup(s)) != -1)
        {
            *pVal = val - mSymbolOffset;
        }
        else
        { // PC offset!
            tokenizer->err(
                "The string '%s' is not a valid %sexpression.",
                s, mSymTable ? "label or " : "");
            suc = SuccessFail;
            *pVal = 0;
        }
        
        if (suc != SuccessFail)
        {
            tokenizer->gobble();
        }
    }
    
    return suc;
}

SuccessT ExprParser::parseNumber(Reporter* reporter, const char* str,
    int base, GVal *pVal) const
{
    SuccessT suc = SuccessPass;
    const char* pos = str;
    GVal val = 0;
    
    // TODO: Check for GVal overflow
    
    for (; *pos; pos++)
    {
        int digit;
        
        if (*pos >= '0' && *pos <= '9') digit = *pos - '0';
        else if (*pos >= 'A' && *pos <= 'Z') digit = *pos - 'A' + 10;
        else if (*pos >= 'a' && *pos <= 'z') digit = *pos - 'a' + 10;
        else digit = -1;
        
        if (digit < 0 || digit >= base)
        {
            reporter->err(
                "Character '%c' not allowed in base-%d constant..",
                *pos, base);
            suc = SuccessFail;
            break;
        }
        
        val *= base;
        val += digit;
    }
    
    *pVal = val;
    
    return suc;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

SuccessT MatchRule::parseBitSpec(const char* bits,
    GInstr* maskp, GInstr* valp)
{
    const char *s = bits;
    GInstr mask = 0;
    GInstr val = 0;
    
    while (*s)
    {
        switch (*s)
        {
         case '0':
            mask = (mask << 1) | 1;
            val = (val << 1);
            break;
         case '1':
            mask = (mask << 1) | 1;
            val = (val << 1) | 1;
            break;
         case '.':
         case '?':
            mask = (mask << 1);
            val = (val << 1);
            break;
         default:
            // ignore whitespace
            break;
        }
        
        s++;
    }
    
    *maskp = mask;
    *valp = val;
    
    return SuccessPass;
}

void MatchRule::claimBits(GInstr instr, AsmRule const ** bitArray) const
{
    for (GInstr tmpMask = mMask; tmpMask; tmpMask >>= 1)
    {
        if (tmpMask & 1)
        {
            *bitArray = this;
        }
        bitArray++;
    }
}

//---------------------------------------------------------------------------

String TokRule::disassemble(const AsmContext& context, GInstr instr) const
{
    return mTokText;
}

SuccessT TokRule::assemble(const AsmContext& context,
    AsmTokenizer* tokenizer, GInstr *pInstr) const
{
    bool found;
    
    if (mCaseSensitive) found = (tokenizer->peek() == mTokText);
    else found = (strcasecmp(tokenizer->peek().c_str(), mTokText.c_str()) == 0);
    
    //printf("Looking for '%s', I have '%s', found = %d\n", tokenizer->peek().c_str(), mTokText.c_str(), found);
    
    if (found)
    {
        tokenizer->gobble();
        *pInstr = 0;
        return SuccessPass;
    }
    else
    {
        return SuccessFail;
    }
}

void TokRule::extractTokens(SymTable* table) const
{
    table->add(mTokText, 1);
}

//---------------------------------------------------------------------------

MultiRule::MultiRule() { }
MultiRule::~MultiRule() { }

void MultiRule::addRules(
    AsmRule* r1, AsmRule* r2, AsmRule* r3, AsmRule* r4,
    AsmRule* r5, AsmRule* r6, AsmRule* r7, AsmRule* r8)
{
    if (r1) addRule(r1);
    if (r2) addRule(r2);
    if (r3) addRule(r3);
    if (r4) addRule(r4);
    if (r5) addRule(r5);
    if (r6) addRule(r6);
    if (r7) addRule(r7);
    if (r8) addRule(r8);
}

void MultiRule::extractTokens(SymTable* table) const
{
    for (size_t i = 0; i < mRules.size(); i++)
    {
        mRules[i]->extractTokens(table);
    }
}

//---------------------------------------------------------------------------

MuxRule::MuxRule(
    AsmRule* r1, AsmRule* r2, AsmRule* r3, AsmRule* r4,
    AsmRule* r5, AsmRule* r6, AsmRule* r7, AsmRule* r8)
{
    addRules(r1, r2, r3, r4, r5, r6, r7, r8);
}

bool MuxRule::matches(GInstr instr) const
{
    for (size_t i = 0; i < mRules.size(); i++)
    {
        if (mRules[i]->matches(instr)) return true;
    }
    return false;
}

void MuxRule::claimBits(GInstr instr, AsmRule const ** bitArray) const
{
    for (size_t i = 0; i < mRules.size(); i++)
    {
        if (mRules[i]->matches(instr))
        {
            mRules[i]->claimBits(instr, bitArray);
            break;
        }
    }
}

String MuxRule::disassemble(const AsmContext& context, GInstr instr) const
{
    for (size_t i = 0; i < mRules.size(); i++)
    {
        if (mRules[i]->matches(instr))
        {
            return mRules[i]->disassemble(context, instr);
        }
    }
    return "<error>";
}

SuccessT MuxRule::assemble(const AsmContext& context,
    AsmTokenizer* tokenizer, GInstr *pInstr) const
{
    SuccessT retval = SuccessFail;
    int oldPos = tokenizer->pos();
    int maxPos = oldPos; // keep track of the "best" or "longest" parse
    
    for (size_t i = 0; i < mRules.size(); i++)
    {
        //printf("MuxRule %p trying %p\n", this, mRules[i]);
        SuccessT result = mRules[i]->assemble(context, tokenizer, pInstr);
        
        if (!Failed(result))
        {
            retval = result;
            break;
        }
        
        maxPos = max(maxPos, tokenizer->pos());
        
        tokenizer->setPos(oldPos);
    }
    
    if (retval == SuccessFail)
    {
        // put the tokenizer to the position of the best parse
        tokenizer->setPos(maxPos);
    }
    
    return retval;
}

//---------------------------------------------------------------------------

CompoundRule::CompoundRule(
    AsmRule* r1, AsmRule* r2, AsmRule* r3, AsmRule* r4,
    AsmRule* r5, AsmRule* r6, AsmRule* r7, AsmRule* r8)
{
    addRules(r1, r2, r3, r4, r5, r6, r7, r8);
}

bool CompoundRule::matches(GInstr instr) const
{
    for (size_t i = 0; i < mRules.size(); i++)
    {
        if (!mRules[i]->matches(instr)) return false;
    }
    return true;
}

void CompoundRule::claimBits(GInstr instr, AsmRule const ** bitArray) const
{
    for (size_t i = 0; i < mRules.size(); i++)
    {
        mRules[i]->claimBits(instr, bitArray);
    }
}

String CompoundRule::disassemble(const AsmContext& context, GInstr instr) const
{
    String result = "";
    
    for (size_t i = 0; i < mRules.size(); i++)
    {
        String tmp = mRules[i]->disassemble(context, instr);
        
        // Try to be smart on when to add spaces.
        // Between two punctuation characters it is not important
        // to add whitespace.  However, always add a space after a comma
        // for more natural readability.
        //
        // In addition, do not add whitespace if the either the existing
        // string or the string being added is empty.
        
        if (result[0] && (!ispunct(result(-1)) || result(-1) == ',')
            && tmp[0] && (!ispunct(tmp[0]) || strchr("_.%$#", tmp[0])))
        {
            result += ' ';
        }
        
        result += tmp;
    }
    
    return result;
}

SuccessT CompoundRule::assemble(const AsmContext& context,
    AsmTokenizer* tokenizer, GInstr *pInstr) const
{
    SuccessT retval = SuccessPass;
    GInstr instr = 0;
    
    for (size_t i = 0; i < mRules.size(); i++)
    {
        GInstr tmpInstr;
        
        //printf("Compound rule %p proceeding to %p '%s'\n", this, mRules[i], tokenizer->peek().c_str());
        
        SuccessT result = mRules[i]->assemble(context, tokenizer, &tmpInstr);
        
        if (result != SuccessPass)
        {
            retval = result;
            if (retval == SuccessFail)
            {
                //printf("Compound rule %p failing!\n", this);
                break;
            }
        }
        //printf("Compound rule %p gives %X, let me OR it with %X\n", mRules[i], tmpInstr, instr);
        instr |= tmpInstr;
    }
    
    if (!Failed(retval))
    {
        //printf("Compound rule %p winning with %X!\n", this, instr);
        *pInstr = instr;
    }
    
    return retval;
}

//---------------------------------------------------------------------------

AsmRule* AsmRuleGenerator::makeRule(GInstr mask, GInstr val,
    const char* operands, CompoundRule* rule) const
{
    if (!rule) rule = new CompoundRule();
    if (mask) rule->addPart(new MatchRule(mask, val));
    Reporter nullReporter;
    AsmTokenizer tokenizer(operands, 0, &nullReporter);
    
    //printf("Starting\n");
    
    while (tokenizer.remaining())
    {
        AsmRule* subrule = lookup(tokenizer.peek().c_str());
        
        if (subrule)
        {
            //printf("  -> Adding subrule\n");
            rule->addPart(subrule);
        }
        else
        {
            //printf("  -> Adding [%s]\n", tokenizer.peek().c_str());
            rule->addPart(new TokRule(tokenizer.peek().c_str()));
        }
        
        tokenizer.gobble();
    }
    
    return rule;
}

AsmRule* AsmRuleGenerator::makeRule(const char *bitspec,
        const char* operands, CompoundRule* rule) const
{
    GInstr mask = 0;
    GInstr val = 0;
    
    MatchRule::parseBitSpec(bitspec, &mask, &val);
    
    return makeRule(mask, val, operands, rule);
}
    
void AsmRuleGenerator::bind(const char* name, AsmRule* rule)
{
    mNames.add(name, mRules.size());
    mRules.push_back(rule);
}

AsmRule* AsmRuleGenerator::lookup(const char* name) const
{
    int ruleNum = mNames.lookup(name);
    
    if (ruleNum >= 0)
    {
        return mRules[ruleNum];
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------

