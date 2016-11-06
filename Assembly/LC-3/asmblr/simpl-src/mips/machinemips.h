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
#ifndef MACHINEMIPS_H
#define MACHINEMIPS_H

#include "ui/simview.h"
#include "arch/arch.h"
#include "model/machine.h"
#include "model/bigmemory.h"

/**
 * Machine implementation for Mips.
 */
class MachineMips
 : public Machine
{
 public:
    MachineMips(SimView* view);
    ~MachineMips() { }
    
    typedef ui32 Word;
    typedef ui32 Instr;
    typedef ui8 Byte;
        
    enum {
        IST_J      = 2,
        IST_JAL    = 3,
        IST_BEQ    = 4,
        IST_BNE    = 5,
        IST_BLE    = 6,
        IST_BGT    = 7,
        IST_ADDI   = 8,
        IST_ADDIU  = 9,
        IST_SLTI   = 10,
        IST_SLTIU  = 11,
        IST_ANDI   = 12,
        IST_ORI    = 13,
        IST_XORI   = 14,
        IST_LUI    = 15,
        IST_MFC0   = 16,
        IST_LW     = 35,
        IST_LBU    = 37,
        IST_SB     = 41,
        IST_SW     = 43
    };

 private:
    enum {
        HI = 32,
        LO = 33
    };
    
 public:
    GWord memWord(GAddr addr) const
     { return mMemory.nat32(addr); }
    GInstr memInstr(GAddr addr) const
     { return mMemory.nat32(addr); }
    GByte memByte(GAddr addr) const
     { return mMemory.octet(addr); }
    void setMemWord(GAddr addr, GWord value)
     { mMemory.setNat32(addr, value); }
    void setMemInstr(GAddr addr, GInstr value)
     { mMemory.setNat32(addr, value); }
    void setMemByte(GAddr addr, GByte value)
     { mMemory.setOctet(addr, value); }

    GReg reg(int id) const
     { return mReg[id]; }
    void setReg(int id, GReg value)
     { mReg[id] = value; }
    
    GAddr pc() const
     { return mPC; }
    void setPC(GAddr value)
     { mPC = value; }
    
    Machine* clone() const
     { return new MachineMips(*this); }
    
    //---------------------------------------------------------------------
    
    bool isProperlyHalted() const;
    bool isHalted() const
     { return (mHalted) != 0; }
    void setHalted(bool halted = true)
     { mHalted = halted; }
    
    void reinit();
    
    void runOne();
    void fastRun();
    
    bool enteringSubroutine(GAddr* addr = 0) const;
    
    bool runMode(int flag) const
     { return 0; }
    void setRunMode(int flag, bool on)
     { }
 
 protected:
    ui32 wordAt(GAddr addr) const
     { return mMemory.nat32(addr); }
    ui8 byteAt(GAddr addr) const
     { return mMemory.octet(addr); }
    void setWordAt(GAddr addr, ui32 value)
     { return mMemory.setNat32(addr, value); }
    void setByteAt(GAddr addr, ui8 value)
     { return mMemory.setOctet(addr, value); }
 
 private:
    BigMemory mMemory;
    ui32 mPC;
    ui32 mReg[34];
    int mHalted;
};

#endif
