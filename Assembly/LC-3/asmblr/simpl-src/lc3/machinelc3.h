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
#ifndef MACHINELC3_H
#define MACHINELC3_H

#include "lc3.h"
#include "model/machine.h"
#include "model/model.h"
#include "archlc3.h"

/**
 * Machine implementation for LC-3.
 */
class MachineLC3
 : public Machine
{
 public:
    static lc_state& getState(Machine& machine)
     { return static_cast<MachineLC3&>(machine).st; }
    static const lc_state& getState(const Machine& machine)
     { return static_cast<const MachineLC3&>(machine).st; }
    
 private:
    static int lc_input_func(void *obj);
    static int lc_output_func(void *obj, int c);
    static void lc_printerr_func(void *obj, const char *msg);
 
 public:
    MachineLC3(SimView* inView);
    MachineLC3(const MachineLC3& other);
    virtual ~MachineLC3()
     { lc_destroy(&st); }
    
    MAKE_ASSIGNOP(MachineLC3);
    
    GWord memWord(GAddr addr) const
     { return st.mem[addr]; }
    GInstr memInstr(GAddr addr) const
     { return st.mem[addr]; }
    GByte memByte(GAddr addr) const
     { return st.mem[addr]; }
    void setMemWord(GAddr addr, GWord value)
     { st.mem[addr] = value; }
    void setMemInstr(GAddr addr, GInstr value)
     { st.mem[addr] = value; }
    void setMemByte(GAddr addr, GByte value)
     { st.mem[addr] = value; }
    
    GReg reg(int id) const
     { return st.reg[id]; }
    void setReg(int id, GReg value)
     { st.reg[id] = value; }
    
    GAddr pc() const
     { return st.pc; }
    void setPC(GAddr value)
     { st.pc = value; }
    
    Machine* clone() const
     { return new MachineLC3(*this); }
    
    void reinit();
    void fastRun()
     { lc_run(&st, 999999999); } // TODO: 999999999 or INT_MAX
    void runOne()
     { lc_run_one(&st); }
    bool enteringSubroutine(GAddr* pAddr) const;
    bool isProperlyHalted() const
     { return st.mem[st.pc] == 0xf025; }
    bool isHalted() const
     { return st.halted; }
    void setHalted(bool halted)
     { st.halted = halted; }

    void attachSymTable(const SymTable* table)
     { st.symt = table; }

    bool runMode(int flag) const;
    void setRunMode(int flag, bool on);
    
 private:
    struct lc_state st;
};

#endif
