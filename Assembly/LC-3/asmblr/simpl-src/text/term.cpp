#include "term.h"
#include <cstring>
#include <cstdlib>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <ctype.h>

Term Term::in(stdin);
Term Term::out(stdout);
Term Term::err(stderr);

Term::Term(FILE *termStream)
 : stream(termStream),
   fd(fileno(stream)),
   rawCount(0),
   state(S_UNKNOWN)
{
    useColor = isatty(fd);
}


void Term::enterRaw()
{
    if (rawCount == 0) doRaw();
    rawCount++;
}

void Term::exitRaw()
{
    rawCount--;
    if (rawCount == 0) doUnraw();
}

void Term::doRaw()
{
#ifdef HAVE_TERM
    struct termios tio;
    if (state != S_FAIL && isatty(fd) && tcgetattr(fd, &tio) == 0)
    {
        old_tio = tio;
        tio.c_lflag &= ~(ICANON|ECHO);
        tio.c_cc[VMIN] = 1;
        tio.c_cc[VTIME] = 0;
        (void) tcsetattr(fd, TCSANOW, &tio);
        state = S_RAW;
    }
    else
    {
        state = S_FAIL;
    }
#endif
}

void Term::doUnraw()
{
#ifdef HAVE_TERM
    if (state == S_RAW)
    {
        (void) tcsetattr(0, TCSANOW, &old_tio);
        state = S_GOOD;
    }
#endif
}

char *Term::appendColor(char *buf, const char *s) const
{
    if (useColor)
        return buf + sprintf(buf, "[%sm", s);
    else
        return buf;
}

void Term::printLines(const char * const *s)
{
    for (; *s; s++)
    {
        fputs(*s, stream);
        putc('\n', stream);
    }
}

