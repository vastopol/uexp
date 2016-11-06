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

#ifndef ARCH_H
#define ARCH_H

#include "common.h"
#include "sizes.h"
#include "locid.h"
#include "format/format.h"
#include "model/symbol.h"

class Machine;
class SimView;
class FileFormat;
class AsmFormat;

class AsmRule;

/**
 * Interface for an architecture definition factory.
 *
 * An "architecture" is primarily a factory to make all the related
 * components to define an architecture in terms of a simulator.  Of
 * all base classes relating to architecture abstraction, this is the
 * core.
 *
 * In an architecture, the following are defined:
 *
 *  1. Sizes of the fundamental data types.
 *
 *  2. Formatter-parser object used to enter-in and display data from
 *  the user interface perspective.
 *
 *  3. Name of the architecture and its registers, and the layout used
 *  to display the registers for the GUI simulator.
 *
 *  4. Assembler to use.
 *
 *  5. Simulation and display options available.
 *
 *  6. Stack pointer register.
 *
 *  7. Miscellaneous other things.
 *
 */
class Arch
{
 public:
    /**
     * Get all available architectures.
     */
    static const Array<const Arch*>& availableArchs()
     { return *mAvailableArchs; }
    
    /**
     * Find an architecture by name.
     */
    static const Arch* findArch(const char* name);
    
 private:
    /** All registered architectures. */
    static Array<const Arch*>* mAvailableArchs;
    
 public:
    /**
     * Get the string name of the architecture.
     */
    const char* name() const
     { return mName; }
    
    /**
     * Get the relevant format object for the particular architecture.
     * @return the Format object to format fields for the architecture
     */
    const Format& format() const
     { return *mFormat; }
    /**
     * Get the name of a particular register.
     * @param regNum - the numerical ID of the register
     * @return the register's name
     */
    const char *regName(int regNum) const
     { return mRegTable.getname(regNum); }
    /**
     * Find the int number of a particular register.
     * @param str - the string of the name
     * @return the register name
     */
    int lookupReg(const char *str) const
     { return mRegTable.lookup(str); }
    
    /**
     * Create a machine instance of this architecture.
     *
     * @param view - the view to attach the machine to
     * @return a newly allocated machine
     */
    virtual Machine* makeMachine(SimView* view) const = 0;
    
    /**
     * Get the register number of the stack pointer.
     * @return the numerical ID of the stack pointer
     */
    int stackPointerNum() const
     { return mStackPointerNum; }
    /**
     * Get the location identifier of the stack pointer.
     * @return the stack pointer identifier
     */
    LocID stackPointer() const
     { return LocID(LocID::Reg, mStackPointerNum); }
    
    /**
     * Get the available simulation options for the machine.
     *
     * See Machine::runMode(int).
     *
     * @return the simulation options
     */
    const Array<Option>& runOptions() const
     { return mRunOptions; }
    
    /**
     * Add the default symbols to a particular symbol table.
     * @param table - the symbol table
     */
    virtual void addDefaultSymbols(SymTable* table) const
     { }
    
    /**
     * Get an assembler for this architecture.
     */
    virtual AsmFormat* makeAssembler(SymTable* table) const;
    
    /**
     * Get the assembly rule.
     */
    const AsmRule& asmRule() const
     { return *mAsmRule; }
    
    /**
     * Is this particular token reserved?
     */
    bool isTokenReserved(const char* str) const
     { return mReservedTokens.lookup(str) >= 0; }
    
    /**
     * Get the default extension for object files for this architecture.
     */
    const char* objExtension() const
     { return mObjExtension; }
    
    //--- Methods from Sizes ---
    
    /** Returns the number of bits in a byte. */
    int byteBits() const { return mSizes.byteBits(); }
    
    /** Returns the number of bytes in a word. */
    int wordBytes() const { return mSizes.wordBytes(); }
    /** Returns the number of bits in a word. */
    int wordBits() const { return mSizes.wordBits(); }
    /** Returns a mask for masking words from generic values. */
    GVal wordMask() const { return mSizes.wordMask(); }
    
    /** Returns the number of bytes in an instruction. */
    int instrBytes() const { return mSizes.instrBytes(); }
    /** Returns the number of bits in an executable instruction. */
    int instrBits() const { return mSizes.instrBits(); }
    /** Returns a mask for masking instructions from generic values. */
    GVal instrMask() const { return mSizes.instrMask(); }
    
    /** Returns the number of bytes in a register. */
    int regBytes() const { return mSizes.regBytes(); }
    /** Returns the number of bits in a register. */
    int regBits() const { return mSizes.regBits(); }
    /** Returns a mask for masking register values. */
    GReg regMask() const { return mSizes.regMask(); }
    
    /** Returns the number of bits used to address memory. */
    int addrBits() const { return mSizes.addrBits(); }
    /** Returns a mask for masking valid addresses. */
    GAddr addrMask() const { return mSizes.addrMask(); }
    /** Get the last address in the address space */
    GAddr lastAddress() const { return mSizes.addrMask(); }
    
    /**
     * Returns the number of bits for a particular location type.
     *
     * For instance, if locType were LocID::MemInstr, this would return
     * instrBits().
     *
     * @param locType - the location type
     */
    int bitsOf(LocID::Type locType) const { return mSizes.bitsOf(locType); }
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
     { return mSizes.bytesOf(locType); }
    /**
     * Returns the number of bits for a particular location.
     * This refers simply to the bitsOf of the parciular location type.
     */
    int bitsOf(const LocID& loc) const
     { return mSizes.bitsOf(loc); }
    /**
     * Returns the number of bytes of a particular location.
     * This simly refers to the bytesOf for the location's type.
     */
    int bytesOf(const LocID& loc) const
     { return mSizes.bytesOf(loc); }

    
 protected:
    /**
     * Construct the base Arch class.
     *
     * The register table and run options list are initialized to empty,
     * and the other three paramaters are initialized as specified.
     *
     * This automatically registers the architecture in the global list
     * of architectures.
     *
     * @param inSizes - the sizes for the architecture
     * @param inFormat - format for the architecture
     * @param inStackPointerNum - the stack pointer register number
     */
    Arch(const char* inName, const Sizes& inSizes, Format* inFormat,
        int inStackPointerNum, const char *inObjExtension = ".obj")
     : mSizes(inSizes)
     , mFormat(inFormat)
     , mRegTable(false)
     , mStackPointerNum(inStackPointerNum)
     , mName(inName)
     , mObjExtension(inObjExtension)
     , mReservedTokens(false)
     , mAsmRule(0)
     { registerInList(); }
    virtual ~Arch()
     { }
    
    void setAsmRule(AsmRule* inAsmRule);
    
 private:
    /**
     * Register this architecture in the list of architectures.
     */
    void registerInList() const;
    
 protected:
    /**
     * The sizes of various fields for the architecture.
     */
    Sizes mSizes;
    /**
     * The relevant format object for the architecture.
     */
    Ref<Format> mFormat;
    /**
     * The register symbol lookup table.
     */
    SymTable mRegTable;
    /**
     * The number of the stack pointer.
     */
    int mStackPointerNum;
    /**
     * Simulation options available.
     */
    Array<Option> mRunOptions;
    /**
     * Name of the architecture.
     */
    const char* mName;
    /**
     * Default extension for .obj files.
     */
    const char* mObjExtension;
 private:
    /**
     * Reserved tokens.
     */
    SymTable mReservedTokens;
    /**
     * Assembly rule.
     */
    AsmRule* mAsmRule;
};

#endif
