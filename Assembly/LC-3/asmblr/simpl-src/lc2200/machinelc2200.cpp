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
#include "machinelc2200.h"

MachineLC2200::MachineLC2200(SimView* inView)
 : Machine(inView, &ArchLC2200::instance())
{
    reinit();
}

void MachineLC2200::runOne()
{
    unsigned char pc = mPC;
    ui16 ir = memInstr(mPC);
    int ra = Bits::extract(ir, 9, 4);
    int rb = Bits::extract(ir, 5, 4);
    int rc = Bits::extract(ir, 1, 4);
    ui16 imm = Bits::extractSigned(ir, 0, 5);
    
    pc += 2;
    mPC = pc;
    
    switch (Bits::extract(ir, 13, 3))
    {
     case 0:
      mReg[ra] = mReg[rb] + mReg[rc];
      break;
     case 1:
      mReg[ra] = ~(mReg[rb] & mReg[rc]);
      break;
     case 2:
      mReg[ra] = mReg[rb] + imm;
      break;
     case 3:
      mReg[ra] = mMem[(mReg[rb] + imm) & 0xff];
      break;
     case 4:
      mMem[(mReg[rb] + imm) & 0xff] = mReg[ra];
      break;
     case 5:
      if (mReg[ra] == mReg[rb]) mPC += (imm << 1);
      break;
     case 6:
      mReg[rb] = mPC; // NOTE: jalr order
      mPC = mReg[ra];
      break;
     case 7:
      mPC -= 2;
      mHalted = 1;
      break;
    }
    
    mReg[0] = 0;
}

void MachineLC2200::fastRun()
{
    while (!mHalted) runOne();
}

bool MachineLC2200::isProperlyHalted() const
{
    return (memInstr(mPC) == 0xe000);
}

void MachineLC2200::reinit()	
{
    memset(mMem, 0, sizeof(mMem));
    memset(mReg, 0, sizeof(mReg));
    mPC = 0;
    mHalted = 0;
}

bool MachineLC2200::enteringSubroutine(GAddr* addr) const
{
    ui16 ir = memInstr(mPC);
    
    if (Bits::extract(ir, 13, 3) == 6 && Bits::extract(ir, 5, 4) != 0)
    {
        *addr = mReg[Bits::extract(ir, 9, 4)];
        return true;
    }
    else
    {
        return false;
    }
}
