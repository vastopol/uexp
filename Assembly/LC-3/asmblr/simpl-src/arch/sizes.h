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

#ifndef SIZES_H
#define SIZES_H

#include "common.h"
#include "locid.h"

/**
 * Structure defining various important sizes for an architecture.
 *
 * In this specification, a byte is as specified by the basic model -
 * a byte is the smallest unit of memory that is addressible.
 *
 * An architecture specifies the number of bits in a byte, or the
 * smallest addressible memory unit.  From this, the following are
 * defined:
 *
 * - Word bytes: The size of the "natural" memory unit
 *
 * - Instruction bytes: The size of a default instruction.  Does not
 * yet handle architectures with variable-byte instructions.
 *
 * - Register bytes: The (maximum) size of a register.
 *
 * - Address bits: Bits comprising an address.  This is a bit-only
 * quantity; some architectures might have 12-bit memory addresses
 * but 8-bit words, etc.
 *
 */
class Sizes
{
 public:
    Sizes()
     { }
    
    /**
     * Construct a Sizes object with the specified values.
     *
     * @param byteBits - number of bits in a byte
     * @param addrBits - number of bits for a memory address
     * @param wordBytes - number of bytes in a word
     * @param instrBytes - number of bytes in an instruction
     * @param regBytes - number of bytes in a register
     */
    Sizes(int byteBits, int addrBits, int wordBytes, int instrBytes,
        int regBytes)
     : mByteBits(byteBits)
     , mWordBytes(wordBytes)
     , mWordBits(wordBytes * byteBits)
     , mInstrBytes(instrBytes)
     , mInstrBits(instrBytes * byteBits)
     , mRegBytes(regBytes)
     , mRegBits(regBytes * byteBits)
     , mAddrBits(addrBits)
    {
    }
    
 public:
    /** Returns the number of bits in a byte. */
    int byteBits() const { return mByteBits; }
    
    /** Returns the number of bytes in a word. */
    int wordBytes() const { return mWordBytes; }
    /** Returns the number of bits in a word. */
    int wordBits() const { return mWordBits; }
    /** Returns a mask for masking words from generic values. */
    GVal wordMask() const { return Bits::mask<GVal>(mWordBits); }
    
    /** Returns the number of bytes in an instruction. */
    int instrBytes() const { return mInstrBytes; }
    /** Returns the number of bits in an executable instruction. */
    int instrBits() const { return mInstrBits; }
    /** Returns a mask for masking instructions from generic values. */
    GVal instrMask() const { return Bits::mask<GVal>(mInstrBits); }
    
    /** Returns the number of bytes in a register. */
    int regBytes() const { return mRegBytes; }
    /** Returns the number of bits in a register. */
    int regBits() const { return mRegBits; }
    /** Returns a mask for masking register values. */
    GReg regMask() const { return Bits::mask<GReg>(mRegBits); }
    
    /** Returns the number of bits used to address memory. */
    int addrBits() const { return mAddrBits; }
    /** Returns a mask for masking valid addresses. */
    GAddr addrMask() const { return Bits::mask<GAddr>(mAddrBits); }
    
    /**
     * Returns the number of bits for a particular location type.
     *
     * For instance, if locType were LocID::MemInstr, this would return
     * instrBits().
     *
     * @param locType - the location type
     */
    int bitsOf(LocID::Type locType) const;
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
     { return bitsOf(locType) / mByteBits; }
    /**
     * Returns the number of bits for a particular location.
     * This refers simply to the bitsOf of the parciular location type.
     */
    int bitsOf(const LocID& loc) const
     { return bitsOf(loc.type()); }
    /**
     * Returns the number of bytes of a particular location.
     * This simly refers to the bytesOf for the location's type.
     */
    int bytesOf(const LocID& loc) const
     { return bytesOf(loc.type()); }
    
 private:
    int mByteBits;
    int mWordBytes;
    int mWordBits;
    int mInstrBytes;
    int mInstrBits;
    int mRegBytes;
    int mRegBits;
    int mAddrBits;
};


#endif
