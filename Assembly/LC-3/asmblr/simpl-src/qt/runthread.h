#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include <qthread.h>
#include <qmutex.h>
#include <qwaitcondition.h>
#include "model/model.h"
#include "ui/commandui.h"
#include "qtui.h"

class QtUI;

class ConsoleWidget;

/**
 * The RunThread represents the thread in the background that makes
 * blocking IO not freeze the GUI.
 *
 * This thread operates somewhat as a state machine.  Communication is very
 * limited.  To initiaate running, use "exec" with one of the types.
 */
class RunThread :
#ifndef QT4
    public QObject,
#endif
    public QThread
{
 Q_OBJECT
 public:
    /**
     * This is used to represent the current state, ie, mShouldRun.
     */
    enum RunType
    {
        Idle=0,  //< Idle state (waiting).
        Command, //< Execute a command
        Die      //< Stop execution
    };
 public:
    /**
     * Create a new RunThread.
     * @param inUI - the UI box that is communicating with the rest of the
     * program
     */
    RunThread(QtUI* inUI);
    /**
     * Destructor; call only after die() and wait().
     */
    ~RunThread();
 public:
    /**
     * Initiate execution.
     * @param type - whether to perform a next(), step(), finish(), or run()
     */
    void exec(RunType type)
     { mShouldRun = type; mRunCond.wakeOne(); }
    /**
     * Runs a command.
     * @param command - a user-specified command
     */
    void execLine(const char* command)
     { mShouldRun = Command; mCommand = command; mRunCond.wakeOne(); }
    /**
     * Stop simulation.
     * This will not return until the model is successfully stopped.
     */
    void halt();
    /**
     * Read one character from the console widget.
     * This must be called from the running thread.
     * @return character code if input ok, -1 if error
     */
    int readChar();
    /**
     * Is there any current execution?
     * @return true if still executing
     */
    bool isExecuting()
     { return mShouldRun != Idle; }
    /**
     * Shut down the simulation loop.
     * Follow this with a wait() and you can happily destruct the thread.
     */
    void die()
     { mShouldRun = Die; mModel->halt(); mRunCond.wakeOne(); }
    
    /**
     * Sets the handler to be called to parse commands.
     */
    void setCommandUI(CommandUI* commandUI)
     { mCommandUI = commandUI; }
    /**
     * Gets the CommandUI object.
     */
    const CommandUI& commandUI() const
     { return *mCommandUI; }
    
 public slots:
    /**
     * Accept input from the console widget.
     */
    void handleInput();
    
 protected:
    /**
     * This method is the internal loop for the thread.
     */
    void run();
    
 private:
    void waitForSomething()
     { mRunCondMutex.lock();
       mRunCond.wait(&mRunCondMutex);
       mRunCondMutex.unlock(); }
    
 private:
    /** The simulator mModel. */
    Model* mModel;
    /** The UI middleman. */
    QtUI* mUI;
    /** Wait condition to use to notify of events. */
    QWaitCondition mRunCond;
    /** Mutex for the run condition */
    QMutex mRunCondMutex;
    /**
     * This is a mutex on the model.
     * Its existence is not completely justified and might be removed.
     */
    QMutex mModelMutex;
    /** Current state. */
    RunType mShouldRun;
    /** Command to execute if mShouldRun is Command. */
    String mCommand;
    /** Command interpreter. */
    CommandUI* mCommandUI;
};

#endif
