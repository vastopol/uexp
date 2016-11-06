
#ifndef TERMINALSTREAM_H
#define TERMINALSTREAM_H

#include "ui/textstream.h"

class TerminalStream
 : public TextStream
{
 public:
    TerminalStream();
    
    String readline();
    void addHistory(const char* string);
    
    void fputs(const char *s);
    void flush();
    
    String editRaw(const char* s);
};

#endif
