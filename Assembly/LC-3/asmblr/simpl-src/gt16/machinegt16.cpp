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
#include "machinegt16.h"

MachineGT16::MachineGT16(SimView* inView)
 : Machine(inView, &ArchGT16::instance())
{
    reinit();
}

void MachineGT16::runOne()
{
    unsigned char pc = mPC;
    ui16 ir = memInstr(mPC);
    int rx = Bits::extract(ir, 9, 4);
    int ry = Bits::extract(ir, 5, 4);
    int rz = Bits::extract(ir, 1, 4);
    ui16 imm = Bits::extractSigned(ir, 0, 5);
    
    pc += 1;
    mPC = pc;
    
    switch (Bits::extract(ir, 13, 3))
    {
     case 0: mReg[rx] = mReg[ry] + mReg[rz]; break;
     case 1: mReg[rx] = ~(mReg[ry] & mReg[rz]); break;
     case 2: mReg[rx] = mReg[ry] + imm; break;
     case 3: mReg[rx] = mMem[(mReg[ry] + imm) & 0xffff]; break;
     case 4: mMem[(mReg[ry] + imm) & 0xffff] = mReg[rx]; break;
     case 5: if (mReg[rx] == mReg[ry]) mPC += imm; break;
     case 6:
      mReg[ry] = mPC;
      mPC = mReg[rx];
      break;
     case 7:
      mPC -= 1; // don't increment PC when halting
      mHalted = 1;
      break;
    }
    
    mReg[0] = 0;
}

void MachineGT16::fastRun()
{
    while (!mHalted) runOne();
}

bool MachineGT16::isProperlyHalted() const
{
    return (memInstr(mPC) == 0xe000);
}

void MachineGT16::reinit()	
{
    memset(mMem, 0, sizeof(mMem));
    memset(mReg, 0, sizeof(mReg));
    mPC = 0;
    mHalted = 0;
}

bool MachineGT16::enteringSubroutine(GAddr* addr) const
{
    ui16 ir = memInstr(mPC);
    
    // We're in a JALR...
    if (Bits::extract(ir, 13, 3) == 6)
    {
        int rx = Bits::extract(ir, 9, 4);
        int ry = Bits::extract(ir, 5, 4);
        
        // Make sure we're not just saving PC, or simply jumping.
        if (rx != ry && ry != 0) {
          *addr = mReg[rx];
          return true;
        } else {
          return false;
        }
    }
    else
    {
        return false;
    }
}
