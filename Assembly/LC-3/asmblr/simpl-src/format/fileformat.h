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

#ifndef FILEFORMAT_H
#define FILEFORMAT_H

#include "model/machine.h"
#include "codeemitter.h"

/**
 * Class to model a file format for simulator input and output.
 */
class FileFormat
{
 public:
    /** Default constructor */
    FileFormat()
     { }
    /** Default destructor */
    virtual ~FileFormat()
     { }
    
    /**
     * Read data from the stream f into the machine.
     *
     * @param f - the stream to read from
     * @param emitter - code emitter used to emit the code to
     * @param reporter - where to report error messages to
     * @return the success of reading the entire file
     */
    virtual SuccessT read(FILE* f,
        CodeEmitter* emitter, Reporter* reporter) const = 0;
    
    /**
     * Write data from the state back into a file.
     * @param f - output stream
     * @param machine - machine to get the state from
     * @param origin - the origin to start writing
     * @param length - number of bytes to write
     * @return the success of reading the entire file
     */
    virtual SuccessT write(FILE* f,
        const Machine& machine, GAddr origin, size_t length) const = 0;
};

#endif
