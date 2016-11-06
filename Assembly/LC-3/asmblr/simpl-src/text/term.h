#ifndef TERM_H
#define TERM_H

#include "common.h"

#ifndef NO_TERM
#define HAVE_TERM
#endif

#ifdef HAVE_TERM
#include <sys/termios.h>
#endif


class Term
{
    DISALLOW_COPY(Term);
 public:
    static Term in;
    static Term err;
    static Term out;
    
 public:
    Term(FILE *termStream);
    ~Term() { doUnraw(); }
    
    void enterRaw();
    void exitRaw();
    
    bool isRaw() { return state == S_RAW; }
    bool inRaw() { return rawCount != 0; }

    void beep()
     { if (useColor) fputc(7, stream); }
    
    void dumpChar(int c);
    void echoChar(int c);
    char *appendDumpChar(char *s, int v) const;
    
    char *appendColor(char *buf, const char *s) const;
    
    void writeColor(const char *s) const
     { if (useColor) fprintf(stream, "\033[%sm", s); }
    void sendEscape(const char *s = "") const
     { if (useColor) fprintf(stream, "\033[%s", s); }
    
    void setUseColor(bool shouldUseColor)
     { useColor = shouldUseColor; }
    
    void printLines(const char * const *s);
    
    FILE* getStream()
     { return stream; }
    bool getUseColor()
     { return useColor; }
    
 private:
    void doRaw();
    void doUnraw();
    
 private:
    bool useColor;
    FILE *stream;
    int fd;
    int rawCount;
    enum { S_FAIL=-1, S_UNKNOWN, S_GOOD, S_RAW } state;
#ifdef HAVE_TERM
    struct termios old_tio;
#endif
};

#endif
