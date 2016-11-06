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

#ifndef SIM_H_INCLUDED
#define SIM_H_INCLUDED

#include <deque>
#include <string>
#include "common.h"
#include "symbol.h"
#include "machine.h"
#include "bigmemory.h"
#include "format/fileformat.h"
#include "format/format.h"

class SimView;

/**
 * This class represents the model of the simulator in an MVC-like paradigm.
 *
 * The model communicates with the UI by informing it about certain events
 * (described in the UI interface) that are triggered by some of the
 * non-const methods.
 *
 * This model does several major things beyond a machine:
 *
 *  - Handles breakpoints
 *  - Allows blackboxing of functions
 *  - Step/next functionality, that respect blackboxed functions
 *  - Running until breakpoint, halt, or interrupt
 *  - Informing the UI every time something changes.
 *  - Keeping track of a call stack (it assumes that the code is playing
 *  fair and returning from one function at a time)
 *  - Saving/restoring machine state (ie forking)
 * 
 */
class Model
{
    DISALLOW_COPY(Model);

 public:
    /// Enumeration for simulation flags
    enum SimFlag {
        SimFlagBreak = 1, //< Address marked as breakpoint
        SimFlagBlack = 2, //< Address marked as blackboxed
    };

    /**
     * Stack entry structure.  This structure is used in order to 
     */
    struct StackEntry
    {
     public:
        StackEntry() { } /* for Vector */
        StackEntry(GAddr inSubAddr, GAddr inRetAddr, GAddr inSP)
         : subAddr(inSubAddr),
           retAddr(inRetAddr),
           sp(inSP)
         { }
        // DEM - Crufty Solution
        StackEntry(const Model& model, int i)
         : subAddr(model.mStack[i].subAddr)
         , retAddr(model.mStack[i].retAddr)
         , sp(model.mStack[i].sp)
         { }
        
     public:
        /** Address of the start of the subroutine */
        GAddr subAddr;
        /** Point of execution at which execution will resume when subroutine
         * has completed. */
        GAddr retAddr;
        /** Stack pointer at start of subroutine */
        GAddr sp;
    };
    
 protected:
    /**
     * Result of the doRunOne() routine.
     * TODO: Some of these may no longer be applicable.
     */
    enum RunResultType {
        SIM_RUN_NORMAL, SIM_RUN_HALT, SIM_RUN_BREAK, SIM_RUN_LIMIT,
        SIM_RUN_FLOOD
    };

    /** The stack is stored as an array of stack entries. */
    typedef Array<StackEntry> Stack;
    
 public:
    /**
     * Instantiate a simulator model.
     *
     * This uses a machine specified by the given architecture.
     *
     * The architecture and view must be provided.  Thus, in general, a
     * SimUI should be constructed prior to the Model.
     *
     * @param machine - the machine being used (will take exclusive control)
     * @param view - the external view to connect everything to
     */
    Model(Machine* inMachine, SimView* view);
    /**
     * Destroy the model (does not delete the UI or symbol table).
     */
    virtual ~Model();
 
 public:
    /**
     * Detect the format of the filename.
     *
     * @param strname - the string name of the file
     * @return a newly all
     */
    static FileFormat* detectFormat(const String& strname,
        const Arch& arch, SymTable* symTable);
    
 public:
    /** Get the current traced stack. */
    size_t stackSize() const
     { return mStack.size(); }
    
    /** Get the address of the innermost subroutine. */
    GAddr subroutineAddr() const
     { return mStack.back().subAddr; }
    
    /** Get any value from the machine */
    GVal get(const LocID& location) const
     { return mMachine->get(location); }
    /** Set anything and send an update */
    void set(const LocID& location, GVal val)
     { mMachine->set(location, val); ud(); }
    
    /** Get the program counter. */
    GAddr pc() const
     { return mMachine->pc(); }
    /** Get the program counter after the current. */
    GAddr nextPC() const
     { return mMachine->pc() + mMachine->bytesOf(LocID::MemInstr); }
    
    // TODO: STOPGAP; what is the real decision on verbosity?
    /**
     * Is this currently in the middle of running (as opposed to stepping)?
     */
    bool isRunning() const
     { return mIsRunning; }
    
    /** Is this address word-aligned? */
    bool isAligned(const LocID& location)
     { return (location.id() % bytesOf(location)) == 0; }
    /** Modulo-wraps an address to be within range. */
    GAddr wrapAddr(GAddr addr) const { return mMachine->wrapAddr(addr); }
    /** Get the last address of the machine. */
    GAddr lastAddress() const { return mMachine->lastAddress(); }
    /**
     * Returns the number of bits for a particular location type.
     *
     * For instance, if locType were LocID::MemInstr, this would return
     * instrBits().
     *
     * @param locType - the location type
     */
    int bitsOf(LocID::Type locType) const { return mMachine->bitsOf(locType); }
    /**
     * Returns the number of bytes of a particular location type.
     *
     * This returns simply the bitsOf the type divided by the number of
     * bytes in a bit.
     *
     * It is not recommended that you call this on LocID::PC unless
     * addrBits is divisible by byteBits.
     */
    int bytesOf(LocID::Type locType) const
     { return mMachine->bytesOf(locType); }
    /**
     * Returns the number of bits for a particular location.
     * This refers simply to the bitsOf of the parciular location type.
     */
    int bitsOf(const LocID& loc) const
     { return mMachine->bitsOf(loc); }
    /**
     * Returns the number of bytes of a particular location.
     * This simly refers to the bytesOf for the location's type.
     */
    int bytesOf(const LocID& loc) const
     { return mMachine->bytesOf(loc); }
    
    
    /**
     * Re-initialize the machine.
     */
    void reinit();
    
    /**
     * Randomize the machine.
     */
    void randomize()
     { mMachine->randomize(); ud(); }
    
    /** Halt the machine (or un-halt it) */
    void halt(bool halted = true)
     { mMachine->setHalted(halted); ud(); }
    /** Is the machine currently halted? */
    bool isHalted()
     { return mMachine->isHalted(); }
    
    /**
     * Is this machine "properly" halted?
     *
     * Typically, a machine might have several reasons for being halted, such
     * as invalid instruction exceptions of the like.  This method is used
     * to determine when, after simulation, whether the debugger should be
     * brought up or if the simulator should simply terminate.
     *
     * @return whether the simulator is "properly" halted
     */
    bool isProperlyHalted() const
     { return mMachine->isProperlyHalted(); }
    
    /**
     * Unique number to represent a memory address to use for update
     * checks: this will tell if ANYTHING changed.
     * @param addr - address to get unique value for
     * @return a value that will change if the memory or flags have changed
     *         at this address, or the PC has pointed to this address
     * TODO: Phase out.  New solution: Compute string but only update if
     * string varies.
     */
    long unique(GAddr addr) const
     { return mMachine->memWord(addr)^(mFlags[addr]<<16)^(mMachine->pc()==addr?(1<<24):0); }
    
    /**
     * Execute a step or next command.
     * This sends a single updateOnce and a single updateEnd event to the UI.
     * @param skipSub - set this to true if subroutines should not be followed
     */
    virtual void doStep(bool skipSub);
    /**
     * Run until PC = "until", keeping track of the stack and respecting
     * breakpoints.
     *
     * This will send updateOnce events to the UI after each instruction,
     * and updateEnd at the end of running.
     *
     * This will terminate upon a halt, breakpoint, or when PC=until.
     *
     * @param until - if PC reaches this address, running stops
     */
    virtual void doRun(GAddr until = GAddr(-1));
    /**
     * Finish the subroutine that begin at subaddr.
     * @param subaddr - the address of the subroutine to finish
     */
    virtual void doFinish(GAddr subaddr);
    
    /**
     * Do a fast run but do not keep track of debugging data.
     * Fast runs can be five or more times faster.
     */
    virtual void doFastRun()
     { mUI->warn("Performing fast run invalidates stack trace.");
       clearStack();
       mIsRunning = true;
       mMachine->fastRun();
       mIsRunning = false;
       ud(); }
    
    /**
     * Save the contents of the machine to a file, with a given
     * file format.
     * @param outfile - output stream
     * @param format - the format to write it in
     * @param origin - place to start writing
     * @param length - number of bytes to write
     * @return whether it was successful
     */
    virtual SuccessT saveFile(FILE* outfile, const FileFormat& format,
        GAddr origin = 0, size_t length = 0) const;
    /**
     * Save the contents of the machine to a file, automatically
     * opening the file and guessing the file format.
     * @param fname - file name
     * @param origin - place to start writing
     * @param length - number of bytes to write
     * @return whether it was successful
     */
    virtual SuccessT saveFile(const char* fname,
        GAddr origin = 0, size_t length = 0) const;
    
    /**
     * Load a file and set the program counter to origin.
     * @param infile - file to read
     * @param type - integer type
     * @return negative on error
     */
    virtual SuccessT load(FILE *infile, const FileFormat& format);
    /**
     * Load a file by filename, and if a .sym file exist load its symbols
     * too.
     * @param fname - file name to load "file.obj" "file.bin" "file.hex"
     * @param readfunc - read function to use (0 means auto-detect)
     * @return negative on error
     */
    virtual SuccessT load(const char *fname, const FileFormat* format = 0);
    /**
     * Load symbols from a .sym file.
     * @param symfile - file using the standard LC-3 symbol file format
     * @return negative on error
     */
    virtual SuccessT loadSymbols(FILE *symfile);
    
    /**
     * Set a flag to true or false.
     * @param addr - address to change flag for
     * @param flag - the flag to change
     * @param on - whether to set or clear the flag
     */
    void setSimFlag(GAddr addr, int flag, bool on)
     { if (on) mFlags[addr]|=flag; else mFlags[addr]&=~flag; ud(); }
    
    // DEM ?  Useful for other abstraction, but not pure OO.
    /**
     * Get whether a flag is set for a particular address.
     * @param addr - the address to check
     * @param flag - the flag, such as SimFlagBreak
     */
    bool getSimFlag(GAddr addr, int flag) const
     { return (mFlags[addr]&flag) != 0; }
    // DEM ?  Useful for other abstraction, but not pure OO.
    /**
     * Toggle a flag at an address.
     * @param addr - the address
     * @param flag - the flag, such as SimFlagBreak
     */
    void toggleSimFlag(GAddr addr, int flag)
     { mFlags[addr] ^= flag; ud(); }
    
    /** Is this address blackboxed? */
    bool isBlack(GAddr addr) const
     { return getSimFlag(addr, SimFlagBlack); }
    /** Blackbox this address */
    void setBlack(GAddr addr, bool on)
     { setSimFlag(addr, SimFlagBlack, on); ud(); }
    /** Toggle a blackbox */
    void toggleBlack(GAddr addr)
     { toggleSimFlag(addr, SimFlagBlack); }
    
    /** Is there a breakpoint? */
    bool isBreak(GAddr addr) const
     { return getSimFlag(addr, SimFlagBreak); }
    /** Set a breakpoint at this address */
    void setBreak(GAddr addr, bool on)
     { setSimFlag(addr, SimFlagBreak, on); ud(); }
    /** Toggle a breakpoint */
    void toggleBreak(GAddr addr)
     { toggleSimFlag(addr, SimFlagBreak); }
    
    /** Get a particular running or simulation mode */
    bool runMode(int flag) const
     { return mMachine->runMode(flag); }
    /** Change a particular running or simulation mode */
    void setRunMode(int flag, bool on)
     { mMachine->setRunMode(flag, on); ud(); }
    
    /**
     * Get total dynamic instruction count.
     * This includes instructions executed in any forks.
     */
    int totalCount()
     { return mTotalCount; }
    
    /** 
     * Restore machine state.
     */
    void restore()
     { mStack = mSavedStack; restoreMachine(); ud(); }
    /**
     * Save machine state.
     */
    void save()
     { saveMachine(); mSavedStack = mStack; ud(); }
    
    /**
     * Add a symbol name to an address.
     * @param s - symbol name
     * @param addr - address to add it to
     */
    void addSym(const char *s, GAddr addr)
     { mSymTable.add(s, addr); ud(); }
    /**
     * Get the symbol name for an address.
     */
    const char* getname(GAddr addr)
     { return mSymTable.getname(addr); }
    
    /**
     * Get the last file loaded.
     */
    const String& lastFileLoaded() const
     { return mLastFileLoaded; } 
    
    /**
     * Get the current update count.
     *
     * This can be used to, for example, avoid painting the same screen
     * twice.  For instance, the QtUI postpones screen updates until the
     * next event cycle; thus, several update events may be executed at
     * once, but only one is really necessary.
     *
     * @return the number of times the update signal has been sent
     */
    int updateCount() const
     { return mUpdateCount; }
    
    /**
     * Set dynamic instruction limit.
     */
    void setLimit(int newLimit)
     { mLimit = newLimit; }
    
    /** @see Format */
    String format(const LocID& location, GVal value, int flags = 0) const
     { return formatter().format(fctx(), location, value, flags); }
    /** @see Format */
    String format(const LocID& location) const
     { return formatter().format(fctx(), location, mMachine->get(location)); }
    /** @see Format */
    SuccessT parse(const char *str,
        const LocID& location, GVal* pVal) const
     { return formatter().parse(str, mUI, fctx(), location, pVal); }
    /** @see Format */
    LocID parseDest(const char* str) const
     { return formatter().parseDest(str, mUI, fctx()); }
    /** @see Format */
    String formatDest(const LocID& dest) const
     { return formatter().formatDest(fctx(), dest); }
    // TODO - Uniform formatting for printing/dumping lines
    /** @see Format */
    String formatLine(const LocID& location, int flags = 0) const
     { return formatter().formatLine(fctx(), location, flags); }
    /** @see Format */
    String formatState(int flags = 0) const
     { return formatter().formatState(fctx(), flags); }
    /** @see Format */
    String disassemble(GAddr addr, int flags = 0) const
     { return formatter().disassemble(fctx(), addr, flags); }
    /** @see Format */
    String formatInstrBits(GInstr instr, bool useColor = true) const
     { return formatter().formatInstrBits(instr, useColor); }
    // DEM - Rework dumpLine?
    /** @see Format */
    String dumpLine(GAddr addr, int flags = 0, GAddr highlightAddr = GAddr(-1)) const
     { return formatter().dumpLine(fctx(), addr, flags, highlightAddr); }
    /** @see Format */
    int calcDumpBytes() const
     { return formatter().calcDumpBytes(); }
    /** @see Format */
    const Array<Option>& formatOptions() const
     { return formatter().formatOptions(); }
    /** @see Format */
    const char *panelLayout() const
     { return formatter().panelLayout(); }
    
    const char* archName() const
     { return mMachine->archName(); }
    LocID stackPointerLocation() const
     { return mMachine->stackPointerLocation(); }
    
    // DEM - Very specific.
    /**
     * Get the available simulation options for the machine.
     *
     * See Machine::runMode(int).
     *
     * @return the simulation options
     */
    const Array<Option>& runOptions() const
     { return mMachine->runOptions(); }
    
 protected:
    /**
     * Get the architecture information.
     */
    const Arch& arch() const
     { return mMachine->arch(); }
    const Format& formatter() const
     { return mMachine->formatter(); }
    const Format::Context fctx() const
     { return Format::Context(mMachine, &mSymTable); }
    
    /// Save machine state
    virtual void saveMachine();
    /// Restore machine state
    virtual void restoreMachine();
    
    /**
     * Run a single instruction and return what happened.
     * This sends an updateOnce event to the UI.
     *
     * @param verbose whether to be verbose
     * @return the result of execution
     */
    virtual RunResultType doRunOne(bool verbose = false);
    /**
     * Clear the execution stack.
     */
    void clearStack();
    /**
     * Send an update signal.
     *
     * Additionally changes the update count so the UI can avoid updating
     * twice when only once update is necessary.
     */
    void ud();

 protected:
    /** Connection to the user interface; redundant with mMachine->mUI */
    SimView* mUI;
    
    /** The machine */
    Ref<Machine> mMachine;
    /** The saved machine */
    Ref<Machine> mSavedMachine;
    
    /** The symbol table */
    SymTable mSymTable;
    /** Flags set for each address, such as SimFlagBreak. */
    BigMemory mFlags; /* bitwise or of SimFlagBreak|SimFlagBlack */
    /** The current stack trace. */
    Stack mStack;
    /** The stack trace during previous trace. */
    Stack mSavedStack;
    
    /** Cumulative dynamic instruction count (includes forks) */
    int mTotalCount;
    /** Instruction count for current doStep or doRun */
    int mLocalCount;
    /** Local instruction limit (for flood purposes) */
    int mLimit;
    /** The current update count of the model */
    int mUpdateCount;
    
    /** The last file loaded. */
    String mLastFileLoaded;
    
    /** Whether the machine is currently running */
    bool mIsRunning;
};

#endif/*SIM_H_INCLUDED*/

