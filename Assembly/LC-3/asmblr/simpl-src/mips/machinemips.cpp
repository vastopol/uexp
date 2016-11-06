
#include "common.h"
#include "machinemips.h"
#include "archmips.h"

MachineMips::MachineMips(SimView* inView)
 : Machine(inView, &ArchMips::instance())
{
    reinit();
}

void MachineMips::runOne()
{
    ui32 *r = mReg;
    ui32 ir = wordAt(mPC);
    int opcode = ir>>26;
    int s = (ir>>21)&31;
    int t = (ir>>16)&31;
    int d = (ir>>11)&31;
    ui32 imm16u = ui16(ir);
    i32 imm16s = i16(ir);
    int shamt = (ir >> 6) & 31;
    #define ADDR (imm16s+r[t])
    
    // TODO: Support the exception-raising exceptions
    // (The priority is very low -- this simulator is geared for new students
    // debugging, where exceptions themselves are a low priority.)
    
    mPC += 4;
    
    switch(opcode)
    {
     case 0:
        switch(ir & 0x3f)
        {
         case 0x00: r[d] = r[s] << shamt; break;
         case 0x03: // right shift arithmetic
             if (r[s] & 0x80000000)
             {
                 r[d] = (r[s] >> shamt) | ~(0xffffffff >> shamt);
                 break;
             }
             /* fallthrough */
         case 0x02: r[d] = r[s] >> shamt; break;
         case 0x04: r[d] = r[s] << r[t]; break;
         case 0x06: r[d] = r[s] >> r[t]; break;
         case 0x08: mPC = r[s]; break;
         case 0x10: r[d] = r[HI]; break;
         case 0x11: r[HI] = r[d]; break;
         case 0x12: r[d] = r[LO]; break;
         case 0x13: r[LO] = r[d]; break;
         case 0x18: // mult
         case 0x19: // multu
         {
            unsigned long long val = r[s];
            val *= r[t];
            r[HI] = (val >> 32);
            r[LO] = (val & 0xffffffff);
            break;
         }
         case 0x1A: // div
         case 0x1B: // divu
         {
            if (r[t] == 0)
            {
                mHalted = 1;
                break;
            }
            r[HI] = r[s] / r[t];
            r[LO] = r[s] % r[t];
            break;
         }
         case 0x20: r[d] = r[s] + r[t]; break;
         case 0x21: r[d] = r[s] + r[t]; break;
         case 0x22: r[d] = r[s] - r[t]; break;
         case 0x23: r[d] = r[s] - r[t]; break;
         case 0x24: r[d] = r[s] & r[t]; break;
         case 0x25: r[d] = r[s] | r[t]; break;
         case 0x2A: r[d] = (i32(r[s]) < i32(r[t])); break;
         case 0x2B: r[d] = (r[s] < r[t]); break;
        }
        break;
     case 1:
        switch((ir >> 16) & 31)
        {
         case 0x00:
           if (i32(r[s]) < i32(0)) mPC += imm16s*4;
           break;
         case 0x01:
           if (i32(r[s]) >= i32(0)) mPC += imm16s*4;
           break;
         case 0x10:
           if (i32(r[s]) < i32(0)) { mReg[31] = mPC; mPC += imm16s*4; }
           break;
         case 0x11:
           if (i32(r[s]) >= i32(0)) { mReg[31] = mPC; mPC += imm16s*4; }
           break;
        }
        break;
     case IST_J:     mPC = Bits::truncate(ir, 26) * 4; break;
     case IST_JAL:   mReg[31] = mPC; mPC = Bits::truncate(ir, 26) * 4; break;
     case IST_BEQ:   if (r[s] == r[t]) mPC += imm16s*4; break;
     case IST_BNE:   if (r[s] != r[t]) mPC += imm16s*4; break;
     case IST_BLE:   if (i32(r[s]) <= i32(r[t])) mPC += imm16s*4; break;
     case IST_BGT:   if (i32(r[s]) > i32(r[t])) mPC += imm16s*4; break;
     case IST_ADDI:  r[t] = r[s] + imm16s; break;
     case IST_ADDIU: r[t] = r[s] + imm16s; break;
     case IST_SLTI:  r[t] = (i32(r[s]) < imm16s); break;
     case IST_SLTIU: r[t] = (r[s] < imm16u); break;
     case IST_ANDI:  r[t] = r[s] & imm16s; break;
     case IST_ORI:   r[t] = r[s] | imm16s; break;
     case IST_XORI:  r[t] = r[s] ^ imm16s; break;
     case IST_LUI:   r[t] = imm16u << 16; break;
     case IST_MFC0:  printf("Not Implemented\n"); mHalted = 1; break;
     case IST_LW:    r[t] = wordAt(ADDR); break;
     case IST_LBU:   r[t] = byteAt(ADDR); break;
     case IST_SB:    setByteAt(ADDR, r[t]); break;
     case IST_SW:    setWordAt(ADDR, r[t]); break;
     default:
        // Send a notice?
        break;
    }
    
    r[0] = 0;
}

void MachineMips::fastRun()
{
    while (!mHalted) runOne();
}

bool MachineMips::isProperlyHalted() const
{
    //return (memInstr(mPC) == 0xe000);
    return true;
}

void MachineMips::reinit()	
{
    mMemory.reinit();
    memset(mReg, 0, sizeof(mReg));
    mPC = 0;
    mHalted = 0;
}

bool MachineMips::enteringSubroutine(GAddr* addr) const
{
    /*ui16 ir = memInstr(mPC);
    
    if (Bits::extract(ir, 13, 3) == 6 && Bits::extract(ir, 5, 4) != 0)
    {
        *addr = mReg[Bits::extract(ir, 9, 4)];
        return true;
    }
    else
    {
        return false;
    }*/
    // TODO(gboyer): fix this!
    return false;
}


