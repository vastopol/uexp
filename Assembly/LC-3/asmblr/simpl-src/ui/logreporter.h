#include "reporter.h"
#ifndef LOGREPORTER_H
#define LOGREPORTER_H

/**
 * A LogReporter is a Reporter implementation that queues up error messages.
 */
class LogReporter
 : public Reporter
{
 public:
    LogReporter();
    
    /**
     * Display a message that is error or warning or notice.
     * @param type - the type
     * @param s - string to display
     */
    void msg(MsgType UNUSED(type), const char *UNUSED(s));
    
    bool hasMessages() const
     { return mMessages.size() != 0; }
    bool hasErrors() const
     { return mWorstType >= Reporter::Error; }
    Reporter::MsgType worstType() const
     { return mWorstType; }
    
 protected:
    Array<String> mMessages;
    Array<Reporter::MsgType> mMsgTypes;
    Reporter::MsgType mWorstType;
};

#endif
