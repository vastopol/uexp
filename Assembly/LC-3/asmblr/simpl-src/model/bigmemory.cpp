#include "bigmemory.h"

BigMemory::BigMemory()
{
    for (int i1 = 0; i1 < SizeLevel1; i1++)
    {
        mOctets[i1] = 0;
    }
    memset(mZero, 0, sizeof(mZero));
}

BigMemory::BigMemory(const BigMemory& other)
{
    for (int i1 = 0; i1 < SizeLevel1; i1++)
    {
        ui8 const * const * oldL1 = other.mOctets[i1];
        ui8** myL1 = 0;
        
        if (oldL1)
        {
            myL1 = Memory::alloc<ui8*>(SizeLevel2);
            
            for (int i2 = 0; i2 < SizeLevel2; i2++)
            {
                ui8 const * oldL2 = oldL1[i2];
                ui8* myL2 = 0;
                
                if (oldL2)
                {
                    myL2 = Memory::dup(oldL2, SizeLevel3);
                }
                
                myL1[i2] = myL2;
            }
        }
        
        mOctets[i1] = myL1;
    }
}

BigMemory::~BigMemory()
{
    for (int i1 = 0; i1 < SizeLevel1; i1++)
    {
        ui8** l1 = mOctets[i1];
        if (l1)
        {
            for (int i2 = 0; i2 < SizeLevel2; i2++)
            {
                ui8* l2 = l1[i2];
                if (l2)
                {
                    Memory::free(l2);
                }
            }
            Memory::free(l1);
        }
    }
}

const ui8* BigMemory::ptrAt(GAddr addr) const
{
    const ui8* retval = mZero;
    ui8 const * const * l1 = mOctets[l1index(addr)];

    if (l1)
    {
        ui8 const * l2 = l1[l2index(addr)];
        
        if (l2)
        {
            retval = &l2[l3index(addr)];
        }
    }

    return retval;
}

ui8* BigMemory::ptrAt(GAddr addr)
{
    ui8** l1 = mOctets[l1index(addr)];
    
    // Must be careful about threading -- if someone calls the const version
    // of ptrAt at the same time in another thread,
    // the other ptrAt call must *not* segfault.
    
    if (!l1)
    {
        l1 = Memory::allocZeroed<ui8*>(SizeLevel2);
        mOctets[l1index(addr)] = l1;
    }
    
    ui8* l2 = l1[l2index(addr)];
    
    if (!l2)
    {
        l2 = Memory::allocZeroed<ui8>(SizeLevel3);
        l1[l2index(addr)] = l2;
    }
    
    return &l2[l3index(addr)];
}
