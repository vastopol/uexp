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
#ifndef MACHINEGT16_H
#define MACHINEGT16_H

#include "ui/simview.h"
#include "arch/arch.h"
#include "model/machine.h"

/**
 * Machine implementation for LC-2200.
 */
class MachineGT16
 : public Machine
{
 public:
    MachineGT16(SimView* view);
    ~MachineGT16() { }
    
 public:
    GWord memWord(GAddr addr) const
     { return mMem[addr]; }
    GInstr memInstr(GAddr addr) const
     { return mMem[addr]; }
    GByte memByte(GAddr addr) const
     { return mMem[addr]; }
    void setMemWord(GAddr addr, GWord value)
     { mMem[addr] = value; }
    void setMemInstr(GAddr addr, GInstr value)
     { mMem[addr] = value; }
    void setMemByte(GAddr addr, GByte value)
     { mMem[addr] = value; }

    GReg reg(int id) const
     { return mReg[id]; }
    void setReg(int id, GReg value)
     { mReg[id] = value; }
    
    GAddr pc() const
     { return mPC; }
    void setPC(GAddr value)
     { mPC = value; }
    
    Machine* clone() const
     { return new MachineGT16(*this); }
    
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
 
 private:
    ui16 mMem[65536];
    ui16 mReg[16];
    ui16 mPC;
    int mHalted;
};

#endif
