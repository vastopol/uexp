
#ifndef TEXTSTREAM_H
#define TEXTSTREAM_H

#include "ui/reporter.h"

/**
 * Abstract class to represent an output stream, with minimal input features.
 *
 * This is the hook used by CommandUI to provide command-based UI's for both
 * TextUI and the Qt TextWindow.
 */
class TextStream
 : public Reporter
{
 public:
    /**
     * Creates a text stream with the specified color mode.
     */
    TextStream(bool inUseColor = false)
     : mUseColor(inUseColor)
     { }

    /**
     * Reads a line of input.
     *
     * TODO: This method is only used in TextUI, so perhaps
     * this method should be moved.
     *
     * @return a string just read from stdin
     */
    virtual String readline() { return ""; }
    /**
     * Adds a line to history, for convenience in readline.
     *
     * TODO: This method is only used in TextUI, so perhaps
     * this method should be moved.
     *
     * @param string a string to add to history
     */
    virtual void addHistory(const char* string) { }
    
    /**
     * Print a string to the output.
     *
     * @param s the string to print to standard out
     */
    virtual void fputs(const char *s) = 0;
    /**
     * Prints a formatted string to output, in printf format.
     *
     * @param format the format string
     */
    void printf(const char *format, ...);
    /**
     * Prints an ANSI color string if color is enabled,
     * otherwise prints nothing.
     *
     * @param colorstr a string such as "0;22"
     */
    void color(const char *colorstr);
    /**
     * Flushes all output.
     */
    virtual void flush()
     { }
    /**
     * Shows a possibly non-printable such that it takes exactly one
     * character block of space, using color when available.
     *
     * @param c the character to print
     */
    void dumpChar(int c);
    /**
     * Echoes a possibly non-printable character back to screen.
     * Characters are not necessarily in exactly one block of space, for
     * instance newlines are echoed.  Colors are used when available.
     *
     * @param c the character to print
     */
    void echoChar(int c);

    /**
     * Should ANSI color be used?
     */
    bool useColor() const
     { return mUseColor; }
    /**
     * Sets whether ANSI colors should be used.
     */
    void setUseColor(bool inUseColor)
     { mUseColor = inUseColor; }
    /**
     * Prints a sequence of character strings one per line.
     *
     * @param lines an array of character pointers, terminated by a NULL
     *        character pointer
     */
    void printLines(const char * const * lines);
    /**
     * Echoes a character-7 beep.
     */
    void beep()
     { this->fputs("\007"); }
    /**
     * Sends an escape sequence.
     * @param s what should come after the escape character
     */
    void sendEscape(const char* s)
     { this->printf("\033%s", s); }
    /**
     * Handles editing a string with possibly unprintable characters.
     *
     * The default implementation produces an error message saying the
     * operation is not available, and returns the input string.
     *
     * @param s the original string
     * @return the edited string
     */
    virtual String editRaw(const char* s)
     { err("Operation not available in this type of UI"); return s; }

 public:
    /**
     * Prints an error, warning, or notice, which is already formatted.
     *
     * This default implementation prints a short prefix depending on the
     * type, and then the string.
     *
     * @param type whether it is an error, warning, or notice
     * @param str the message to print
     */
    virtual void msg(Reporter::MsgType type, const char* str);

 private:
    /** Whether ANSI color mode is enabled. */
    bool mUseColor;
};

#endif
