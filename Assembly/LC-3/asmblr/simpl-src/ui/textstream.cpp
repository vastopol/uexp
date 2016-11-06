
#include "textstream.h"
#include <stdarg.h>
#include "format/format.h"

void TextStream::printf(const char *format, ...)
{
    char buf[1024];
    va_list vl;
    va_start(vl, format);
    vsprintf(buf, format, vl);
    this->fputs(buf);
    va_end(vl);
}

void TextStream::color(const char* colorstr)
{
    if (mUseColor) this->printf("\033[%sm", colorstr);
}

void TextStream::msg(Reporter::MsgType type, const char* str)
{
    if (mUseColor)
    {
        if (type == Error)
            this->color("1;31");
        else if (type == Warning)
            this->color("1;33");
        else
            this->color("1;30");
        
        this->printf(" [sim] ");
        this->color("0");
        this->printf("%s\n", str);
    }
    else
    {
        char c;
        
        switch(type)
        {
         case Reporter::Error: c = 'X'; break;
         case Reporter::Warning: c = '!'; break;
         case Reporter::Note: c = '.'; break;
         default: abort();
        }
        
        this->printf("[%c] %s\n", c, str);
    }
}

void TextStream::printLines(const char * const * lines)
{
    for (; *lines; lines++)
    {
        this->printf("%s\n", *lines);
    }
}

void TextStream::dumpChar(int v)
{
    String s;
    Format::dumpChar(mUseColor, &s, v);
    this->fputs(s.c_str());
}

void TextStream::echoChar(int c)
{
    if (mUseColor)
    {
        dumpChar(c);
        if (c == '\n') this->fputs("\n");
    }
    else
    {
        if (c < 32 && c != 10)
            this->printf("%c", c+64);
        else
            this->printf("%c", c);
    }
}
