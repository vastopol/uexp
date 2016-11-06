#ifndef MACHINE_H
#define MACHINE_H

#include "ui/simview.h"
#include "arch/arch.h"

/**
 * The Machine class provides general abstraction for a single instance of a
 * machine of a particular architecture.  For instance, one LC3Machine would
 * contain 8 registers, plus the program coutner and flags register,
 * and all 65536 memory locations.  It will expose the generic methods for
 * getting and seetting memory locations of the three different types,
 * bytes words and instructions (which are all 16 bits in LC-3).
 *
 * In the design of the Machine class, a few things were made apparent:
 *
 *  1. It is hard to base the simulator on code that is written using global
 *  variables.  Perhaps, though, the concept of an "active machine" and
 *  a requirement to swap machines, or a simple disabling of save/restore
 *  state, might allow these to be plugged in.
 *
 *  2. The state of a machine and the actions of executing instructions
 *  are lumped into one.
 *
 */
class Machine
{
 public:
    /** Virtual destructor */
    virtual ~Machine()
     { }
    
 public:
    /**
     * Get the value at a particular location.
     *
     * @param identifier - a specifier for any location on the machine
     * @return the value of the particular location
     */
    GVal get(const LocID& identifier) const;
    /**
     * Set the value of a particular location.
     *
     * @param identifier - a specifier for any location on the machine
     * @param value - the value to set it to
     */
    SuccessT set(const LocID& identifier, GVal value);
    
    /**
     * Get the architecture associated with this machine.
     *
     * @return the relevant architecture
     */
    const Arch& arch() const
     { return *mArch; }
    
    /**
     * Get the formatter associated with this machine.
     *
     * @return the associated formatter
     */
    const Format& formatter() const
     { return mArch->format(); }
    
    // DEMETER
    /**
     * Get the available simulation options for the machine.
     *
     * See Machine::runMode(int).
     *
     * @return the simulation options
     */
    const Array<Option>& runOptions() const
     { return mArch->runOptions(); }
    
    /**
     * Get the name of this machine's architecture.
     */
    const char* archName() const
     { return mArch->name(); }
    
    /** Get the LocID of the stack pointer. */
    LocID stackPointerLocation() const
     { return LocID(LocID::Reg, mArch->stackPointerNum()); }
    
    /** Modulo-wraps an address to be within range. */
    GAddr wrapAddr(GAddr addr) const { return addr & mArch->addrMask(); }
    /** Get the last address of the machine. */
    GAddr lastAddress() const { return mArch->addrMask(); }
    /**
     * Returns the number of bits for a particular location type.
     *
     * For instance, if locType were LocID::MemInstr, this would return
     * instrBits().
     *
     * @param locType - the location type
     */
    int bitsOf(LocID::Type locType) const { return mArch->bitsOf(locType); }
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
     { return mArch->bytesOf(locType); }
    /**
     * Returns the number of bits for a particular location.
     * This refers simply to the bitsOf of the parciular location type.
     */
    int bitsOf(const LocID& loc) const
     { return mArch->bitsOf(loc); }
    /**
     * Returns the number of bytes of a particular location.
     * This simly refers to the bytesOf for the location's type.
     */
    int bytesOf(const LocID& loc) const
     { return mArch->bytesOf(loc); }
    
    //---------------------------------------------------------------------
    
    /**
     * Get a word of memory at the spcified address.
     * @param addr - the address
     * @return the value of that word
     */
    virtual GWord memWord(GAddr addr) const = 0;
    /**
     * Get an instruction of memory at the spcified address.
     * @param addr - the address of the instruction
     * @return the value of that instruction
     */
    virtual GInstr memInstr(GAddr addr) const = 0;
    /**
     * Get a byte of memory at the specified address.
     * @param addr - the address
     * @return the value of that "byte" (note: bytes might not be 8-bit)
     */
    virtual GByte memByte(GAddr addr) const = 0;
    /**
     * Set an architecture-specified word at the particular address to the
     * particular value.
     *
     * @param addr - the address
     * @param value - the value to set it to
     */
    virtual void setMemWord(GAddr addr, GWord value) = 0;
    /**
     * Set an architecture-specified instruction at the particular address
     * to the particular value.
     *
     * @param addr - the address
     * @param value - the value to set it to
     */
    virtual void setMemInstr(GAddr addr, GInstr value) = 0;
    /**
     * Set an architecture-specified byte at the particular address
     * to the particular value.
     *
     * @param addr - the address
     * @param value - the value to set it to
     */
    virtual void setMemByte(GAddr addr, GByte value) = 0;
    
    
    /**
     * Get the value of a particular register.
     */
    virtual GReg reg(int id) const = 0;
    /**
     * Set the value of a particular register.
     */
    virtual void setReg(int id, GReg value) = 0;
    
    /**
     * Get the value of the program counter.
     */
    virtual GAddr pc() const = 0;
    /**
     * Change the program counter.
     */
    virtual void setPC(GAddr value) = 0;
    
    /**
     * Make a clone of this Machine for save and restore.
     *
     * This should typically be a simple "return new MyClass(*this)".
     *
     * @return a freshly allocated clone of this Machine
     */
    virtual Machine* clone() const = 0;
    
    //---------------------------------------------------------------------
    
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
    virtual bool isProperlyHalted() const = 0;
    /**
     * Is the machine halted?
     *
     * This is used to determine when the model should stop running more
     * instructions.
     *
     * @return true iff the machine is halted
     */
    virtual bool isHalted() const = 0;
    /**
     * Set whether the machine is halted.
     *
     * This is a slight hack, but the intention of this method is to
     * allow an external entity to prevent the model from running, or
     * to somehow allow the machine to re-start.
     *
     * This implies two things:
     *
     *   1. The fastRun function must stop if setHalted(true) is called by
     *   another thread.
     *
     *   2. If the machine halts as with a halt or an invalid instruction,
     *   make sure the program counter does not pass that instruction; the
     *   model has the luxury to turn off the halted flag and try to run the
     *   machine again, and it would not be good to continue executing.
     *
     * @param halted - whether the machine shall be halted
     */
    virtual void setHalted(bool halted) = 0;
    
    /**
     * Randomize the machine.
     * This is virtual only in case some memory addresses are reserved.
     */
    virtual void randomize();
    /**
     * Reinitialize the machine.
     */
    virtual void reinit() = 0;
    
    /**
     * Run one single instruction.
     */
    virtual void runOne() = 0;
    /**
     * Run instructions until a halt is encountered.
     * The fastRun method *must* terminate if setHalted(true) is called
     * in another thread.
     */
    virtual void fastRun() = 0;
    
    /**
     * Check to see if the machine will enter a subroutine in the next
     * instruction -- important for call traces and skipping routines.
     *
     * @param pAddr - where to place the address; the value is changed only
     *        if this method returns true
     * @return true if entering a subroutine
     */
    virtual bool enteringSubroutine(GAddr* pAddr = 0) const = 0;
    
    /**
     * Get the state of a particular simulation or run mode.
     *
     * An example of simulation mode is the LC-3's option to enable or
     * disable the default trap mechanism in favor of simulated traps.
     *
     * To get a list of valid flags for the architecture, look at
     * Arch::runOptions().
     *
     * @param flag - the flag to query
     * @return whether the flag is set
     */
    virtual bool runMode(int flag) const
     { return 0; }
    /**
     * Set a particular simulation flag.
     *
     * @param flag - the flag to query
     */
    virtual void setRunMode(int flag, bool on)
     { }
    
    /**
     * Set the external view for this model.
     *
     * @param newView - The view to use.  It will not be freed.
     */
    void setView(SimView* newView)
     { mView = newView; }
    
    /**
     * A machine may want a symbol table because this is wrapping a "smart"
     * machine.
     */
    virtual void attachSymTable(const SymTable* table)
     { }
    
    //---------------------------------------------------------------------
    
 protected:
    /**
     * Construct a Machine given a view and the corresponding architecture.
     *
     * @param inView - the view to attach to
     * @param arch - the architecture to attach to
     */
    Machine(SimView* inView, const Arch* arch)
     : mView(inView), mArch(arch)
     { }
    /**
     * Copy-construct this machine.
     */
    Machine(const Machine& other)
     : mView(other.mView), mArch(other.mArch)
     { }
    
 protected:
    /**
     * This is the view that the model is attached to.
     */
    SimView* mView;
    /**
     * 
     */
    const Arch* mArch;
};

#endif
