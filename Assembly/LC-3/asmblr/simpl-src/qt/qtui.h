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

/*
 * The QtUI is the simulator UI interface that the model talks to to get
 * to the GUI, and the GUI talks to in order to accomplish higher-level
 * model tasks (simpler tasks are sent directly to the model).
 *
 * Because of the text/GUI abstraction, the QtUI has somehow evolved into
 * this giant mediator class that talks to everybody.  I made a lot of
 * progress when I had the model send its own updates and keep an update
 * count, so that the MVC paradigm is better followed.
 *
 *
 *
 */

 
#ifndef QTUI_H_INCLUDED
#define QTUI_H_INCLUDED

#include "ui/simui.h"
#include <qobject.h>
#include <qdatetime.h>
#include "compat.h"

class SimplWin;
class TextWindow;
class QtUI;
class QApplication;
class Console;
class SimplSettings;

class RunThread;

/**
 * This very important class coordinates all the GUI actions with the
 * model.
 *
 * You might think of the name as a misnomer.  To the perspective of the
 * core package, this is definitely the "Qt UI" -- but in truth it is the
 * lowest-layer class in the entire qtui package.
 *
 * One may think of this as a controller class.  It is responsible for
 * receiving the update messages of the model and making sure the GUI
 * is not updated too many unnecessary times in rapid succession.
 *
 * It is also responsible for handling warning messages and error messages.
 *
 * This also connects the I/O mechanism of the LC-3 machine to the Console
 * widget.
 *
 * The final responsibility of QtUI is to basically start the whole
 * whole GUI interface for the user to interact with; i.e. a springboard.
 */
class QtUI : public QObject, public SimUI
{
 Q_OBJECT
 
 public:
    /**
     * Create the QtUI.
     * This alone will not create a GUI -- you must call install().
     */
    QtUI();
    /**
     * Destroy the QtUI.
     * This alone will not ensure destruction -- uninstall() must be called
     * first.
     */
    ~QtUI();
    
    /**
     * Print out a message (warning, error, notice).
     * Right now this prints out to screen; hopefully I will get a nice
     * widget for logging errors.
     *
     * TODO: Consider using sub reporters.
     *
     * @param type - Error, Note, or Warning
     * @param str - the string to print
     */
    void msg(MsgType type, const char *str);
    
    /**
     * Read in a character so the model has an input stream.
     * @return the character read, or 0 if interrupted
     */
    int readChar();
    /**
     * Write out a character to the mConsole.
     * @param c - the character to print
     * @return 0 because this operation is always successful
     */
    int writeChar(int c);
    
    /**
     * This initializes the QApplication.
     */
    void parseArgs(int &argc, char **argv);
    /**
     * This shows the GUI.
     */
    void install();
    /**
     * This hands off execution to Qt's event loop.
     */
    void loop();
    /**
     * This removes any things which may need to be removed at the end of
     * the event loop.
     */
    void uninstall();

    /**
     * Send an update because you know something has changed.
     * The operation will be postponed until the next iteration of the event
     * loop, and some unnecessary updates will be cut as the mUpdateCount
     * will be used.
     */
    void handleModelUpdated();
    
    /**
     * Handle a single update during the execution of a sequence of
     * instructions.
     *
     * This will check to see if a certain amount of time has passed since
     * the last update, and if so, refresh the view.
     */
    void handleInstructionExecuted();
    
    /**
     * This handles the update at the end of a run, next, step, etc command.
     *
     * This has little use in QtUI -- the main purpose is for the text UI
     * to print the final state if the state had not already been shown 
     * during execution.
     */
    void handleInstructionSetExecuted();
    
 public:
    // The part starting here is used by the the GUI classes
    
    /**
     * Pause the current execution by the RunThread.
     */
    void pauseExec();
    /**
     * Query whether the RunThread is executing.
     */
    bool isExecuting() const;
    
    /**
     * Get the SimplSettings object for this.
     */
    SimplSettings& settings() const
     { return *mSettings; }
    /**
     * Get the fixed font.
     */
    const QFont& fixedFont() const;
    /**
     * Get the width of the fixed font.
     */
    int fixedFontWidth() const;
    /**
     * Add the accelerators.
     */
    void addAccelsTo(Q3Accel* accel) const;
    
 public slots:
    /** Perform a "step" command */
    void step();
    /** Perform a "next" command */
    void next();
    /** Perform a "run" or "continue" command
     * (passing the execution off to the RunThread).
     */
    void run();
    /** Finish current subroutine. */
    void finish();
    /** Save the current machine state. */
    void save();
    /** Restore the previously saved state. */
    void restore();
    /** Open up a new memory view. */
    void openSplit();
    /** Open up a view of the stack. */
    void openStack();
    /** Open the text interface. */
    void showTextWindow();
    /** Hide the text window. */
    void hideTextWindow();
    /** Reinitialize machine */
    void reinit();
    /** Randomize the machine */
    void randomize();
    
    bool hasInput() const;
    int pullInputChar();
    
 signals:
    // Update Mechanism Follows
    /**
     * This signal is sent out whenever a refresh needs to be done because
     * the model has changed.
     * @param noCache - set to true if a change occured where *everything*
     *                needs to be updated; there are some situations where
     *                a cache might still have invalid data
     */
    void updated(bool noCache);
    
    /**
     * This signal is sent whenever an error is added to the error log.
     */
    void warning();
    
 public slots:
    /**
     * Update the GUI *NOW*.
     * @param noCache - set to true if this change should invalidate the
     *                internal caches
     */
    void performGlobalUpdateNow(bool noCache = false);
    /**
     * Schedule an update for later.
     * This definitely will update the GUI.
     */
    void scheduleUpdate();
    
    // Re-implemented routines and internal data
 protected:
    /**
     * Handle an event (API not affected).
     * Overridden to accept our custom update events.
     * @param e - the event to handle
     */
    bool event(QEvent *e);
    /**
     * Kill the running thread if exists.
     */
    void killThread();
    
 private:
    /** The KApplication for the program. */
    Ref<QApplication> mApp;
    /** The Console widget for displaying text. */
    Console* mConsole;
    /** The main window with both state control and memory view. */
    SimplWin* mMainWin;
    /** Text interface which duals to show error messages. */
    TextWindow* mTextWindow;
    /** The run thread */
    RunThread* mRunThread;
    /** The model's mUpdateCount at time of last refresh */
    int mUpdateCount;
    /**
     * The actual time of the last update, used for tracking by
     * handleInstructionExecuted during a run sequence.
     */
    QTime lastUpdate;
    /** The internal mSettings */
    Ref<SimplSettings> mSettings;
};

#endif/*QTUI_H_INCLUDED*/
