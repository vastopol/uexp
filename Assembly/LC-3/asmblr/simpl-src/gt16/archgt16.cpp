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

#include "archgt16.h"
#include "formatgt16.h"
#include "machinegt16.h"
#include "asmformatgt16.h"
#include "asm/asmformat.h"
#include "asm/asmrule.h"

ArchGT16 ArchGT16::mInstance;

ArchGT16::ArchGT16()
 : Arch("LC-2200", Sizes(16, 16, 1, 1, 1), new FormatGT16(), 13, ".lc")
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

Machine* ArchGT16::makeMachine(SimView* view) const
{
    return new MachineGT16(view);
}

void ArchGT16::addDefaultSymbols(SymTable* table) const
{
}

AsmFormat* ArchGT16::makeAssembler(SymTable* table) const
{
    return new AsmFormatGT16(table);
}

AsmRule* ArchGT16::makeAsmTreeRoot() const
{
    AsmRuleGenerator gen;
    
    // operand types
    gen.bind("regX", new TableRule(&mRegTable, 9, 4));
    gen.bind("regY", new TableRule(&mRegTable, 5, 4));
    gen.bind("regZ", new TableRule(&mRegTable, 1, 4));
    gen.bind("imm2u",
        new ImmRule(2, Format::DisplayDec, ImmRule::Unsigned, 0, 2));
    gen.bind("imm5",
        new ImmRule(16, Format::DisplayDec, ImmRule::Signed, 0, 5));
    gen.bind("imm8",
        new ImmRule(16, Format::DisplayDec, ImmRule::Signed, 0, 5));
    gen.bind("pcoff5",
        new ImmRule(16, Format::DisplayHexSym,
            ImmRule::Signed|ImmRule::PCOff, 0, 5, 1, 1));
    
    // instructions
    gen.add("0000 0000 0000 0000", "noop");
    gen.add("000. .... .... ....", "add regX,regY,regZ");
    gen.add("001. .... .... ....", "nand regX,regY,regZ");
    gen.add("010. .... .... ....", "addi regX,regY,imm5");
    gen.add("011. .... .... ....", "lw regX,imm8(regY)");
    gen.add("100. .... .... ....", "sw regX,imm8(regY)");
    gen.add("101. .... .... ....", "beq regX,regY,pcoff5");
    gen.add("110. .... ...0 0000", "jalr regX,regY");
    gen.add("1110 0000 0000 0000", "halt");
    gen.add("1110 0000 0000 00..", "spop imm2u");
    
    // rules for disassembly
    gen.add(
        new CompoundRule(
            new TokRule(".filli"),
            new ImmRule(16, Format::DisplayHex, ImmRule::Signed, 0, 16)));
    
    gen.add(0, 1, "la !! keep from parsing as label");
    
    return gen.extractRoot();
}
