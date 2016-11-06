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

#include "machinelc3.h"
#include "ui/simview.h"
#include "lc3os.h"

MachineLC3::MachineLC3(SimView* inView)
 : Machine(inView, &ArchLC3::instance())
{
    lc_init(&st);
    
    st.write_func = lc_output_func;
    this->st.write_obj = this;
    st.read_func = lc_input_func;
    this->st.read_obj = this;
    st.printerr_func = lc_printerr_func;
    this->st.printerr_obj = this;
}

MachineLC3::MachineLC3(const MachineLC3& other)
 : Machine(other)
{
    lc_copy(&st, &other.st);
    st.write_obj = this;
    st.read_obj = this;
    st.printerr_obj = this;
}

int MachineLC3::lc_input_func(void *obj)
{
    MachineLC3 *model = reinterpret_cast<MachineLC3*>(obj);
    return model->mView->readChar();
}

int MachineLC3::lc_output_func(void *obj, int c)
{
    MachineLC3 *model = reinterpret_cast<MachineLC3*>(obj);
    return model->mView->writeChar(c);
}

void MachineLC3::lc_printerr_func(void *obj, const char *msg)
{
    MachineLC3 *model = reinterpret_cast<MachineLC3*>(obj);
    model->mView->msg(SimView::Warning, msg);
}

void MachineLC3::reinit()
{
    //lc_reinit(&st); TODO: This should be moved
    memset(st.mem, 0, LC_MEM * sizeof(*st.mem));
    memset(st.reg, 0, 8 * sizeof(*st.reg));
    st.reg[1] = 0x7FFF;
    st.reg[7] = 0x0490;
    st.pc = 0x3000; /* for lack of anywhere better to put it */
    st.flag = LC_FL_Z;
    if (st.real_traps) lc_load_os(&st);
    // TODO: Load the OS, etc.
}

bool MachineLC3::enteringSubroutine(GAddr* pAddr) const
{
    GAddr pc = this->pc();
    GInstr ist = this->memInstr(pc);
    GAddr result = 0;
    bool found;
    
    found = true;
    
    if (ist >> 12 == IST_JSR)
    {
        if (ist&(1<<11))
        {
            result = (LC_GET_S(ist, 0, 11) + pc + 1) & 0xffff;
        }
        else
        {
            result = this->reg(LC_GET_U(ist, 6, 3));
        }
    }
    else if (false) //(ist >> 12 == IST_TRAP && isRaw()) TODO: Traps
    {
        result = ist&0xff;
    }
    else
    {
        found = false;
    }
    
    if (found && pAddr)
    {
        *pAddr = result;
    }
    
    return found;
}

bool MachineLC3::runMode(int flag) const
{
    bool state;
    
    switch(flag)
    {
      case 1:
        state = (st.real_traps != 0);
        break;
      default:
        mView->err("BUG in Simulator! Invalid run mode selected %d.", flag);
        state = false;
    }
    
    return state;
}

void MachineLC3::setRunMode(int flag, bool on)
{
    switch(flag)
    {
      case 1:
        if (on)
        {
            st.real_traps = 1;
            lc_load_os(&st);
        }
        else
        {
            st.real_traps = 0;
        }
        break;
      default:
        mView->err("BUG in Simulator! Invalid run mode selected %d.", flag);
    }
}

