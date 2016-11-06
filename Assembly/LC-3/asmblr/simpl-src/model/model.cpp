/*
Copyright (c) 2005, Garrett Franklin Boyer
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

#include "model.h"
#include "ui/simui.h"
#include "format/numformat.h"
#include "format/objformat.h"
#include "asm/asmformat.h"

#include <sys/types.h>
#ifndef _WIN32
#include <sys/wait.h>
#endif
#include <unistd.h>

Model::Model(Machine* inMachine, SimView* view)
 : mUI(view)
 , mMachine(inMachine)
 , mSavedMachine(mMachine->clone())
 , mSymTable()
 , mTotalCount(0)
 , mLocalCount(0)
 , mLimit(0)
 , mUpdateCount(0)
 , mIsRunning(0)
{
    mMachine->attachSymTable(&mSymTable);
    mSavedMachine->attachSymTable(&mSymTable);
    clearStack();
    mMachine->arch().addDefaultSymbols(&mSymTable);

    const Array<Option>* pOptions = &runOptions();

    for (size_t i = 0; i < (*pOptions).size(); i++)
    {
        setRunMode((*pOptions)[i].val(), (*pOptions)[i].isDefault());
    }

}

Model::~Model()
{
}

void Model::ud()
{
    ++mUpdateCount;
    mUI->handleModelUpdated();
}

void Model::clearStack()
{
    mStack.resize(1);
    mStack[0] = StackEntry(0, 0, 0);
    //not sending update here because this is an internal routine
    //and we might not have a GUI...
}

void Model::saveMachine()
{
    mSavedMachine = mMachine->clone();
}

void Model::restoreMachine()
{
    mMachine = mSavedMachine->clone();
}

void Model::reinit()
{
    clearStack();
    mSymTable.clear();
    mMachine->reinit();
    mTotalCount = 0;
    ud();
}

Model::RunResultType Model::doRunOne(bool verbose)
{
    GAddr pc = nextPC();
    bool doBlackbox;
    GAddr subaddr;
    
    doBlackbox = false;
    
    if (mMachine->enteringSubroutine(&subaddr))
    {
        if (mStack.size() < 1000)
        {
            // avoid eating up mem/cpu with stack traces
            mStack.push_back(StackEntry(GAddr(subaddr), pc,
                mMachine->get(stackPointerLocation())));
        }
        doBlackbox = isBlack(subaddr);
    }
    
    if (doBlackbox)
    {
        int i = 0;
        const char *s;
        
        if (verbose && (s = getname(subaddr)))
            mUI->note("Executing blackboxed routine %s...", s);
        else if (verbose)
            mUI->note("Executing blackboxed subroutine...");
        do {
            mMachine->runOne();
            i++;
        } while (!mMachine->isHalted() && mMachine->pc() != pc);
        if (verbose)
            mUI->note("Executed %d instructions in blackboxed routine.", i);
        this->mTotalCount += i;
        this->mLocalCount += i;
    }
    else
    {
        mMachine->runOne();
        this->mTotalCount++;
        this->mLocalCount++;
    }
    
    pc = mMachine->pc();
    
    if (mStack.back().retAddr == pc && mStack.size() > 1)
        mStack.pop_back();
    
    if (mMachine->isHalted()) return SIM_RUN_HALT;
    
    if (isBreak(pc))
    {
        mUI->note("Encountered breakpoint.");
        return SIM_RUN_BREAK;
    }
    
    mUpdateCount++;
    mUI->handleInstructionExecuted();
    
    // FIXME TODO: print flood
    
    if (this->mLimit && this->mLocalCount > this->mLimit)
    {
        mUI->err("Limit of %d instructions exceeded for this run.", this->mLimit);
        return SIM_RUN_LIMIT;
    }
    
    return SIM_RUN_NORMAL;
}

void Model::doStep(bool skipSub)
{
    GAddr subaddr;
    
    halt(false);
    
    this->mLocalCount = 0;
    
    if (skipSub && mMachine->enteringSubroutine(&subaddr))
    {
        /* Use local instruction count; that way, you can 'next' after a
         * "mLimit exceeded" to see what was going on.
         */
        GAddr pc = nextPC();
        unsigned size = mStack.size();
        int result;
        const char *subname;
        
        if ((subname = mSymTable.getname(subaddr)))
            mUI->note("Executing subroutine %s.", subname);
        else
            mUI->note("Executing subroutine at x%04X.", subaddr);
        
        while ((result = doRunOne()) == SIM_RUN_NORMAL
               && mStack.size() != size)
         { }
        
        if (result == SIM_RUN_LIMIT)
        {
            setBreak(pc, true);
            mUI->note("For your convenience, I have placed breakpoint at the next");
            mUI->note("instruction within the calling subroutine, x%04X.", pc);
            mUI->note("I recommend that you turn off verbose and type 'run'.");
        }
        
        if (this->mLocalCount > 1)
            mUI->note("Skipped %d instructions in subroutine.", this->mLocalCount);
    }
    else
    {
        doRunOne(true);
    }
    
    mUI->handleInstructionSetExecuted();
    ud();
}

void Model::doFinish(GAddr addr)
{        
    bool found = false;
    
    for (unsigned i = mStack.size(); i-- > 0;)
    {
        if (mStack[i].subAddr == addr)
        {
            found = true;
            halt(0);
            mUI->note("Running until return of x%04X.", mStack[i].subAddr);
            int v;
            while ((v = doRunOne()) == SIM_RUN_NORMAL && mStack.size() > i)
             { }
            mUI->handleInstructionSetExecuted();
            ud();
            break;
        }
    }
    
    if (!found)
    {
        mUI->err("Subroutine at specified address is not currently executing.");
    }
    
    ud();
}

void Model::doRun(GAddr until)
{
    this->mLocalCount = 0;
    halt(false);
    
    mIsRunning = true;
    
    while (doRunOne() == SIM_RUN_NORMAL && mMachine->pc() != until)
    {
    }
    
    mIsRunning = false;
    
    mUI->handleInstructionSetExecuted();
    ud();
}

SuccessT Model::loadSymbols(FILE *symfile)
{
    if (mSymTable.read(symfile) < 0)
    {
        mUI->err("Error reading symbol file - labels will not be shown.");
        return SuccessFail;
    }
    else
    {
        ud();
        mUI->note("Labels loaded successfully.");
        return SuccessPass;
    }
}

SuccessT Model::saveFile(FILE* outfile, const FileFormat& format,
    GAddr origin, size_t length) const
{
    if (!length) length = min(size_t(65535), size_t(lastAddress())) + 1;
    return format.write(outfile, *mMachine, origin, length);
}

SuccessT Model::saveFile(const char* fname,
    GAddr origin, size_t length) const
{
    Ref<FileFormat> format;
    OpenFile outfile(fname, "wb");
    SuccessT suc = SuccessPass;
    
    if (!outfile)
    {
        mUI->err("Could not save file '%s'.", fname);
        suc = SuccessFail;
    }
    else
    {
        Ref<FileFormat> format;
        format = detectFormat(fname, mMachine->arch(),
            const_cast<SymTable*>(&mSymTable));
        
        if (!format)
        {
            mUI->warn("Assuming object output format for name '%s'.", fname);
            suc = SuccessWarn;
        }
        
        suc = min(suc, saveFile(outfile, *format, origin, length));
    }
    
    return suc;
}

SuccessT Model::load(FILE* infile, const FileFormat& format)
{
    SuccessT result;
    CodeEmitter emitter(mMachine);
    result = format.read(infile, &emitter, mUI);
    ud();
    return result;
}

FileFormat* Model::detectFormat(const String& strname,
    const Arch& arch, SymTable* symTable)
{
    FileFormat* format = 0;
    
    if (strname.endsWith(".lc"))
    {
        format = new ObjFormat(false);
    }
    else if (strname.endsWith(".obj"))
    {
        format = new ObjFormat();
    }
    else if (strname.endsWith(".bin"))
    {
        format = new NumFormat(1);
    }
    else if (strname.endsWith(".hex"))
    {
        format = new NumFormat(4);
    }
    else if (strname.endsWith(".asm") || strname.endsWith(".s"))
    {
        format = arch.makeAssembler(symTable);
    }
    
    return format;
}

SuccessT Model::load(const char *fname, const FileFormat* format)
{
    OpenFile infile(fname, "rb");
    SuccessT retval;
    
    mUI->note("Loading %s...", fname);
    
    if (!infile)
    {
        mUI->err("Could not open file %s.", fname);
        retval = SuccessFail;
    }
    else
    {
        String strname = String(fname);
        FileFormat* newformat = 0;
        SymTable tempTable;
        
        if (!format)
        {
            newformat = detectFormat(strname, mMachine->arch(), &tempTable);
            format = newformat;
        }
        
        if (format)
        {
            const char* dotPos = strname.findr('.');
            
            retval = load(infile, *format);
            
            if (retval == SuccessFail)
            {
                mUI->err("One or more errors found while loading the file.");
            }
            else if (!dynamic_cast<const AsmFormat*>(format) && dotPos)
            {
                String symname = strname(0, dotPos) + "sym";
                OpenFile symfile(symname.c_str(), "rb");
                
                if (symfile)
                {
                    mUI->note("Loading labels from %s.", symname.c_str());
                    loadSymbols(symfile);
                }
                else
                {
                    mUI->warn("Could not find symbol file (%s); labels not displayed.",
                        symname.c_str());
                }
            }
            
            if (newformat) delete newformat;
        }
        else
        {
            mUI->err("Could not determine file format of \"%s\".",
                strname.c_str());
            retval = SuccessFail;
        }
        
        if (retval == SuccessPass)
        {
            mLastFileLoaded = strname;
            mSymTable.addAll(tempTable);
        }
    }
    
    return retval;
}
/*
bool Model::hasStackFrameAt(GAddr addr, StackEntry* outEntry)
{
    Array<Stack>::iterator it = mStack.begin();
    
    for (; it != mStack.end(); it++)
    {
        if (it->sp == addr)
        {
            if (outEntry) *outEntry = *it;
            return true;
        }
    }
    
    return false;
}

bool Model::hasReturnAddressAt(GAddr addr, StackEntry* outEntry)
{
    Array<Stack>::iterator it = mStack.begin();
    
    for (; it != mStack.end(); it++)
    {
        if (it->retAddr == addr)
        {
            if (outEntry) *outEntry = *it;
            return true;
        }
    }
    
    return false;
}

bool Model::hasRunningSubroutineAt(GAddr addr, StackEntry* outEntry)
{
    Array<Stack>::iterator it = mStack.begin();
    
    for (; it != mStack.end(); it++)
    {
        if (it->retAddr == addr)
        {
            if (outEntry) *outEntry = *it;
            return true;
        }
    }
    
    return false;
}
*/

