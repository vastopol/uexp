#include "logreporter.h"

LogReporter::LogReporter()
 : mWorstType(Reporter::Note)
{
}

void LogReporter::msg(Reporter::MsgType type, const char *s)
{
    new(mMessages.make_back())String(s);
    mMsgTypes.push_back(type);
    
    if (type > mWorstType)
    {
        mWorstType = type;
    }
}
