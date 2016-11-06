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
#include "formatmips.h"
#include "machinemips.h"
#include "asmformatmips.h"
#include "asm/asmformat.h"
#include "asm/asmrule.h"

ArchMips ArchMips::mInstance;

// TODO(gboyer): unfortunately a 32-bit address space messes things up,
// so only using 31 bits.

ArchMips::ArchMips()
 : Arch("Mips", Sizes(8, 32, 4, 4, 4), new FormatMips(), 13, ".lc")
{
    mRegTable.add("$zero", 0);
    mRegTable.add("$at", 1);
    mRegTable.add("$v0", 2);
    mRegTable.add("$v1", 3);
    mRegTable.add("$a0", 4);
    mRegTable.add("$a1", 5);
    mRegTable.add("$a2", 6);
    mRegTable.add("$a3", 7);
    mRegTable.add("$t0", 8);
    mRegTable.add("$t1", 9);
    mRegTable.add("$t2", 10);
    mRegTable.add("$t3", 11);
    mRegTable.add("$t4", 12);
    mRegTable.add("$t5", 13);
    mRegTable.add("$t6", 14);
    mRegTable.add("$t7", 15);
    mRegTable.add("$s0", 16);
    mRegTable.add("$s1", 17);
    mRegTable.add("$s2", 18);
    mRegTable.add("$s3", 19);
    mRegTable.add("$s4", 20);
    mRegTable.add("$s5", 21);
    mRegTable.add("$s6", 22);
    mRegTable.add("$s7", 23);
    mRegTable.add("$t8", 24);
    mRegTable.add("$t9", 25);
    mRegTable.add("$k0", 26);
    mRegTable.add("$k1", 27);
    mRegTable.add("$gp", 28);
    mRegTable.add("$sp", 29);
    mRegTable.add("$fp", 30);
    mRegTable.add("$ra", 31);
    
    setAsmRule(makeAsmTreeRoot());
}

Machine* ArchMips::makeMachine(SimView* view) const
{
    return new MachineMips(view);
}

void ArchMips::addDefaultSymbols(SymTable* table) const
{
}

AsmFormat* ArchMips::makeAssembler(SymTable* table) const
{
    return new AsmFormatMips(table);
}


AsmRule* ArchMips::makeAsmTreeRoot() const
{
    AsmRuleGenerator gen;
    
    // TODO(gboyer): Find some reference binaries in mips
    // TODO(gboyer): Task List:
    //  .global, .data, .text, .ent
    // Translation for multi-byte: sd, division, linking
    
    // New symbol table structure with scoping info?
    
    gen.bind("$rs", new TableRule(&mRegTable, 21, 5));
    gen.bind("$rt", new TableRule(&mRegTable, 16, 5));
    gen.bind("$rd", new TableRule(&mRegTable, 11, 5));
    gen.bind("imm16s",
        new ImmRule(32, Format::DisplayDec, ImmRule::Signed, 0, 16));
    gen.bind("negimm16s", new NegateRule(gen.lookup("imm16s"), 0, 16));
    gen.bind("imm16u",
        new ImmRule(32, Format::DisplayHex, ImmRule::Unsigned, 0, 16));
    gen.bind("pcoff16",
        new ImmRule(32, Format::DisplayDecSym, ImmRule::PCOff, 0, 16, 4, 4));
    gen.bind("addr26x4",
        new ImmRule(32, Format::DisplayHex, ImmRule::Unsigned, 0, 16, 0, 4));
    gen.bind("shamt",
        new ImmRule(5, Format::DisplayDec, ImmRule::Unsigned, 6, 5));
    gen.bind("relative", "imm16s($rs)");
    
    gen.bind("jrule", "addr26x4");
    gen.bind("irule_mem", "$rt, imm16s($rs)");
    
    // TODO(gboyer): syscall
    
    gen.add("000000.. ........ ........ ..000000", "sll $rd, $rs, shamt");
    gen.add("000000.. ........ .....000 00000000", "move $rd, $rs");         //N
    gen.add("000000.. ........ ........ ..000010", "srl $rd, $rs, shamt");
    gen.add("000000.. ........ ........ ..000011", "sra $rd, $rs, shamt");
    gen.add("000000.. ........ ........ ..000100", "sllv $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..000110", "srlv $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..001000", "jr $rs");
    gen.add("00000000 00000000 .....000 00010000", "mfhi $rd");
    gen.add("00000000 00000000 .....000 00010010", "mflo $rd");
    gen.add("00000000 00000000 .....000 00010001", "mthi $rd");
    gen.add("00000000 00000000 .....000 00010011", "mtlo $rd");
    gen.add("000000.. ........ 00000000 00011000", "mult $rs, $rt");
    gen.add("000000.. ........ 00000000 00011001", "multu $rs, $rt");
    gen.add("000000.. ........ 00000000 00011010", "div $rs, $rt");
    gen.add("000000.. ........ 00000000 00011011", "divu $rs, $rt");
    gen.add("000000.. ........ ........ ..100000", "add $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..100001", "addu $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..100010", "sub $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..100011", "subu $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..100100", "and $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..100101", "or $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..100110", "xor $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..101010", "slt $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..101010", "sgt $rd, $rt, $rs");     //N
    gen.add("000000.. ........ ........ ..101011", "sltu $rd, $rs, $rt");
    gen.add("000000.. ........ ........ ..101011", "sgtu $rd, $rt, $rs");    //N
    
    gen.add("000001.. ...00000 ........ ........", "bltz $rs, pcoff16");
    gen.add("000001.. ...00001 ........ ........", "bgez $rs, pcoff16");
    gen.add("000001.. ...01000 ........ ........", "bltzal $rs, pcoff16");
    gen.add("000001.. ...01001 ........ ........", "bgezal $rs, pcoff16");
    gen.add("000010.. ........ ........ ........", "j addr26x4");
    gen.add("000011.. ........ ........ ........", "jal addr26x4");
    gen.add("000100.. ........ ........ ........", "beq $rs, $rt, pcoff16");
    gen.add("00010000 00000000 ........ ........", "b pcoff16");
    gen.add("000101.. ........ ........ ........", "bne $rs, $rt, pcoff16");
    gen.add("000110.. ........ ........ ........", "ble $rs, $rt, pcoff16");
    gen.add("000110.. ........ ........ ........", "bge $rt, $rs, pcoff16"); //N
    gen.add("000110.. ...00000 ........ ..000000", "blez $rs, pcoff16");
    gen.add("000111.. ........ ........ ........", "bgt $rs, $rt, pcoff16");
    gen.add("000111.. ........ ........ ........", "blt $rt, $rs, pcoff16"); //N
    gen.add("000111.. ...00000 ........ ..000000", "bgtz $rs, pcoff16");
    
    // the remainder are switched by opcode (top six bits)
    gen.add(8, 26, 6,               "addi $rt, $rs, imm16s");
    gen.add(8, 26, 6,               "add $rt, $rs, imm16s");       // NICE
    gen.add(9, 26, 6,               "addiu $rt, $rs, imm16s");
    gen.add(9, 26, 6,               "addu $rt, $rs, imm16s");      // NICE
    gen.add(8, 26, 6,               "subi $rt, $rs, negimm16s");   // NICE
    gen.add(8, 26, 6,               "sub $rt, $rs, negimm16s");    // NICE
    gen.add(9, 26, 6,               "subiu $rt, $rs, negimm16s");  // NICE
    gen.add(9, 26, 6,               "subu $rt, $rs, negimm16s");   // NICE
    gen.add(12, 26, 6,              "andi $rt, $rs, imm16s");
    gen.add(12, 26, 6,              "and $rt, $rs, imm16s");       // NICE
    gen.add(13, 26, 6,              "ori $rt, $rs, imm16s");
    gen.add(13, 26, 6,              "or $rt, $rs, imm16s");        // NICE
    gen.add(14, 26, 6,              "xori $rt, $rs, imm16s");
    gen.add(14, 26, 6,              "xor $rt, $rs, imm16s");       // NICE 
    gen.add(15, 26, 6,              "lui $rt, imm16s($rs)");
    gen.add(16, 26, 6,              "mfc0 $rt, $rd");

    gen.add(33, 26, 6,              "lh $rt, imm16s($rs)");
    gen.add(34, 26, 6,              "lwl $rt, imm16s($rs)");
    gen.add(35, 26, 6,              "lw $rt, imm16s($rs)");
    gen.add(36, 26, 6,              "lbu $rt, imm16s($rs)");
    gen.add(37, 26, 6,              "lhu $rt, imm16s($rs)");
    gen.add(38, 26, 6,              "lwr $rt, imm16s($rs)");
    
    gen.add(40, 26, 6,              "sb $rt, imm16s($rs)");
    gen.add(41, 26, 6,              "sh $rt, imm16s($rs)");
    gen.add(42, 26, 6,              "swl $rt, imm16s($rs)");
    gen.add(43, 26, 6,              "sw $rt, imm16s($rs)");
    gen.add(46, 26, 6,              "swr $rt, imm16s($rs)");
    
    // NICE sd/ld: rsrc and rsrc+1 to ADDR
    // NICE ulh, ulhu, ulw
    // SLE, SGT, etc?
    
    gen.add(10, 26, 6,              "slti $rt, $rs, imm16s");
    gen.add(11, 26, 6,              "sltiu $rt, $rs, imm16u");
    
    
    gen.add(0, 0, 32, "noop");
    
    // for disassembly only
    gen.add(
        new CompoundRule(
            new TokRule(".word"),
            new ImmRule(32, Format::DisplayHex, ImmRule::Unsigned, 0, 32)));
    
    return gen.extractRoot();
}
