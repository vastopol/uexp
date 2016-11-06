#include "commandui.h"
#include "textstream.h"

#include <unistd.h>
#include <ctype.h>

const char *cmdui_help_main[] =
{
    "MAIN",
    "",
    " Welcome to the simulator text interface.",
    "",
    " There are several categories of commands to get help on:",
    "",
    "   help display      (print, list, data, dump, page, stack, state)",
    "   help running      (run, step, next, finish, fastrun)",
    "   help manipulation (set, go, save, restore, input, infile)",
    "   help modes        (viewmode [colors etc], runmode [traps etc], verbose)",
    "   help numbers      (types of numbers you can input)",
    "   help extras       (running multiple commands, etc)",
    "",
    " To exit, type 'quit'.",
    NULL
};

const char *cmdui_help_numbers[] =
{
    "NUMBERS",
    "",
    " <num> A numerical expression.",
    "    Literals: 12 x3102 SOME_LABEL 0b0011000100000010 0777 pc '\\n' <reg>",
    "    Operations: add(+), subtract(-), multiply(*), divide(/), negate(-)",
    "    Dereference: @(someByteAddress) *(someWordAddress) ^(someInstruction)",
    "",
    " <addr> A memory address, specified in the same form as for <num>.",
    "",
    " <place> A destination: a register, PC, or a dereferenced addr.  A literal",
    "    number is assumed to be a memory word (i.e. 0x50 is read as *0x50).",
    "",
    " <reg> A register, following the machine's registers.",
    "",
    " EXAMPLES",
    "   15+25  (number 40)             ^x3121  (instruction at 0x3121)",
    "   pc+7   (program counter + 7)   15+*r6  (word pointed to by r6, plus 15)",
    "   ^x3210 (instruction at x3210)  NUMS+5  (value of symbol NUMS, plus 5)",
    "   13-*(r6+*r7)  (dereference r7, add r6, dereference, subtract from 13)",
    NULL
};

const char *cmdui_help_display[] =
{
    "DISPLAY",
    "",
    "   p[rint] <place>         print contents of the destination",
    "   l[ist] [addr1 [addr2]]  print mem[addr1] through mem[addr2] as instructions",
    "   da[ta] [addr1 [addr2]]  list mem[addr1] through mem[addr2] as data",
    "   d[ump] [addr1 [addr2]]  dump mem[addr1] through mem[addr2]",
    "   pa[ge]                  continue listing/dumping from address left off",
    "   stac[k]                 show the contents of the stack",
    "   stat[e]                 print machine's general state",
    "",
    "",
    " See 'help numbers' for info on <addr> and <place>.",
    NULL
};

const char *cmdui_help_running[] =
{
    "RUNNING THE CODE",
    "   r[un] or c[ontinue]     resume execution until next breakpoint",
    "   s[tep]                  execute one instruction",
    "   n[ext]                  execute one instruction, but do not trace subroutines",
    "   fin[ish] <addr>         finish executing suroutine <addr>; eg, finish MUL",
    "   fa[strun]               run faster (sometimes 10x) but with less debugging",
    NULL
};

const char *cmdui_help_manipulation[] =
{
    "MANIPULATION",
    "   se[t] <place> <num>     set a destination to a value",
    "   - set r6 r6+2           add two to register 6",
    "   - set *x3200 35         set the word at 0x3200 to 35 decimal",
    "  ",
    "   br[eak] <addr>          toggle breakpoint at addr",
    "   bl[ackbox] <addr>       toggle blackbox subroutine at addr",
    "  ",
    "   sa[ve]                  save machine's current state (including breakpoints)",
    "   res[tore]               restore back to previous state, or original state",
    "  ",
    "   inf[ile] <filename>     pre-specify input from the given file",
    "   inp[ut]                 pre-specify the program's input (text mode)",
    "",
    " See 'help numbers' for info on <addr>, <place>, and <num>.",
    NULL
};

const char *cmdui_help_modes[] =
{
    "MODES",
    "   runm[ode]               simulation modes; 'runmode' for info",
    "   vi[ewmode]              modes that affect view; 'viewmode' for info",
    "   v[erbose] on|off        enable or disable verbose mode",
    NULL
};

const char *cmdui_help_extras[] =
{
    "PSEUDO-COMMANDS",
    "   step;print x3300;list x3000",
    "                           You can combine any commands with semicolons.",
    "                           (My favorite: step;list;stack)",
    "   x25                     Repeat previous command 25 times",
    "   <ENTER>                 Hit ENTER with a blank line to repeat the last",
    "                           'interesting' command. [[console mode]]",
    "   q[uit]                  Quit the simulator [[console mode]]",
    NULL
};

/** Default number of lines for a 'list' command. */
#define LIST_COUNT 24

CommandUI::CommandUI(Model* model, TextStream *stream, InputBuffer* inputBuffer)
 : Reporter(stream)
 , mViewMode(0)
 , mModel(model)
 , mInputBuffer(inputBuffer)
{
    mLastListAddr = 0;
    mLastListType = LocID::MemInstr;
    
    mUser = stream;

    const Array<Option>* pOptions =
        &mModel->formatOptions();
    
    for (size_t i = 0; i < (*pOptions).size(); i++)
    {
        if ((*pOptions)[i].isDefault()) mViewMode |= (*pOptions)[i].val();
    }
    
    // infer color mode from the stream type
    mViewMode &= ~Format::ViewColor;
    if (stream->useColor()) mViewMode |= Format::ViewColor;
    
    mLastListAddr = mModel->pc();
    setSubReporter(stream);
}

void CommandUI::showState()
{
    mUser->printf("%s", mModel->formatState(mViewMode).c_str());
}

// TODO: %04X used.
void CommandUI::showStack()
{
    //mUser->printf("   %8s %8s %s\n", "--------", "--------", "----------");
    mUser->printf("   %8s %8s %s\n", "RetnAddr", "StackPtr", "Subroutine");
    
    for (size_t i = mModel->stackSize(); i-- > 0;)
    {
        Model::StackEntry entry(*mModel, i);
        
        mUser->color("1;32");
        if (entry.retAddr > 0)
            mUser->printf("   %8s",
                mModel->format(LocID::PC, entry.retAddr, Format::DisplayHex)
                .c_str());
        else
            mUser->printf("   --------");
        
        mUser->color("0;33");
        
        if (entry.sp > 0)
            mUser->printf(" %8s",
                mModel->format(LocID::PC, entry.sp, Format::DisplayHex)
                .c_str());
        else
            mUser->printf(" --------");
        
        mUser->color("1;35");
        mUser->printf(" ");
        mUser->printf("%s",
            mModel->format(LocID::PC, entry.subAddr).c_str());
        mUser->printf("\n");
    }
    mUser->color("0");
}

void CommandUI::showAddr(GAddr addr, LocID::Type type)
{
    if (type == LocID::Invalid)
    {
        showAddr(addr, LocID::MemByte);
        
        if (mModel->bytesOf(LocID::MemWord) > 1
            && mModel->isAligned(LocID(LocID::MemWord, addr)))
        {
            // Show it as a word, if word != byte, and it's word-aligned
            showAddr(addr, LocID::MemWord);
        }
        
        if (mModel->isAligned(LocID(LocID::MemInstr, addr)))
        {
            // Disassemble if it's instruction-aligned
            showAddr(addr, LocID::MemInstr);
        }
    }
    else
    {
        if (mModel->isBreak(addr))
        {
            mUser->color("1;31");
            mUser->printf("*");
        }
        else if (mModel->isBlack(addr))
        {
            mUser->color("1;30");
            mUser->printf("#");
        }
        else
        {
            mUser->printf(" ");
        }
        // TODO: isExec, Symbolic
        mUser->printf("%s\n",
            mModel->formatLine(LocID(type, addr), mViewMode).c_str());
    }
}

void CommandUI::dumpAddr(GAddr addr, GAddr highlightAddr)
{
    mUser->printf("%s\n",
        mModel->dumpLine(addr, mViewMode, highlightAddr).c_str());
}

void CommandUI::dumpRange(GAddr start, GAddr end, GAddr highlightAddr)
{
    GAddr min;
    GAddr max;
    GAddr count = calcDumpBytes();
    
    // DEM - Required inspection?
    if (mModel->lastAddress() < 256)
    {
        min = 0;
        max = mModel->lastAddress();
    }
    else
    {
        min = start & ~0xf;
        min &= ~(count - 1);
        max = end;
    }
    
    for (GAddr i = min; mModel->wrapAddr(i - max - 1) > count;
        i = mModel->wrapAddr(i + count))
    {
        dumpAddr(mModel->wrapAddr(i), highlightAddr);
    }
}

void CommandUI::listRange(GAddr start, GAddr end, LocID::Type type)
{
    GAddr diff = mModel->bytesOf(type);
    // TODO: Rounding
    // DEM - Required?
    // This funny loop condition is really "addr <= end" but allows wraparound.
    for (GAddr addr = start; mModel->wrapAddr(addr - end - 1) > diff;
        addr = mModel->wrapAddr(addr + diff))
    {
        for (size_t frame = 0; frame < mModel->stackSize(); ++frame)
        {
            Model::StackEntry se(*mModel, frame);
            if (addr != 0 && GAddr(addr) == se.sp)
            {
                mUser->printf(" ---- %s frame end\n",
                    mModel->format(LocID::PC, se.subAddr).c_str());
            }
        }
        
        showAddr(addr, type);
    }
}

void CommandUI::listStack(GAddr end)
{
    GAddr start = mModel->get(mModel->stackPointerLocation());
    LocID::Type type = LocID::MemWord;
    GAddr numList = 16 * mModel->bytesOf(type);
    
    if (end == 0) end = mModel->wrapAddr(start + numList);
    
    listRange(start, end, type);
}

/*
 * fewest is the least amount of characters required to match
 * most is the most possible characters allowed to match
 */
int starts_with(const char *s, const char *fewest, const char *most)
{
    return ((most == strstr(most, s)) && (s == strstr(s, fewest)));
}

char *strtoken(char **spp, char delim)
{
    char *s = *spp;
    char *end;
    
    if (!s || !*s)
    {
        *spp = NULL;
        return NULL;
    }
    
    if ((end = strchr(s, delim)) != NULL)
    {
        *end++ = '\0';
        while (*end == delim) end++;
        if (!*end) end = NULL;
    }
    
    *spp = end;
    
    return s;
}

void CommandUI::doSet(const char *deststr, const char *valstr)
{
    // TODO: Invalid locations?
    LocID location = mModel->parseDest(deststr);
    
    if (location.type() != LocID::Invalid)
    {
        GVal value;
        if (mModel->parse(valstr, location, &value) != SuccessFail)
        {
            mModel->set(location, value);
            
            if (location.isInMemory())
            {
                showAddr(location.id(), location.type());
            }
            else
            {
                showState();
            }
        }
    }
    else
    {
        err("Invalid machine location to set.");
    }
}

void CommandUI::doInfile(const char *fname)
{
    if (mInputBuffer)
    {
        OpenFile f(fname, "r");
        
        if (f)
        {
            int c;
            String buf = mInputBuffer->inputText();
            
            while ((c = fgetc(f)) != EOF)
            {
                buf += c;
            }
            
            mInputBuffer->setInputText(buf);
            
            note("File %s successfully read into input buffer.", fname);
        }
        else
        {
            err("Error loading file.");
        }
    }
    else
    {
        err("The infile command is not supported.");
    }
}

void CommandUI::doInput()
{        
    if (mInputBuffer)
    {
        note("This command allows you to pre-specify input, rather than");
        note("specifying the input when the program is running.");
        
        mInputBuffer->setInputText(mUser->editRaw(mInputBuffer->inputText()));
    }
    else
    {
        err("The input command is not supported.");
    }
}

SuccessT CommandUI::doParseOptions(const char* cmd, const char* help,
    const Array<Option>& options, const char* arg,
    int* pFlag, bool* pState)
{
    SuccessT suc = SuccessFail;
    
    if (*arg == '+' || *arg == '-')
    {
        *pState = (*arg == '+');
        for (size_t i = 0; i < options.size(); i++)
        {
            if ((arg[1]|0x20) == (options[i].accel()|0x20))
            {
                *pFlag = options[i].val();
                note("Turning %s mode \"%s\" (%c).",
                    *pState ? "on" : "off",
                    options[i].name(), options[i].accel());
                suc = SuccessPass;
            }
        }
    }
    
    if (suc != SuccessPass)
    {
        mUser->printf("%s\n", help);
        mUser->fputs("\n");
        mUser->fputs("Syntax:\n");
        mUser->printf("  %s +x  (to turn mode 'x' on)\n", cmd);
        mUser->printf("  %s -x  (to turn mode 'x' off)\n", cmd);
        mUser->fputs("\n");
        mUser->fputs("Available options:\n");
        
        for (size_t i = 0; i < options.size(); i++)
        {
            mUser->printf(" +%c : %s\n", options[i].accel()|0x20,
                options[i].name());
        }
        
        mUser->fputs("");
    }
    
    return suc;
}

int CommandUI::execCmd(const char* line, bool first)
{
    String buf = line;
    char *cmd = buf.begin();
    char *s;
    char *args[2];
    GAddr addr;
    int rv = 0;
    SuccessT suc = SuccessPass;
    
    while (isspace(*cmd)) cmd++;
    
    s = cmd;
    
    /* tokenize the cmd */
    for (int i = 0; i < 2; i++)
    {
        char *end;
        while (*s && !isspace(*s)) s++;
        end = s;
        while (isspace(*s)) s++;
        *end = '\0';
        args[i] = s;
    }
    
    for (s = cmd; *s; s++)
        *s |= 0x20; /* poor man's to-lower */
    
    // TODO: Smarter dispatch algorithm?
    // Idea: Use assembler framework.  That way, autocomplete or
    // context-sensitive would also be possible (i.e. with readline).
    
    if (starts_with(cmd, "vi", "viewmode"))
    {
        int flag;
        bool state;
        
        suc = doParseOptions("viewmode",
            "Turn on or off various formatting options.",
            mModel->formatOptions(), args[0],
            &flag, &state);
        
        if (suc == SuccessPass)
        {
            mViewMode &= ~flag;
            if (state) mViewMode |= flag;
            if (flag == Format::ViewColor) mUser->setUseColor(state);
        }
    }
    else if (starts_with(cmd, "runm", "runmode"))
    {
        int flag;
        bool state;
        
        suc = doParseOptions("runmode",
            "Turn on or off various simulation options.",
            mModel->runOptions(), args[0],
            &flag, &state);
        
        if (suc == SuccessPass)
        {
            mModel->setRunMode(flag, state);
        }
    }
    else if (starts_with(cmd, "s", "step"))
    {
        mModel->doStep(false);
        rv |= SIMFL_CANREPEAT;
    }
    else if (starts_with(cmd, "n", "next"))
    {
        mModel->doStep(true);
        rv |= SIMFL_CANREPEAT;
    }
    else if (starts_with(cmd, "c", "continue") 
		|| starts_with(cmd, "r", "run"))
    {
        mModel->doRun();
        rv |= SIMFL_CANREPEAT;
    }
    else if (starts_with(cmd, "fa", "fast") || strcmp(cmd, "fastrun") == 0)
    {
        mModel->doFastRun();
    }
    else if (starts_with(cmd, "fin", "finish"))
    {
        /* FIXME TODO: Belongs in mModel */
        if (*args[0]) suc = mModel->parse(args[0], LocID::PC, &addr);
        else addr = mModel->subroutineAddr();
        
        if (suc != SuccessFail) mModel->doFinish(addr);
    }
    else if (starts_with(cmd, "inp", "input"))
    {
        doInput();
    }
    else if (starts_with(cmd, "inf", "infile"))
    {
        if (!mInputBuffer) err("Infile is not supported by this version.");
        else if (!*args[0]) err("With infile please give a filename.");
        else doInfile(args[0]);
    }
    else if (starts_with(cmd, "v", "verbose"))
    {
        setVerbosityLevel((String(args[0]).toLower() == "on")
            ? Verbose : NormalVerbosity);
        if (verbosityLevel() == Verbose)
        {
            note("Status will be printed after every instruction.");
            note("To avoid flooding, there is limit of 1000 instructions.");
        }
        mModel->setLimit(verbosityLevel() == Verbose ? 1000 : 0);
    }
    else if (starts_with(cmd, "q", "quit") || starts_with(cmd, "ex", "exit"))
    {
        rv |= SIMFL_DONE;
    }
    else if (starts_with(cmd, "col", "color"))
    {
        bool state = false;
        if (!*args[0])
        {
            state = !mUser->useColor();
        }
        else
        {
            state = (String(args[0]).toLower().compareTo("on") == 0);
        }
        mUser->setUseColor(state);
        mViewMode &= ~Format::ViewColor;
        if (state) mViewMode |= Format::ViewColor;
    }
    else if (starts_with(cmd, "br", "breakpoint"))
    {
        if (mModel->parse(args[0], LocID::PC, &addr) != SuccessFail)
        {
            mModel->toggleBreak(addr);
            note("Breakpoint at x%04X is now %s.",
                addr, (mModel->isBreak(addr)) ? "set" : "CLEARED");
        }
    }
    else if (starts_with(cmd, "bl", "blackbox"))
    {
        if (mModel->parse(args[0], LocID::PC, &addr) != SuccessFail)
        {
            mModel->toggleBlack(addr);
            note("Blackbox at x%04X is now %s.",
                addr, (mModel->isBlack(addr)) ? "set" : "CLEARED");
        }
    }
    else if (starts_with(cmd, "la", "label"))
    {
        if (!*args[0])
        {
            err("No label specified.");
        }
        else
        {
            if (*args[1]) suc = mModel->parse(args[1], LocID::PC, &addr);
            else addr = mModel->pc();
            
            if (suc != SuccessFail)
            {
                mModel->addSym(args[0], addr);
                showAddr(addr);
                note("Added label for x%04X to %s.", addr, args[0]);
            }
        }
    }
    else if (starts_with(cmd, "lo", "load") || starts_with(cmd, "fil", "file"))
    {
        if (!*args[0])
            err("You must provide a filename to load.");
        
        if (*args[0] == '"' && (s = strchr(args[0]+1, '"')))
        {
            args[0]++;
            *s = '\0';
        }
        
        if (mModel->load(args[0]) != SuccessPass)
        {
            err("Error loading file %s.", args[0]);
        }
        else
        {
            note("File %s loaded and PC set to x%04X.",
                args[0], mModel->pc());
        }
    }
    else if (starts_with(cmd, "stat", "state") || starts_with(cmd, "bt", "bt")
		|| starts_with(cmd, "back", "backtrace"))
    {
        if (first)
        {
            // Show extra information if this is the primary command.
            // Otherwise, assume the user ONLY wants to see the trace.
            note("Total instructions executed: %d.", mModel->totalCount());
            showState();
            //printOutputBuf();
            mUser->printf("\n");
        }
        showStack();
    }
    else if (starts_with(cmd, "sa", "save"))
    {
        mModel->save();
        note("Machine state is saved.");
    }
    else if (starts_with(cmd, "res", "restore"))
    {
        mModel->restore();
        note("Machine state restored.");
        showState();
    }
    else if (starts_with(cmd, "rer", "rerun"))
    {
        mModel->restore();
        mModel->halt(false);
        mModel->doRun();
        rv |= SIMFL_CANREPEAT;
    }
    else if (starts_with(cmd, "p", "print"))
    {
        LocID dest = mModel->parseDest(args[0]);
        
        if (dest.type() != LocID::Invalid)
        {
            if (dest.isInMemory())
            {
                showAddr(dest.id());
            }
            else
            {
                // TODO: Highlight
                showState();
            }
        }
    }
    else if (starts_with(cmd, "stac", "stack"))
    {
        listStack();
    }
    else if (starts_with(cmd, "l", "list") || starts_with(cmd, "d", "dump")
             || starts_with(cmd, "pg", "pg") || starts_with(cmd, "pa", "page")
             || starts_with(cmd, "da", "data"))
    {
        bool ispage = (*cmd == 'p');
        GAddr last;
        SuccessT sucLast = SuccessPass;
        LocID::Type type;
        
        if (ispage) type = mLastListType;
        else if (starts_with(cmd, "l", "list")) type = LocID::MemInstr;
        else if (starts_with(cmd, "da", "data")) type = LocID::MemWord;
        else type = LocID::MemByte;
        
        if (ispage && first) mUser->sendEscape("[A");
        
        if (ispage) addr = mLastListAddr;
        else if (*args[0]) suc = mModel->parse(args[0], LocID::PC, &addr);
        else addr = mModel->wrapAddr(mModel->pc() - 8);
        
        if (ispage || !*args[1])
        {
            int lineDensity;
            if (type == LocID::MemByte) lineDensity = calcDumpBytes();
            else lineDensity = mModel->bytesOf(type);
            last = addr + LIST_COUNT * lineDensity;
        }
        else
        {
            sucLast = mModel->parse(args[1], LocID::PC, &last);
        }
        
        if (suc != SuccessFail && sucLast != SuccessFail)
        {
            if (type != LocID::MemByte) listRange(addr, last, type);
            else dumpRange(addr, last, *args[0] ? addr : mModel->pc());
            mLastListAddr = mModel->wrapAddr(last + 1);
            mLastListType = type;
        }
        
        if (ispage) rv |= SIMFL_CANREPEAT;
    }
    else if (starts_with(cmd, "g", "go"))
    {
        if (mModel->parse(args[0], LocID::PC, &addr) != SuccessFail)
            mModel->set(LocID::PC, addr);
        showState();
    }
    else if (starts_with(cmd, "se", "set") || starts_with(cmd, "reg", "reg")
        || starts_with(cmd, "m", "mem"))
    {
        doSet(args[0], args[1]);
    }
    else if (starts_with(cmd, "h", "help"))
    {
        const char * const * text;
        
        if (strcmp(args[0], "display") == 0)
        {
            text = cmdui_help_display;
        }
        else if (strcmp(args[0], "running") == 0)
        {
            text = cmdui_help_running;
        }
        else if (strcmp(args[0], "manipulation") == 0)
        {
            text = cmdui_help_manipulation;
        }
        else if (strcmp(args[0], "modes") == 0)
        {
            text = cmdui_help_modes;
        }
        else if (strcmp(args[0], "numbers") == 0)
        {
            text = cmdui_help_numbers;
        }
        else if (strcmp(args[0], "extras") == 0)
        {
            text = cmdui_help_extras;
        }
        else
        {
            text = cmdui_help_main;
        }
        
        mUser->printLines(text);
    }
    else
    {
        err("Unknown command: %s.", cmd);
        note("For help on commands and usage, use the \"help\" command.");
    }
    
    return rv;
}

int CommandUI::doExecLine(const char *line)
{
    String buf = line;
    char *s = buf.begin();
    char *cmd;
    int rv = 0;
    bool first = true;
    
    while ((cmd = strtoken(&s, ';')) != NULL)
    {
        rv |= execCmd(cmd, first);
        first = false;
    }
    
    return rv;
}

bool CommandUI::execLine(const char *line)
{
    const char *to_exec = line;
    int times = 1;
    int rv = 0;
    
    if (!*to_exec)
    {
        to_exec = mLastLine.c_str();
    }
    else if (*to_exec == 'x')
    {
        times = atoi(to_exec + 1);
        note("Repeating last command %d times", times);
        to_exec = mLastLine.c_str();
    }
    
    do {
        rv = doExecLine(to_exec);
    } while (!(rv & SIMFL_DONE) && !mModel->isHalted() && --times);
    
    if (line == to_exec && (rv & SIMFL_CANREPEAT))
    {
        mLastLine = line;
    }
    
    if (mModel->isHalted())
    {
        note("Machine halted.  Total executed: %d.", mModel->totalCount());
    }
    
    return !(rv & SIMFL_DONE);
}


