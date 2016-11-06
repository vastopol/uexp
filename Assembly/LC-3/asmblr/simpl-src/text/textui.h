#ifndef TEXTUI_H
#define TEXTUI_H

#include "ui/inputbuffer.h"
#include "ui/commandui.h"
#include "ui/simui.h"
#include "ui/textstream.h"
#include <deque>

/**
 * Text-driven user interface.
 */
class TextUI : public SimUI
{
    DISALLOW_COPY(TextUI);
    
  private:
    class InputBufferAdapter : public InputBuffer
    {
     public:
        InputBufferAdapter(TextUI* ui)
         : mUI(ui) { }
        
        TextUI* mUI;
        
        String inputText() const;
        void setInputText(const char* newText);
    };
    
    friend class InputBufferAdapter;
  
  public:
    /**
     * Construct a new text UI.
     */
    TextUI();
    /**
     * Destroy resources associated with this text UI.
     */
    ~TextUI();
    
  public:
    /** Read a character from console (changes behavior based on verboseIO). */
    int readChar();
    /** Output a character (changes behavior via VerboseIO). */
    int writeChar(int c);
    
    /**
     * Update after each instruction by printing the current state, if in
     * verbose mode.
     */
    void handleInstructionExecuted();
    /**
     * Update after last instruction in a sequence, by printing the
     * current state.
     */
    void handleInstructionSetExecuted();
    /**
     * Sends a message that the model is changed; does little for TextUI
     * as no GUI redraws are necessary.
     */
    void handleModelUpdated();
    
    /** Parse specific command line options (does nothing) */
    void parseArgs(int &UNUSED(argc), char **UNUSED(argv)) { }
    /** Install the ^C signal handler. */
    void install();
    /** Show (does nothing). */
    void show() { }
    /** Uninstall the signal handler. */
    void uninstall();
    /** Begin readline loop. */
    void loop();
    
  private:
    /** Print the current output buffer */
    void printOutputBuf();
    /** Prompt for a character in verbose mode, showing output buffer and all.
     */
    int promptChar();
    
  private:
    /** Signal handler */
    static void signalHandler(int signum);
    /** Simulator instance that the handler will use */
    static TextUI *signalHandlerInstance;
    /** Number of times the handler has been executed before loop has been
     * resumed.  Used to implement that three control-C's will quit
     * the program altogether. */
    static int interruptionCount;
    
    /** Get the number of bytes a line for dumping. */
    int dumpCount() const
     { return mModel->calcDumpBytes(); }
    
  private:
    /** Input buffer for canned input */
    std::deque<char> mInbuf;
    /** Circular output buffer window */
    Array<char> mOutbuf;
    /** Position in output circular buffer */
    int mOutpos;
    
    /** User input device */
    TextStream* mUser;
    
    /** Command handler */
    Ref<CommandUI> mCommand;
};

#endif
