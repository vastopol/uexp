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

#ifndef LOCID_H
#define LOCID_H

/**
 * Unique identifier of a particular value on the model.
 *
 * This class is perhaps one of the most important for modelling the
 * architecture and makes a lot of consistency in formatting possible by
 * specifying in a central location the standard types.
 *
 * There are three kinds of memory addresses, depending on the size of the
 * dereference.  Registers also are numbered by an identifier.  However,
 * the program counter refers to the actual program counter (or any value
 * that serves as a memory address, in some situations).
 */
class LocID
{
 public:
    /**
     * The type of the location being referred to.
     */
    enum Type
    {
        Invalid = -1, ///< Invalid location
        MemWord,      ///< A word of memory
        MemByte,      ///< A byte of memory (arch-specific byte)
        MemInstr,     ///< An instruction in memory
        Reg,          ///< A register
        PC            ///< The program counter, or a memory address
    };
    
 public:
    /**
     * Create an unspecified location ID.
     */
    LocID()
     { }
    
    /**
     * Construct a location ID with the specified two fields.
     */
    LocID(Type inType, long inID = -1)
     : mType(inType)
     , mID(inID)
     { }
    
 public:
    /**
     * Get the type of the LocID.
     */
    Type type() const { return mType; }
    /**
     * Get the numerical identifier of the location if necessary.
     */
    long id() const { return mID; }
    /**
     * Is this particular location in memory?
     * @return whether the location refers to memory
     */
    bool isInMemory() const { return mType >= MemWord && mType <= MemInstr; }
    
    /**
     * Is this a particular register?
     * @param testID - the register to check this with.
     */
    bool isReg(long testID) const
     { return mType == Reg && mID == testID; }
    
 private:
    /** The type of location. */
    Type mType;
    /** The ID number of the location (given the ty0e). */
    long mID;
};

#endif
