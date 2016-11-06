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

#include "asm/asmrule.h"
#include "asm/asmformat.h"
#include "model/machine.h"
#include "model/model.h"
#include "format/objformat.h"
#include "arch/arch.h"
#include "archstrap.h"

#ifdef NO_ASSEMBLER
An assembler main is not made for this architecture, because it is likely
that a user will shoot themselves in the foot by creating gigabyte-sized
output files.  Once there is a demand for an assembler, please rewrite this
file so it will handle assembling these kinds of architectures.

An example is MIPS - assembling might write into both the data and stack
sections.  If these sections are really far away, any output file would be
huge.
#endif

#ifndef OBJ_EXT
#define OBJ_EXT ".obj"
#endif

class AsmView
 : public SimView
{
    void msg(Reporter::MsgType type, const char *s)
    {
        fprintf(stderr, "%c| %s\n",
            type == Reporter::Note ? '.' :
            type == Reporter::Warning ? '!' : 'X',
            s);
    }
};

SuccessT readIn(const String& fname, const Arch& arch, CodeEmitter* emitter,
    SymTable* table, Reporter* reporter)
{
    SuccessT retval;
    OpenFile instream(fname.c_str(), "rb");
    
    if (!instream)
    {
        reporter->err("Could not open %s for reading.", fname.c_str());
        retval = SuccessFail;
    }
    else if (fname.endsWith(".sym"))
    {
        reporter->note("Reading in labels from %s...", fname.c_str());
        table->read(instream);
        retval = SuccessPass;
    }
    else
    {
        Ref<FileFormat> format(
            Model::detectFormat(fname, arch, table));
        
        reporter->note("Processing %s...", fname.c_str());
        
        if (!format)
        {
            reporter->warn(
                "Unrecognized extension on %s, assuming assembly.",
                fname.c_str());
            format = arch.makeAssembler(table);
        }
        
        retval = format->read(instream, emitter, reporter);
        
        if (retval != SuccessFail)
        {
            reporter->note(
                "Read %d bytes (%d bits each) at origin 0x%04X.",
                emitter->length(),
                emitter->bitsOf(LocID::MemByte),
                emitter->origin());
        }
    }
    
    return retval;
}

SuccessT writeOut(const String& fname,
    const String& basename, const Machine& machine,
    const SymTable& table, Reporter* reporter, GAddr minAddr, GAddr maxAddr)
{
    SuccessT retval = SuccessPass;
    OpenFile outstream(fname.c_str(), "wb");
    
    if (!outstream)
    {
        reporter->err("Could not open %s for writing.", fname.c_str());
        retval = SuccessFail;
    }
    else
    {
        Ref<FileFormat> format(
            Model::detectFormat(fname, machine.arch(),
                const_cast<SymTable*>(&table)));
        
        reporter->note("Writing to %s...", fname.c_str());
        
        if (!format)
        {
            reporter->err(
                "Unrecognized extension on %s, assuming object output.",
                fname.c_str());
            format = new ObjFormat();
            retval = min(retval, SuccessWarn);
        }
        
        if (maxAddr != minAddr)
        {
            retval = min(retval,
                format->write(outstream, machine, minAddr,
                    size_t(maxAddr - minAddr)));
        }
        
        if (table.head)
        {
            String symname = basename + ".sym";
            OpenFile symstream(symname.c_str(), "wb");
            
            if (!symstream)
            {
                reporter->warn("Could not open symbol file %s; labels not saved.",
                    symname.c_str());
                retval = min(retval, SuccessWarn);
            }
            else
            {
                reporter->note("Writing labels to %s...",
                    symname.c_str());
                table.write(symstream);
            }
        }
        else
        {
            reporter->note(
                "No labels to write; no symbol file generated.");
        }
    }
    
    return retval;
}

int main(int argc, char *argv[])
{
    int retval = 0;
    const Arch& arch = ARCHCLASS::instance();

    fprintf(stderr, " ** Generalized assembler: %s, version %s **\n",
        ARCHCLASS::instance().name(), VERSION_STRING);
    
    if (argc <= 1)
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "Basic usage: %s filename.asm\n", argv[0]);
        fprintf(stderr, " - The output files will be filename%s and filename.sym.\n",
            arch.objExtension());
        fprintf(stderr, "\n");
        fprintf(stderr, "Advanced usage: %s <input files> [-o outputfile]\n",
            argv[0]);
        fprintf(stderr, " - Input files can be .asm, .s, .obj, .lc, .bin, .hex, or .sym.\n");
        fprintf(stderr, " - Optional output file can be any of those formats except .sym;\n");
        fprintf(stderr, " default format is %s, and a .sym file is written if there are symbols.\n",
            arch.objExtension());
        fprintf(stderr, " - File types determined completely by extension.\n");
        fprintf(stderr, " - Each file is read into memory, and the entire range is written to file.\n");
        retval = 1;
    }
    else
    {
        Ref<AsmView> view(new AsmView());
        Ref<Machine> machine(arch.makeMachine(view));
        CodeEmitter emitter(machine);
        SymTable table;
        SuccessT suc = SuccessPass;
        String outname;
        
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-o") == 0 && ++i < argc)
            {
                outname = argv[i];
            }
            else
            {
                suc = min(suc, readIn(argv[i], arch, &emitter, &table, view));
            }
        }
        
        if (suc == SuccessFail)
        {
            fprintf(stderr, "At least one error found.\n");
            retval = 1;
        }
        else
        {
            String mainname;
            
            if (outname != "") mainname = outname;
            else mainname = argv[1];
            
            const char *s = mainname.findr('.');
            if (!s) s = mainname.c_str() + mainname.length();
            String basename = mainname(0, s - 1);
            
            if (outname == "")
            {
                outname = basename + arch.objExtension();
            }
            
            if (writeOut(outname, basename, *machine, table, view,
                emitter.minAddr(), emitter.maxAddr()) != SuccessPass)
            {
                fprintf(stderr, "Error writing output file.\n");
                retval = 1;
            }
            else
            {
                retval = 0;
            }
        }
        
        machine = 0;
    }
    
    return retval;
}
