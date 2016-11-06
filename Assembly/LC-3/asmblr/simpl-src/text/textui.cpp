#include "textui.h"
#include "term.h"
#include <cstring>
#include <cstdlib>
#include <stdarg.h>
#include <ctype.h>
#include <signal.h>
#include "terminalstream.h"
#include "asm/asmrule.h"

#include <unistd.h>

TextUI *TextUI::signalHandlerInstance = NULL;
int TextUI::interruptionCount = 0;

#define PRINT_WID 60
#define LIST_COUNT 24
#define SIM_OUTSIZE 256

TextUI::TextUI()
 : SimUI(),
   mOutbuf(256, 127)
{
    mOutpos = 0;
    mUser = new TerminalStream();
    setSubReporter(mUser);
}

TextUI::~TextUI()
{
    if (signalHandlerInstance == this)
        signalHandlerInstance = NULL;
}

void TextUI::printOutputBuf()
{
    int start = (mOutpos + mOutbuf.size() - PRINT_WID) % mOutbuf.size();
    
    mUser->printf("\t[");
    
    for (int i = 0; i < PRINT_WID; i++)
        mUser->dumpChar(mOutbuf[(start + i) % mOutbuf.size()]);
    
    mUser->color("0");
    mUser->printf("]");
}

int TextUI::writeChar(int i)
{
    mOutbuf[mOutpos] = i;
    mOutpos = (mOutpos + 1) % mOutbuf.size();
    
    if (!mModel->isRunning() || verbosityLevel() > NormalVerbosity)
    {
        mUser->printf(">> [");
        mUser->color("1;35");
        mUser->printf("%c", i);
        mUser->color("0");
        mUser->printf("] ");
        printOutputBuf();
        mUser->printf("\n");
    }
    else
    {
        fputc(i, stdout);
    }
    
    return 0;
}

int TextUI::promptChar()
{
    int i;
    
    mUser->printf("<< ");
    mUser->color("35");
    mUser->printf("Please specify keyboard input\n");
    mUser->color("0");
    mUser->printf("   ");
    printOutputBuf();
    mUser->beep();
    mUser->color("1;35");
    mUser->printf("?");
    (void) fflush(stdout);
    i = fgetc(stdin);
    mUser->color("0");
    mUser->printf("[");
    mUser->echoChar(i);
    mUser->color("0");
    mUser->printf("]\n");
    mUser->color("0");
    
    return i;
}

int TextUI::readChar()
{
    int i;
    
    if (!mInbuf.empty())
    {
        i = mInbuf.front();
        mInbuf.pop_front();
    }
    else if (!mModel->isRunning() || verbosityLevel() > NormalVerbosity)
    {
        i = promptChar();
    }
    else
    {
        i = fgetc(stdin);
    }
    
    return i;
}

void TextUI::handleInstructionExecuted()
{
    if (verbosityLevel() > NormalVerbosity) mCommand->showState();
}

void TextUI::handleInstructionSetExecuted()
{
    if (verbosityLevel() == NormalVerbosity) mCommand->showState();
}

void TextUI::handleModelUpdated()
{
}

void TextUI::loop()
{
    bool keepGoing = true;
    String last;
    
    setSubReporter(mCommand);
    mModel->save();
    
    mUser->printf("Welcome to the Simp text interface.  (%s, version %s)\n",
        mModel->archName(), VERSION_STRING);
    mUser->printf("\n");
    // DEM
    mUser->printf("         byte = %d bits; address = %d bits\n",
        mModel->bitsOf(LocID::MemByte), mModel->bitsOf(LocID::PC));
    mUser->printf("         word = %d bits (%d bytes), instruction = %d bits (%d bytes)\n",
        mModel->bitsOf(LocID::MemWord), mModel->bytesOf(LocID::MemWord),
        mModel->bitsOf(LocID::MemInstr), mModel->bytesOf(LocID::MemInstr));
    mUser->printf("\n");
    mUser->printf("For help, type in this command: help\n\n");
    
    #ifdef USE_READLINE
    rl_bind_key('\t', rl_insert); /* turn off tab completion */
    #endif
    
    mUser->addHistory("help"); /* help start them off */
    
    mCommand->showState();
    
    while (keepGoing)
    {
        Term::in.exitRaw();
        
        mModel->halt(false); // un-halt the machine
        interruptionCount = 0; // if ^C was pressed, clear the count
        
        String line = mUser->readline();
        
        Term::in.enterRaw();
        
        if (line.length() != 0)
        {
            mUser->addHistory(line);
        }
        
        keepGoing = keepGoing && mCommand->execLine(line);
    }
    
    mUser->printf("\nHave a nice day!\n");
}

void TextUI::signalHandler(int UNUSED(signum))
{
    if (!signalHandlerInstance) return;
    
    if (++interruptionCount >= 3)
    {
        signalHandlerInstance->err("Hit ^C three times, aborting.");
        Term::in.exitRaw();
        exit(1);
    }
    else
    {
        signalHandlerInstance->mModel->halt();
        signalHandlerInstance->note("");
        signalHandlerInstance->note("Pausing machine; use 'quit' to exit, or 'run' to resume.");
    }
}

void TextUI::install()
{
    struct sigaction sa;
    sigset_t s_empty;
    
    mCommand = new CommandUI(mModel, mUser, new InputBufferAdapter(this));
    
    if (signalHandlerInstance)
    {
        err("Programmer bug: TextUI::install: re-registering.");
    }
    else
    {
        sigemptyset(&s_empty);
        sa.sa_handler = TextUI::signalHandler;
        sa.sa_mask = s_empty;
        sa.sa_flags = 0;
        signalHandlerInstance = this;
        
        sigaction(SIGINT, &sa, NULL);
    }
    Term::in.enterRaw();
}

void TextUI::uninstall()
{
    // TODO FIXME how to uninstall a signal handler?
    if (signalHandlerInstance == this) signalHandlerInstance = NULL;
    Term::in.exitRaw();
}

String TextUI::InputBufferAdapter::inputText() const
{
    String str;
    
    str.setLength(mUI->mInbuf.size());
    
    for (unsigned i = 0; i < mUI->mInbuf.size(); i++)
    {
        str[i] = mUI->mInbuf[i];
    }
    
    return str;
}

void TextUI::InputBufferAdapter::setInputText(const char* newText)
{
    int len = strlen(newText);
    
    mUI->mInbuf.resize(len);
    
    for (int i = 0; i < len; i++)
    {
        mUI->mInbuf[i] = newText[i];
    }
}
