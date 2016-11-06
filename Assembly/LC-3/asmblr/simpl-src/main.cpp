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

#include "model/model.h"
#include "uistrap.h"

const char *arg_text[] =
{
    "Options:",
    " -r         initiate running as soon as all files are loaded",
    " -f         fast run mode, but doesn't allow debugging (ignores -v)",
    " -v         be very verbose about instructions",
    " -q         only run the program; no other output",
    " -l limit   stop after executing <limit> instructions",
    /*" -t         use the LC-3's true trap mechanism",*/
    " -B addr    blackbox an address (symbols and hex are accepted)",
    "",
    "To debug test.obj:",
    "  simp test.obj",
    "To run test.obj:",
    "  simp -f test.obj",
    NULL
};

int main(int argc, char *argv[])
{
    Ref<SimUI> sim;
    Ref<Model> model;
    SuccessT suc = SuccessPass;
    const char* archname = ARCHNAME;
    const Arch* arch = 0;
    int run = 0;
    int filteredArgCount;
    
    // Note: UICLASS and ARCHCLASS are #defines
    sim = new UICLASS();
    arch = Arch::findArch(archname);
    model = new Model(arch->makeMachine(sim), sim);
    sim->setModel(model);
    
    filteredArgCount = 1;
    
    // Loop through parameters, keeping all unrecognized ones.
    for (int argnum = 1; argnum < argc; argnum++)
    {
        char *s = argv[argnum];
        
        if (s[0] == '-')
        {
            const char *p = argnum+1 < argc ? argv[argnum+1] : "";
            
            switch(s[1])
            {
             case 'a':
                // TODO(gboyer): This is kind of a hack -- it kills the old
                // model completely, and creates a new one.
                arch = Arch::findArch(p);
                if (!arch)
                {
                    fprintf(stderr, "Unknown architecture: '%s'\n", p);
                    return 1;
                }
                model = new Model(arch->makeMachine(sim), sim);
                sim->setModel(model);
                p = 0;
                break;
             case 'q': /* quiet */
                sim->setVerbosityLevel(SimUI::Silent);
                break;
             case 'v': /* verbose */
                sim->setVerbosityLevel(SimUI::Verbose);
                break;
             case 'r':
                run = 1;
                break;
             case 'f':
                run = 2;
                sim->setVerbosityLevel(SimUI::Silent);
                break;
             case 'l':
                model->setLimit(atoi(p));
                p = 0;
                break;
             
             case 'B':
             {
                GVal addr;
                if (model->parse(
                    p, LocID::PC, &addr) == SuccessFail)
                {
                    sim->err("Invalid address to blackbox (-B)");
                    suc = SuccessFail;
                }
                else
                {
                    model->setBlack(addr, true);
                }
                p = NULL;
                break;
             }
             case '-':
                if (s[1] == 'h')
                {
                    suc = SuccessFail;
                    break;
                }
                
                // fallthrough
             default:
                argv[filteredArgCount] = s;
                filteredArgCount++;
            }
            
            if (!p)
            {
                argnum++; /* eat argument */
            }
        }
        else
        {
            if (model->load(s) == SuccessFail)
            {
                suc = SuccessFail;
            }
        }
    }
    
    if (suc != SuccessPass)
    {
        fprintf(stderr, "\n\n");
        fprintf(stderr, "Usage: %s [options and files to load]\n", argv[0]);
        for (const char **s = arg_text; *s; s++)
            fprintf(stderr, "%s\n", *s);
    }
    else
    {
        argc = filteredArgCount;
        argv[filteredArgCount] = NULL;
        
        sim->parseArgs(argc, argv);
        // TODO: For now, interactive mode only.
        sim->install();
        
        if (run != 0)
            sim->note("Running, starting with PC=x%04X", model->pc());
        
        if (run == 1)
        {
            model->doRun();
        }
        else if (run == 2)
        {
            model->doFastRun();
        }
        
        if (!run || !model->isProperlyHalted())
            sim->loop();
        
        sim->uninstall();
    }
    
    model = 0;
    sim = 0;
    
    return 0;
}
