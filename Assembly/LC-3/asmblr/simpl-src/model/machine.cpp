#include "machine.h"
#include "stdlib.h"

GVal Machine::get(const LocID& location) const
{
    switch(location.type())
    {
     case LocID::MemWord:
        return memWord(location.id());
     case LocID::MemInstr:
        return memInstr(location.id());
     case LocID::MemByte:
        return memByte(location.id());
     case LocID::Reg:
        return reg(location.id());
     case LocID::PC:
        return pc();
     default:
        return 0;
    }
}

SuccessT Machine::set(const LocID& location, GVal value)
{
    switch(location.type())
    {
     case LocID::MemWord:
        setMemWord(location.id(), value);
        break;
     case LocID::MemInstr:
        setMemInstr(location.id(), value);
        break;
     case LocID::MemByte:
        setMemByte(location.id(), value);
        break;
     case LocID::Reg:
        setReg(location.id(), value);
        break;
     case LocID::PC:
        setPC(value);
        break;
     default:
        return SuccessFail;
    }
    return SuccessPass;
}

void Machine::randomize()
{
    GVal addr = lastAddress();
    
    while (addr--)
    {
        int rv = rand();
        
        setMemByte(addr, rv);
    }
}

