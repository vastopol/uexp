#ifndef COMMANDUI_H
#define COMMANDUI_H

#include "model/model.h"
#include "reporter.h"
#include "textstream.h"
#include "inputbuffer.h"
#include <deque>

/**
 * Text-driven user interface.
 */
class CommandUI : public Reporter
{
    DISALLOW_COPY(CommandUI);
    
  public:
    enum { SIMFL_CANREPEAT = 0x1, SIMFL_DONE = 0x2 };
    
    static const int BadAddr = GAddr(-1);
  
  public:
    /**
     * Construct a new text UI.
     */
    CommandUI(Model* model, TextStream *stream, InputBuffer* inputBuffer = 0);
    /**
     * Destroy resources associated with this text UI.
     */
    ~CommandUI() { }
  
  public:
    /** Show machine's regiser state. */
    void showState();
    /** Show machine's current call stack */
    void showStack();
    /** Show memory at a particular addess */
    void showAddr(GAddr addr, LocID::Type type = LocID::Invalid);
    /** Dump memory at a particular address (and the 7 following).
     * @param startAddr - the first of the 8 addresses
     * @param highlightAddr - the address to highlight (may or may not be in
     *        the range)
     */
    void dumpAddr(GAddr startAddr, GAddr highlightAddr = 0);
    /**
     * List the memory at the current stack.
     */
    void listStack(GAddr end = 0);
    /**
     * List the memory range start to end.
     */
    void listRange(GAddr start, GAddr end, LocID::Type type = LocID::MemInstr);
    /**
     * Dump a memory range from start to end, optionally given an address
     * to highlight.
     *
     * @param start - beginning address
     * @param end - end address
     * @param highlightAddr - address to highlight
     */
    void dumpRange(GAddr start, GAddr end, GAddr highlightAddr = BadAddr);
    /**
     * Parse and execute a command.
     * Returns an or of SIMFL_DONE and SIMFL_CANREPEAT.
     */
    int execCmd(const char *cmd, bool alone = true);
    /**
     * Parse and execute a line (which may be several commands separated
     * by semicolons).
     *
     * @return false iff quit was called
     */
    bool execLine(const char *line);
    /** Execute a "set" command */
    void doSet(const char *deststr, const char *valstr);
    /** Read input from console into input buffer */
    void doInput();
    /** Read input from a file */
    void doInfile(const char *fname);
    /** Parse options for viewmode and runmode commands */
    SuccessT doParseOptions(const char* cmd, const char* help,
        const Array<Option>& options, const char* arg,
        int* pFlag, bool* pState);
    
    /** Gets the last interesting, repeatable command. */
    const String& lastLine() const
     { return mLastLine; }

  private:    
    /** Get the number of bytes a line for dumping. */
    int calcDumpBytes() const
     { return mModel->calcDumpBytes(); }
    /** Executes a single line, without support for repetition. */
    int doExecLine(const char *line);
    
  private:
    /** Where the last LIST command ended (for page command) */
    GAddr mLastListAddr;
    /** Last type of list shown */
    LocID::Type mLastListType;
    
    /** Set view flags */
    int mViewMode;
    
    /** User input device */
    TextStream* mUser;
    
    /** Internal model */
    Model* mModel;
    /** Device to store the input buffer in */
    InputBuffer* mInputBuffer;
    /** Last non-trivial command executed. */
    String mLastLine;
};

#endif
