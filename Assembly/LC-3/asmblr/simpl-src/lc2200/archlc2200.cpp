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
#include "formatlc2200.h"
#include "machinelc2200.h"
#include "asmformatlc2200.h"
#include "asm/asmformat.h"
#include "asm/asmrule.h"

ArchLC2200 ArchLC2200::mInstance;

ArchLC2200::ArchLC2200()
 : Arch("LC-2200", Sizes(8, 8, 1, 2, 1), new FormatLC2200(), 13, ".lc")
{
    mRegTable.add("$zero", 0);
    mRegTable.add("$at", 1);
    mRegTable.add("$v0", 2);
    mRegTable.add("$a0", 3);
    mRegTable.add("$a1", 4);
    mRegTable.add("$a2", 5);
    mRegTable.add("$t0", 6);
    mRegTable.add("$t1", 7);
    mRegTable.add("$t2", 8);
    mRegTable.add("$s0", 9);
    mRegTable.add("$s1", 10);
    mRegTable.add("$s2", 11);
    mRegTable.add("$k0", 12);
    mRegTable.add("$sp", 13);
    mRegTable.add("$pr", 14);
    mRegTable.add("$ra", 15);
    
    setAsmRule(makeAsmTreeRoot());
}

Machine* ArchLC2200::makeMachine(SimView* view) const
{
    return new MachineLC2200(view);
}

void ArchLC2200::addDefaultSymbols(SymTable* table) const
{
}

AsmFormat* ArchLC2200::makeAssembler(SymTable* table) const
{
    return new AsmFormatLC2200(table);
}

AsmRule* ArchLC2200::makeAsmTreeRoot() const
{
    AsmRuleGenerator gen;
    
    // operand types
    gen.bind("regA", new TableRule(&mRegTable, 9, 4));
    gen.bind("regB", new TableRule(&mRegTable, 5, 4));
    gen.bind("regC", new TableRule(&mRegTable, 1, 4));
    gen.bind("imm2u",
        new ImmRule(2, Format::DisplayDec, ImmRule::Unsigned, 0, 2));
    gen.bind("imm5",
        new ImmRule(8, Format::DisplayDec, ImmRule::Signed, 0, 5));
    gen.bind("imm8",
        new ImmRule(8, Format::DisplayDec, ImmRule::Signed, 0, 5));
    gen.bind("pcoff5x2",
        new ImmRule(8, Format::DisplayHexSym,
            ImmRule::Signed|ImmRule::PCOff, 0, 5, 2, 2));
    gen.bind("address", "imm8(regB)");
    gen.bind("rRule", "regA,regB,regC");
    
    // instructions
    gen.add("0000 0000 0000 0000", "noop");
    gen.add("000. .... .... ....", "add rRule");
    gen.add("001. .... .... ....", "nand rRule");
    gen.add("010. .... .... ....", "addi regA,regB,imm5");
    gen.add("011. .... .... ....", "lw regA,address");
    gen.add("100. .... .... ....", "sw regA,address");
    gen.add("101. .... .... ....", "beq regA,regB,pcoff5x2");
    gen.add("1100 .... ...0 0000", "jalr regA,regB");
    gen.add("1110 0000 0000 0000", "halt");
    gen.add("111. .... .... ....", "spop imm2u");
    
    // rules for disassembly
    gen.add(
        new CompoundRule(
            new TokRule(".filli"),
            new ImmRule(16, Format::DisplayHex, ImmRule::Signed, 0, 16)));
    gen.add(0, 1, "la !! keep from parsing as label");
    
    return gen.extractRoot();
}
