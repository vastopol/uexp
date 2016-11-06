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

#include "allbf.h"
#include "asm/asmformat.h"
#include "asm/asmrule.h"
#include "model/model.h"
#include <ctype.h>

//---------------------------------------------------------------------------
// ArchBF
//---------------------------------------------------------------------------

ArchBF ArchBF::mInstance;

ArchBF::ArchBF()
 : Arch("BF", Sizes(8, 16, 1, 1, 2), new FormatBF(), 1, ".lc")
{
    mRegTable.add("dp", 0);
    mRegTable.add("bsp", 1);

    AsmRuleGenerator gen;
    
    MuxRule* root = new MuxRule();
    root->addOption(gen.makeRule(0xff, '<', "left"));
    root->addOption(gen.makeRule(0xff, '>', "right"));
    root->addOption(gen.makeRule(0xff, '>', "right"));
    root->addOption(gen.makeRule(0xff, '+', "inc"));
    root->addOption(gen.makeRule(0xff, '-', "dec"));
    root->addOption(gen.makeRule(0xff, '[', "openb"));
    root->addOption(gen.makeRule(0xff, ']', "closeb"));
    root->addOption(gen.makeRule(0xff, '.', "out"));
    root->addOption(gen.makeRule(0xff, ',', "in"));
    root->addOption(gen.makeRule(0xff, '"', "printint"));
    root->addOption(gen.makeRule(0xff, 'X', "halt"));
    root->addOption(gen.makeRule(0xff, '\0', "halt"));
    root->addOption(gen.makeRule(0xff, ' ', "noops"));
    root->addOption(gen.makeRule(0xff, '\n', "noopn"));
    
    root->addOption(
        new CompoundRule(
            new TokRule(".fill"),
            new ImmRule(8, Format::DisplayHex, ImmRule::Signed, 0, 8)));

    setAsmRule(root);
}

Machine* ArchBF::makeMachine(SimView* view) const
{
    return new MachineBF(view);
}

void ArchBF::addDefaultSymbols(SymTable* table) const
{
}

//---------------------------------------------------------------------------
// FormatBF
//---------------------------------------------------------------------------

FormatBF::FormatBF()
 : Format(&ArchBF::instance(), "1:5:0-,1-,pc,w,e")
{
}

String FormatBF::formatState(const Context& context, int flags) const
{
    const Machine& mach = context.machine();
    char buf[1024];
    char *s = buf;
    
    s += sprintf(s, " Now at x%04X: %-32s", mach.pc(),
        disassemble(context, mach.pc()).c_str());
    s += sprintf(s, "(dp=x%04X  bsp=x%04X  cur=x%02X)\n",
        mach.reg(0), mach.reg(1), mach.memWord(mach.reg(0)));
    
    return String(buf);
}

//---------------------------------------------------------------------------
// MachineBF
//---------------------------------------------------------------------------

MachineBF::MachineBF(SimView* inView)
 : Machine(inView, &ArchBF::instance())
{
    reinit();
}

void MachineBF::runOne()
{
    char inst = mMem[mPC];
    mPC++;
    switch (inst)
    {
     case 'X':
     case '\0':
        mHalted = true;
        mPC--;
        break;
     case '+':
        mMem[mReg[0]]++;
        break;
     case '-':
        mMem[mReg[0]]--;
        break;
     case '>':
        mReg[0]++;
        break;
     case '<':
        mReg[0]--;
        break;
     case '[':
        if (mMem[mReg[0]] == 0)
        {
            // hack: find matching ]
            int p;
            int c = 1;
            for (p = mPC + 1; mMem[p] && c != 0; p++)
            {
                if (mMem[p] == '[') c++;
                else if (mMem[p] == ']') c--;
            }
            mPC = p;
        }
        else
        {
            mReg[1]--;
            mMem[mReg[1]] = mPC;
        }
        break;
     case ']':
        if (mMem[mReg[0]] == 0)
        {
            mReg[1]++;
        }
        else
        {
            mPC = mMem[mReg[1]];
        }
        break;
     case '.':
        mView->writeChar(mMem[mReg[0]]);
        break;
     case ',':
        mMem[mReg[0]] = mView->readChar();
        break;
     case '"':
     {
        char buf[10];
        sprintf(buf, "%d", unsigned(mMem[mReg[0]]));
        for (char *s = buf; *s; s++)
        {
            mView->writeChar(*s);
        }
     }
        break;
     case ' ':
     case '\n':
        // no-op
        break;
     default:
        mView->warn("Invalid instruction '%c' at x%04X.", inst, mPC);
    }
}

void MachineBF::fastRun()
{
    while (!mHalted) runOne();
}

bool MachineBF::isProperlyHalted() const
{
    return (memInstr(mPC) == 'X' || memInstr(mPC) == 0);
}

void MachineBF::reinit()	
{
    memset(mMem, 0, sizeof(mMem));
    mReg[0] = 512; // data pointer
    mReg[1] = 511; // block stack pointer
    mPC = 0;
    mHalted = false;
}

bool MachineBF::enteringSubroutine(GAddr* addr) const
{
    // there are no subroutines
    return false;
}
