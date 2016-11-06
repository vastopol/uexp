/*
Copyright (c) 2005, Garrett Franklin Boyer
 All rights reserved.
 
 
 Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
Neither the name of the Georgia Institute of Technology nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CONSOLE_H
#define CONSOLE_H

#include "common.h"
#include "compat.h"
#include <qwidget.h>
#include <qfont.h>
#include <qmainwindow.h>
#include <qthread.h>
#include <qmutex.h>
#include <qwaitcondition.h>
#include <qtextedit.h>
#include <ui/inputbuffer.h>

class QtUI;
class Console;

/**
 * This is the widget that displays the output of the console.
 */
class ConsoleOutput : public KTextEdit
{
 Q_OBJECT
 public:
    ConsoleOutput(QtUI* inUI, Console* parent, const char *name = 0);
    virtual ~ConsoleOutput();
    
 public slots:
    void writeChar(int c);
    void postWriteChar(int c);

 private slots:
    void handleFontChanged()
     { setFont(mUI->fixedFont()); }
    void handleUpdated();
    
 public:
    /**
     * Interpret ANSI color sequences in the string.
     * @param painter - painter to draw to
     * @param x - initial x coordinate on left
     * @param y - initial y coordinate (at bottom of text, between ascent
     *        and descent)
     * @param useColors - whether to actually use colors
     * @return the new x position
     */
    static int interpColors(SimplSettings* settings, QPainter *painter,
        int x, int y, const char *s, bool useColors = true);
    /**
     * Size hint!
     */
    QSize sizeHint() const;
    
 private:
    static Ref<QFont> mFont;
    static Ref<QFontMetrics> mMetrics;
    
 private:
    String mQueue;
    QMutex mMutex;
    QtUI *mUI;
    Console* mConsole;
};

/**
 * This widget contains the entire contents for the console display, both
 * the input and output.
 */
class Console : public QWidget, public InputBuffer
{
 Q_OBJECT
 public:
    Console(QtUI* inUI, QWidget* parent = 0);
    ~Console();
    
 public:
    bool hasInput() const;
    int pullInputChar();
    void addInputChar(char c);

    String inputText() const;
    void setInputText(const char* newText);

    bool event(QEvent* e);
    
 public slots:
    void writeChar(int c)
     { mOutputWidget->writeChar(c); }
    void postWriteChar(int c)
     { mOutputWidget->postWriteChar(c); }
 
 signals:
    void charTyped();
    
 private:
    /// The widget used to display output.
    ConsoleOutput* mOutputWidget;
    /// The edit box for pre-specified input.
    KTextEdit* mEdit;
    /** Instead of immediately changing the text box, mReadIndex is set
     * to the number of characters the simulator has pulled from the
     * beginning.  An UpdateEvent is called, which will update the text box.
     */
    int mReadIndex;
    /**
     * The 'infile' command (via InputBuffer interface) uses this to set
     * the input buffer asynchronously.  If this string is non-empty, an
     * UpdateEvent signals that mEdit's contents should be updated to this.
     */
    String mNextInput;
    /// Mutex on mReadIndex and mNextInput
    mutable QMutex mMutex;
};

/**
 * This is the window containing the Console widget.
 */
class ConsoleWin : public QMainWindow
{
 public:
    ConsoleWin(QtUI* inUI, QWidget *parent = 0);
    ~ConsoleWin();
    
 public:
    Console* console() { return mConsole; }
    QSize sizeHint() const;
    
 protected:
    void closeEvent(QCloseEvent *e);
 
 private:
    Console* mConsole;
};

#endif /*CONSOLE_H*/
