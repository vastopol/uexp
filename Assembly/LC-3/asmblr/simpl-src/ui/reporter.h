#include "common.h"

#ifndef REPORTER_H
#define REPORTER_H

class LogReporter;

/**
 * A Reporter is a base class for error reporters.  Many components, such as
 * the assembler, will send error messages to the Reporter, which will then
 * report the error messages in any ways necessary.
 *
 * One subclass of Reporter is LogReporter, which logs all error messages for
 * future display.
 *
 * TODO IDEA: A re-design might later use Reporter for handling of success
 * and failure instead of returning SuccessT or booleans.  This way, a new
 * LogReporter object might be constructed on a "trial path", and if that
 * trial path is determined to be the final path, that LogReporter's
 * messages will be used.
 */
class Reporter
{
 public:
    /**
     * Type of message.  This is sent by the warn(..), err(..), and note(..)
     * to the msg(int type, const char *msg) function.
     */
    enum MsgType
    {
        Note,   //< A status message
        Warning,  //< Warning Message (Used for LC-3 sim errors and octal, etc)
        Error //< Error message
    };
    /**
     * VerbosityLevel level.
     */
    enum VerbosityLevel
    {
        Silent, //< Don't print anything
        NormalVerbosity, //< Print something at the beginning and end of every action
        Verbose //< Print after every instruction
    };
    
 
 public:
    Reporter()
     : mVerbosityLevel(NormalVerbosity)
     , mSubReporter(0)
     { }
    Reporter(Reporter* subReporter)
     : mVerbosityLevel(NormalVerbosity)
     , mSubReporter(subReporter)
     { }
    virtual ~Reporter() { }
    
    /**
     * Create a warning message, using printf-style args.
     * The final message is displayted using msg(MsgType, const char ).
     * @param s - the format string, printf-style
     */
    void warn(const char *s, ...);
    /**
     * Create a notice message, using printf-style args.
     * The final message is displayed using msg(MsgType, const char ).
     * @param s - the format string, printf-style
     */
    void note(const char *s, ...);
    /**
     * Create an error message, using printf-style args.
     * The final message is displayted using msg(MsgType, const char ).
     * @param s - the format string, printf-style
     */
    void err(const char *s, ...);
    
    /**
     * Display a message that is error or warning or notice.
     * @param type - the type of message
     * @param s - string to display
     */
    virtual void msg(MsgType type, const char *s)
     { if (mSubReporter) mSubReporter->msg(type, s); }
    
    /** Get the current verbosity setting. */
    VerbosityLevel verbosityLevel() const
     { if (mSubReporter) return mSubReporter->verbosityLevel();
       else return mVerbosityLevel; }
    /** Change the current verbosity setting. */
    void setVerbosityLevel(VerbosityLevel inv)
     { if (mSubReporter) mSubReporter->setVerbosityLevel(inv);
       else mVerbosityLevel = inv; }
    
    /** Handle a logging repoter */
    void handleLog(const LogReporter& logs) const;
 
 protected:
    /** Change the sub-reporter. */
    void setSubReporter(Reporter* subReporter)
     { mSubReporter = subReporter; }
    
 private:
    VerbosityLevel mVerbosityLevel;
    
 protected:
    Reporter* mSubReporter;
};

#endif
