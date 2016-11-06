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

#ifndef SIMUI_H_INCLUDED
#define SIMUI_H_INCLUDED

#include "common.h"
#include "model/model.h"
#include "format/format.h"
#include "simview.h"

/**
 * Abstract simulator user interface.
 *
 * Whereas SimView is the model's look at the user, SimUI represents the
 * "center of the UI" -- the glue between application start-up and the
 * simulation experience.
 *
 * In words, the main() function will basically tell the SimUI to parse
 * command line arguments, install any global devices necessary, and then
 * enter the main loop of the program, without knowing anything about how
 * the UI is composed.  In addition, a SimUI subclass will handle and
 * dispatch events that are sent by the lower layers back to the user.
 */
class SimUI : public SimView
{
    DISALLOW_COPY(SimUI);
 public:
    
    /** Construct the UI */
    SimUI();
    /** Virtual destructor */
    virtual ~SimUI()
     { }
    
    /**
     * Parse global command line options.
     * After calling this, the argc and argv will be modified to remove any
     * options tha have been parsed.
     */
    virtual void parseArgs(int &UNUSED(argc), char **UNUSED(argv))
     { }
    /**
     * This initializes the UI after the model has been set, to get it
     * ready.
     *
     * For TextUI, for instance, this will install a handler on SIGINT
     * to handle ^C during execution, and it will make sure the CommandUI
     * is initialized.
     */
    virtual void install()
     { }
    /**
     * Run the event loop.
     */
    virtual void loop()
     { }
    /**
     * Uninstall any global handlers.
     */
    virtual void uninstall()
     { }
    
    /** Get the internal model of the U/I. */
    Model& model() const
     { return *mModel; }
    void setModel(Model* inModel)
     { mModel = inModel; }
    
 protected:
    /** The simulator model */
    Model* mModel;
};

#endif /*SIMUI_H*/
