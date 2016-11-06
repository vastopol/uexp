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

#ifndef OPTION_H
#define OPTION_H

/**
 * An Option models a generic way of expressing an option.
 *
 * The use of Option is so that classes like Arch can export a list of
 * options available for a particular platform.  This way, the user
 * can interact and respond with these options, without having the core
 * layer know ahead of time what options to use.
 */
class Option
{
 public:
    /**
     * Create an option.
     */
    Option(int inVal, const char* inName,
        char accel = 0, const char* help = 0, bool isDefault = false)
     : mVal(inVal), mName(inName), mAccel(accel), mHelp(help),
       mIsDefault(isDefault)
     { }
 
 public:
    /**
     * The associated integer identifier or flag of this option.
     */
    int val() const
     { return mVal; }
    /**
     * The name to be displayed for this option.
     */
    const char* name() const
     { return mName; }
    /**
     * Accelerator key to use for this option.
     */
    char accel() const
     { return mAccel; }
    /**
     * A short help string explaining the purpose of this option.
     */
    const char* help() const
     { return mHelp; }
    /**
     * Determine whether this option should be enabled by default.
     */
    bool isDefault() const
     { return mIsDefault; }
 
 private:
    int mVal;
    const char* mName;
    char mAccel;
    const char* mHelp;
    bool mIsDefault;
};

#endif
