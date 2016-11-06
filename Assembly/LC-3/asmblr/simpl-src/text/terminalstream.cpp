
#include "terminalstream.h"
#include "term.h"

#ifndef NO_READLINE
#define USE_READLINE
#endif

#ifdef USE_READLINE
/* It seems a bunch of systems are missing the headers but will still link
 * with readline.  This is annoying me so I'm going to include the
 * prototypes and hope things work.
 */
extern "C" {
    void add_history(const char *history);
    char *readline(const char *prompt);
    void rl_insert(int, int);
    void rl_bind_key(int key, void (*fn)(int, int));
};
#else
void add_history(const char *UNUSED(s))
{
}
char *readline(const char *prompt)
{
    char buf[256];
#ifdef HAVE_TERM
    int c;
    char *pos = buf;
    
    Term::in.enterRaw();
    printf("%s", prompt);
    fflush(stdout);
    while (1)
    {
        c = fgetc(stdin);
        
        if (c == 27)
        {
            c = fgetc(stdin);
            if (c == '[')
            {
                c = fgetc(stdin);
                if (c == 'D' && pos > buf)
                {
                    c = 127;
                }
                else
                {
                    putchar(7);
                    c = 0;
                }
            }
        }
        
        if (c == '\n' || c == EOF)
            break;
        
        if (c == '\b' || c == 127 || pos > buf + 70)
        {
            if (pos > buf)
            {
                printf("\033[D \033[D");
                pos--;
            }
        }
        else if (c != 0)
        {
            (void) putchar(c);
            *pos++ = c;
        }
    }
    printf("\n");
    *pos = '\0';
    Term::in.exitRaw();
#else
    (void)fgets(buf, sizeof(buf), stdin);
#endif
    return strdup(buf);
}
#endif

TerminalStream::TerminalStream()
 : TextStream(Term::out.getUseColor())
{
}

String TerminalStream::readline()
{
    char *s = ::readline("(sim) ");
    String retval = s ? s : "quit";
    free(s);
    return retval;
}

void TerminalStream::addHistory(const char* string)
{
#ifdef USE_READLINE
    ::add_history(string);
#endif
}

void TerminalStream::fputs(const char* string)
{
    ::fputs(string, stdout);
}

void TerminalStream::flush()
{
    fflush(stdout);
}

String TerminalStream::editRaw(const char* instring)
{
    int c;
    String buf = instring;
    
    Term::in.enterRaw();

    note("Start typing, and press ctrl-D when you are finished.");
    note("Use backspace to delete a character.");
    
    for (unsigned i = 0; i < buf.length(); i++)
        this->echoChar(buf[i]);
    
    while ((c = getc(stdin)) != EOF && c != 4)
    {
        if ((c == '\b' || c == 127))
        {
            if (!buf.empty())
            {
                char del = buf(-1);
                buf.shrink(1);
                if (del == '\n')
                {
                    size_t i;
                    size_t begin;
                    for (i = buf.length(); i-- > 0 && buf[i] != '\n';)
                     { }
                    begin = i;
                    this->fputs("\033[A"); // TODO: upArrow
                    for (i = begin + 1; i < buf.length(); i++)
                    {
                        this->echoChar(buf[i]);
                    }
                    this->fputs(" \b");
                }
                else
                {
                    this->fputs("\b \b");
                }
            }
        }
        else
        {
            buf += c;
            this->echoChar(c);
        }
    }
    
    this->color("0");
    Term::in.exitRaw();
    this->printf("\n");
    
    return buf;
}
