#include "reporter.h"
#include <cstring>
#include <stdarg.h>

void Reporter::err(const char *formatStr, ...)
{
    char buf[1024];
    va_list vl;
    va_start(vl, formatStr);
    vsprintf(buf, formatStr, vl);
    va_end(vl);
    msg(Reporter::Error, buf);
}

void Reporter::warn(const char *formatStr, ...)
{
    char buf[1024];
    if (mVerbosityLevel >= NormalVerbosity)
    {
        va_list vl;
        va_start(vl, formatStr);
        vsprintf(buf, formatStr, vl);
        va_end(vl);
        msg(Reporter::Warning, buf);
    }
}

void Reporter::note(const char *formatStr, ...)
{
    char buf[1024];
    if (mVerbosityLevel >= NormalVerbosity)
    {
        va_list vl;
        va_start(vl, formatStr);
        vsprintf(buf, formatStr, vl);
        va_end(vl);
        msg(Reporter::Note, buf);
    }
}
