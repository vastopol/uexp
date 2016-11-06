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
#ifndef CODEEMITTER_H
#define CODEEMITTER_H

#include "model/machine.h"
#include "arch/arch.h"

/**
 * A CodeEmitter is a utility used by FileFormat subclasses for writing data
 * into a machine.  It provides a "cursor" into a current address, methods
 * to write something to the end, and the ability to reposition.
 *
 * Currently, after the code generation is done, the minimum and maximum
 * addresses, or origin and length, can be extracted.  Later development
 * might include arbitrary intervals.
 */
class CodeEmitter
{
 public:
    /**
     * Create a CodeEmitter to write to this machine.
     */
    CodeEmitter(Machine* inMachine)
     : mMachine(inMachine)
     , mMinAddr(inMachine->lastAddress())
     , mMaxAddr(0)
     , mCurAddr(0)
     { }
    
    /**
     * Get the first address writtent o.
     * @return the first address written to
     */
    GAddr minAddr() const { return mMinAddr; }
    /**
     * Get the address write after last address written to.
     * @return the address right after the last written to
     */
    GAddr maxAddr() const { return mMaxAddr; }
    /**
     * Get the current position that will next be written to.
     * @return the current position that will next be written to
     */
    GAddr curAddr() const { return mCurAddr; }
    
    /**
     * Get the origin.
     * @return the origin
     */
    GAddr origin() const { return mMinAddr; }
    /**
     * Get the number of bytes past the origin written to.
     */
    size_t length() const
     { return (mMaxAddr > mMinAddr) ? mMaxAddr - mMinAddr : 0; }
    
    /**
     * Emit a line of code.
     * @param type - the type or size being writtten to
     * @param val - the value to write
     */
    void emitCode(LocID::Type type, GVal val);
    /**
     * Mark an entire interval as being written to.
     * @param firstAddr - the first address
     * @param onePastAddr - the last address
     */
    void setIntervalUsed(GAddr firstAddr, GAddr onePastAddr);
    /**
     * Move the current position up by times * bitsOf(type).
     * @param type - the unit to increment by
     * @param times - the number of units to increment by
     */
    void increment(LocID::Type type, int times = 1);
    /**
     * Determine if there is room left to increment by the given amount.
     * @param type - the unit wished to increment in units of
     * @param times - the number of units wished to increment by
     */
    bool canIncrement(LocID::Type type, int times = 1)
     { return (mMachine->bytesOf(type) * GAddr(times))
              <= (mMachine->lastAddress() - mCurAddr); }
    /**
     * Set the current address for insertion.
     * @param newAddr - the new current address
     */
    void setAddr(GAddr newAddr);
    
    /**
     * Get the number of bits of a location type.
     */
    int bitsOf(LocID::Type type) const
     { return mMachine->bitsOf(type); }
    /**
     * Get the number of bytes of a location type.
     */
    int bytesOf(LocID::Type type) const
     { return mMachine->bytesOf(type); }
    
 private:
    Machine* mMachine;
    GAddr mMinAddr;
    GAddr mMaxAddr;
    GAddr mCurAddr;
};

#endif
