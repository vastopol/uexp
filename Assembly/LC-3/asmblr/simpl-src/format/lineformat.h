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

#ifndef LINEFORMAT_H
#define LINEFORMAT_H

#include "fileformat.h"

/**
 * A subclass of FileFormat for formats that are read by reading first the
 * origin and then each successive data point, where no rewinding is
 * necessary.
 */
class LineFormat
 : public FileFormat
{
 public:
    virtual ~LineFormat()
     { }
    
    SuccessT read(FILE* f, CodeEmitter* emitter, Reporter* reporter) const;
    SuccessT write(FILE* f,
        const Machine& machine, GAddr origin, size_t length) const;
    
 public:
    /**
     * Write one line (virtual).
     *
     * This is called by write() to write a single line of a certain
     * number of bits.
     *
     * @param f - output stream
     * @param addr - the address being written (for annotation purposes)
     * @param val - the value to write
     * @param bits - the number of bits of the value to write
     * @return the success of the operation
     */
    virtual SuccessT writeOne(FILE* f, GAddr addr, GVal val, int bits) const = 0;
    /**
     * Read one line from the file.
     *
     * @param f - input stream
     * @param reporter - where error messages are sent
     * @param pVal - where to write the value read in
     * @param bits - number of bits to read in
     * @return 0 normal, -1 EOF, -2 invalid
     */
    virtual int readOne(FILE* f, Reporter* reporter,
        GVal* pVal, int bits) const = 0;

 protected:
    /**
     * Construct a LineFormat object.
     *
     * The line type is used for the read and write implemenations to
     * decide how many bytes at a time to write or read.
     *
     * @param lineType - the unit being read in for each line
     */
    LineFormat(LocID::Type lineType = LocID::MemInstr, bool useOrigin = true)
     : mLineType(lineType), mUseOrigin(useOrigin)
     { }
    
 private:
    /**
     * The type for the lines being read in.
     */
    LocID::Type mLineType;
    /**
     * Whether or not to output/input the origin (assume it to be 0).
     */
    bool mUseOrigin;
};

#endif
