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

#include "archlc3.h"
#include "formatlc3.h"
#include "machinelc3.h"
#include "asm/asmformat.h"
#include "asm/asmrule.h"

ArchLC3 ArchLC3::mInstance;

ArchLC3::ArchLC3()
 : Arch("LC-3", Sizes(16, 16, 1, 1, 1), new FormatLC3(), 6)
{
    mRegTable.add("R0", LC_REG_R0);
    mRegTable.add("R1", LC_REG_R1);
    mRegTable.add("R2", LC_REG_R2);
    mRegTable.add("R3", LC_REG_R3);
    mRegTable.add("R4", LC_REG_R4);
    mRegTable.add("R5", LC_REG_R5);
    mRegTable.add("R6", LC_REG_R6);
    mRegTable.add("R7", LC_REG_R7);
    mRegTable.add("PC", LC_REG_PC);
    mRegTable.add("PSR", LC_REG_PSR);
    mRegTable.add("CC", LC_REG_PSR);
    
    new(mRunOptions.make_back())Option(0x01,
        "True traps using LC3 OS", 't');
    
    setAsmRule(makeAsmTreeRoot());
}

Machine* ArchLC3::makeMachine(SimView* view) const
{
    return new MachineLC3(view);
}

void ArchLC3::addDefaultSymbols(SymTable* table) const
{
    // KBSR, etc
    table->add("KBSR", LC_KBSR);
    table->add("KBDR", LC_KBDR);
    table->add("DSR", LC_DSR);
    table->add("DDR", LC_DDR);
    table->add("MCR", LC_MCR);
}

AsmRule* ArchLC3::makeAsmTreeRoot() const
{
    AsmRuleGenerator gen;
    
    gen.bind("reg0", new TableRule(&mRegTable, 0, 3));
    gen.bind("reg6", new TableRule(&mRegTable, 6, 3));
    gen.bind("reg9", new TableRule(&mRegTable, 9, 3));
    
    gen.bind("imm5d",
        new ImmRule(16, Format::DisplayDec, ImmRule::Signed, 0, 5));
    gen.bind("imm5h",
        new ImmRule(16, Format::DisplayHex, ImmRule::Signed, 0, 5));
    gen.bind("imm6d",
        new ImmRule(16, Format::DisplayDec, ImmRule::Signed, 0, 6));
    
    gen.bind("pcoff11",
        new ImmRule(16, Format::DisplayHexSym,
                    ImmRule::PCOff|ImmRule::Signed, 0, 11, 1));
    gen.bind("pcoff9",
        new ImmRule(16, Format::DisplayHexSym,
                    ImmRule::PCOff|ImmRule::Signed, 0, 9, 1));
    
    // reg or immediate, but disassembles as decimal
    gen.bind("ri5d",
        new MuxRule(
            gen.makeRule(".... .... ..00 0...", "reg0"),
            gen.makeRule(".... .... ..1. ....", "imm5d")));
    // reg or immediate, but disassembles as hex
    gen.bind("ri5h",
        new MuxRule(
            gen.makeRule(".... .... ..00 0...", "reg0"),
            gen.makeRule(".... .... ..1. ....", "imm5h")));
    
    gen.bind("regOff", "reg9,pcoff9");
    gen.bind("regRegOff", "reg9,reg6,imm6d");
    gen.bind("vec8", new ImmRule(8, Format::DisplayHex, ImmRule::Unsigned, 0, 8));
    
    gen.add("0001 .... .... ....", "add reg9,reg6,ri5d");
    gen.add("0101 .... .... ....", "and reg9,reg6,ri5h");
    gen.add("1001 .... ..11 1111", "not reg9,reg6");
    gen.add("0100 000. ..00 0000", "jsrr reg6");
    gen.add("0100 1... .... ....", "jsr pcoff11");
    gen.add("1100 000. ..00 0000", "jmp reg6");
    gen.add("0010 .... .... ....", "ld regOff");
    gen.add("1010 .... .... ....", "ldi regOff");
    gen.add("0011 .... .... ....", "st regOff");
    gen.add("1011 .... .... ....", "sti regOff");
    gen.add("1110 .... .... ....", "lea regOff");
    gen.add("0110 .... .... ....", "ldr regRegOff");
    gen.add("0111 .... .... ....", "str regRegOff");
    
    SymTable* brTable = new SymTable(false);
    brTable->add("brp", 1);
    brTable->add("brz", 2);
    brTable->add("brzp", 3);
    brTable->add("brn", 4);
    brTable->add("brnp", 5);
    brTable->add("brnz", 6);
    brTable->add("brnzp", 7);
    brTable->add("br", 7);
    
    gen.bind("brXYZ", new TableRule(brTable, 9, 3));
    gen.add("0000 .... .... ....", "brXYZ pcoff9");
    
    SymTable* trapTable = new SymTable(false);
    trapTable->add("getc", 0x20);
    trapTable->add("out", 0x21);
    trapTable->add("puts", 0x22);
    trapTable->add("in", 0x23);
    trapTable->add("putsp", 0x24);
    trapTable->add("halt", 0x25);
    
    gen.add(
        new CompoundRule(
            new TableRule(trapTable, 0, 8),
            new MatchRule("1111 0000 .... ....")));
    
    gen.add("1111 0000 .... ....", "trap vec8");
    gen.add("1100 0001 1100 0000", "ret");
    gen.add("1000 0000 0000 0000", "rti");
    gen.add("0000 0000 0000 0000", "noop");
    
    gen.add(
        new CompoundRule(
            new TokRule(".fill"),
            new ImmRule(16, Format::DisplayHex, ImmRule::Unsigned, 0, 16)));
    
    return gen.extractRoot();
}
