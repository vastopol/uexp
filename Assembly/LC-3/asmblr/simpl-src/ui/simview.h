#ifndef SIMVIEW_H
#define SIMVIEW_H

#include "reporter.h"

/**
 * This class is the model's look at the user, with only a few important
 * hooks.  Among other things, the model uses the SimView for I/O,
 * error reporting, and model/view/control elements.
 */
class SimView
 : public Reporter
{
    DISALLOW_COPY(SimView);
    
 public:
    /**
     * Construct a worthlessly empty SimView.
     */
    SimView()
     { }
    
    virtual ~SimView()
     { }
    
 public:
    /**
     * Update once.
     *
     * This event is to be called after every single instruction execution
     * (except for blackboxed traps and routines).
     */
    virtual void handleInstructionExecuted()
     { }
    /**
     * Update at the end of a sequence of instructions executed.
     *
     * This is called at the end of an instruction sequence, and by default
     * this shows the state, if it was not already shown by
     * handleInstructionExecuted(), but not if in silent mode.
     */
    virtual void handleInstructionSetExecuted()
     { }
    
    /**
     * This function is called when any update has been made to the machine
     * state (but not during execution or such).
     *
     * This is NOT called after each instruction in a step or after a run
     * of instructions -- use the handleInstructionExecuted and
     * handleInstructionSetExecuted method.
     */
    virtual void handleModelUpdated()
     { }
    
    /**
     * A method to satisfy the LC-3 machine's thirst for input characters.
     * Whenever the LC-3 machine needs input, this method will be called.
     *
     * @return EOF on error, 0 on no data available, the character on
     *         success
     */
    virtual int readChar()
     { return EOF; }
    /**
     * A method to satisfy the LC-3 machine's need to print out characters.
     * @param c - character to print
     * @return EOF on error, >=0 on success
     */
    virtual int writeChar(int c)
     { return EOF; }
    
};

#endif
