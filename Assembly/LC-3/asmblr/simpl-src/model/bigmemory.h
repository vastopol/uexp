#ifndef BIGMEMORY_H
#define BIGMEMORY_H

#include "common.h"

class BigMemory
{
 public:
    enum {
        ShiftLevel1 = 16,
        SizeLevel1 = 65536,
        ShiftLevel2 = 8,
        SizeLevel2 = 256,
        ShiftLevel3 = 0,
        SizeLevel3 = 256
    };
    
 public:
    BigMemory();
    ~BigMemory();
    BigMemory(const BigMemory& other);
    MAKE_ASSIGNOP(BigMemory);
    
    void reinit()
     { this->~BigMemory(); new(this) BigMemory(); }
    
    ui8 octet(GAddr addr) const
     { return *ptrAt(addr); }
    void setOctet(GAddr addr, ui8 value)
     { *ptrAt(addr) = value; }

    ui16 nat16(GAddr addr) const
     { return *reinterpret_cast<const ui16*>(ptrAt(addr)); }
    void setNat16(GAddr addr, ui16 value)
     { *reinterpret_cast<ui16*>(ptrAt(addr)) = value; }

    ui32 nat32(GAddr addr) const
     { return *reinterpret_cast<const ui32*>(ptrAt(addr)); }
    void setNat32(GAddr addr, ui32 value)
     { *reinterpret_cast<ui32*>(ptrAt(addr)) = value; }
    
    ui8& operator [] (GAddr addr)
     { return *ptrAt(addr); }
    ui8 operator [] (GAddr addr) const
     { return *ptrAt(addr); }
    
 private:
    unsigned l1index(GAddr addr) const
     { return addr >> ShiftLevel1; }
    unsigned l2index(GAddr addr) const
     { return (addr >> ShiftLevel2) & (SizeLevel2-1); }
    unsigned l3index(GAddr addr) const
     { return addr & (SizeLevel3-1); }
    const ui8* ptrAt(GAddr addr) const;
    ui8* ptrAt(GAddr addr);
    
 private:
    ui8 **mOctets[SizeLevel1];
    ui8 mZero[8];
};

#endif
