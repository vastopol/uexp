#ifndef INPUTBUFFER_H
#define INPUTBUFFER_H

#include "common.h"

class InputBuffer
{
 public:
    InputBuffer() { }
    virtual ~InputBuffer() { }
    
    virtual String inputText() const = 0;
    virtual void setInputText(const char* newText) = 0;
};

#endif

