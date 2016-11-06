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

#include "qtui.h"
#include "simplview.h"
#include "simpl.h"
#include "console.h"

#include <qpainter.h>
#include <qfontmetrics.h>
#include <qpen.h>
#include <qlayout.h>
#include <qevent.h>
#include <qmainwindow.h>
#include <qapplication.h>
#include <qevent.h>

// Some of these things are just stuck in ConsoleOutput for little reason...
// I am later planning on moving global defaults and routines somewhere
// else, but I'm not sure where and how.

Ref<QFontMetrics> ConsoleOutput::mMetrics(0);
Ref<QFont> ConsoleOutput::mFont(0);

class WriteKeyEvent
 : public QEvent
{
 public:
    WriteKeyEvent(int inValue)
     : QEvent(QEvent::User),
       value(inValue)
     { }
    
    int getValue()
     { return value; }
    
 private:
    int value;
};

int ConsoleOutput::interpColors(SimplSettings* settings, QPainter *painter,
        int x, int y, const char *s, bool useColors)
{
    QPen pen = painter->pen();
    QBrush originalBrush = painter->brush();
    QBrush background = originalBrush;
    QFontMetrics fontMetrics = painter->fontMetrics();
    int bold = 0;
    int color = -1;
    int baseY = y + fontMetrics.ascent();
    int h = fontMetrics.height();
    
    for (; *s;)
    {
        int w;
        char c = *s;
        QString accum = "";
        
        if (c == 27)
        {
            s += 2;
            for (;; s++)
            {
                if (*s == '1')
                {
                    bold = 1;
                    if (useColors)
                    {
                        painter->setPen(
                            settings->foregroundColor(
                                color, bold));
                    }
                }
                else if (*s == '0')
                {
                    bold = 0;
                    color = 8;
                    if (useColors) painter->setPen(pen);
                    if (useColors) background = painter->brush();
                }
                else if (*s == '3')
                {
                    color = *++s - '0';
                    if (useColors)
                    {
                        painter->setPen(
                            settings->foregroundColor(
                                color, bold));
                    }
                }
                else if (*s == '4')
                {
                    int bgcolor = *++s - '0';
                    if (useColors)
                    {
                        background = QBrush(
                            settings->backgroundColor(bgcolor));
                    }
                }
                else if (*s != ';')
                    break;
            }
            s++;
            continue;
        }
        
        for (; *s != 27 && *s != 0; s++)
            accum += *s;

        w = fontMetrics.width(accum);

        painter->fillRect(x, y, w, h, background);
        painter->drawText(x, baseY, accum);
        
        x += w;
    }
    
    painter->setPen(pen);
    
    return x;
}

ConsoleOutput::ConsoleOutput(QtUI *inUI, Console *parent, const char *name)
 : KTextEdit(parent, name)
 , mUI(inUI)
 , mConsole(parent)
{
    setFont(mUI->fixedFont());
    setReadOnly(true);
    mUI->addAccelsTo(new Q3Accel(this));
    insert("Output from the machine will appear here.  \n"
        "You can specify input in the box below. "
        " Enter text beforehand, or type while"
        " the program is running. "
        " Specify bulk input by pasting.\n"
        " --\n");
    connect(inUI, SIGNAL(updated(bool)), this, SLOT(handleUpdated()));
    connect(&inUI->settings(), SIGNAL(fontChanged()),
        this, SLOT(handleFontChanged()));
}

ConsoleOutput::~ConsoleOutput()
{
}

QSize ConsoleOutput::sizeHint() const
{
    return QSize(300, 200);
}

void ConsoleOutput::writeChar(int c)
{
    mMutex.lock();
    // must have at least: "if c != 0"
    if (isprint(c) || isspace(c)) mQueue += char(c);
    mMutex.unlock();
}

void ConsoleOutput::postWriteChar(int c)
{
    writeChar(c);
}

void ConsoleOutput::handleUpdated()
{
    mMutex.lock();
    const char *s = mQueue;
    if (*s)
    {
#ifndef QT4
        moveCursor(KTextEdit::MoveEnd, false);
        insert(QString(s));
#else
        QTextCursor curs(document());
        curs.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        curs.insertText(QString(s));
        setTextCursor(curs);
#endif
        ensureCursorVisible();
        //fputs(mQueue.c_str(), stdout);
        mQueue = "";
    }
    mMutex.unlock();
}

/*------------------------------------------------------------------------*/

Console::Console(QtUI* inUI, QWidget* parent)
 : QWidget(parent), mReadIndex(0)
{
    QGridLayout *grid = new QGridLayout(this, 2, 1);
    grid->setRowStretch(0, 100);
    grid->setRowStretch(1, 20);
    mOutputWidget = new ConsoleOutput(inUI, this);
    grid->addWidget(mOutputWidget, 0, 0);
    mEdit = new KTextEdit(this);
    grid->addWidget(mEdit, 1, 0);
    connect(mEdit, SIGNAL(textChanged()), this, SIGNAL(charTyped()));
    mEdit->setFocus();
}

Console::~Console()
{
}

bool Console::hasInput() const
{
    /* Todo: This should be locked somehow... although returning 0
      is probably acceptable given that 0 means 'no data ready'. */
    mMutex.lock();
    bool result = int(mEdit->text().length()) > mReadIndex;
    mMutex.unlock();
    return result;
}

bool Console::event(QEvent* e)
{
    UpdateEvent *ue = dynamic_cast<UpdateEvent*>(e);
    
    // Process an event delivered.
    // Whenever a character is read, an event is posted to remove the text
    // from the UI.  However, instead of removing one character at a time,
    // an index is kept track.  This way, unnecessary redraws are avoided.
    
    if (ue)
    {
        mMutex.lock();
        if (mReadIndex != 0)
        {
            // The underlying machine has eaten some characters.
            mEdit->setText(mEdit->text().right(
                mEdit->text().length()-mReadIndex));
            mReadIndex = 0;
        }
        else if (mNextInput.length() != 0)
        {
            // The 'infile' command needed to change the queued input text.
            mEdit->setText(mNextInput.c_str());
            mNextInput = "";
        }
        mMutex.unlock();
        return true;
    }
    else
    {
        return QWidget::event(e);
    }
}

int Console::pullInputChar()
{
    mMutex.lock();
    
    QString str = mEdit->text();
    int c = 0;
    
    // Pull an input character off the queue.
    // This will simply update the index of the last read character
    // and post an update event, so that unnecessary redraws are avoided.
    
    if (int(str.length()) > mReadIndex)
    {
        c = mEdit->text()[mReadIndex].latin1();
        mReadIndex++;
        if (c == 13) c = 10;
        if (mReadIndex == 1)
            QApplication::postEvent(this, new UpdateEvent());
    }
    mMutex.unlock();
    
    return c;
}

void Console::addInputChar(char c)
{
    // Add a character to the input.
    mMutex.lock();
    mEdit->setText(mEdit->text() + c);
    mMutex.unlock();
}

String Console::inputText() const
{
    mMutex.lock();
    String result = mEdit->text().latin1();
    mMutex.unlock();

    return result;
}

void Console::setInputText(const char* newText)
{
    mMutex.lock();
    mNextInput = newText;
    mMutex.unlock();
    QApplication::postEvent(this, new UpdateEvent());
}

/*------------------------------------------------------------------------*/

ConsoleWin::ConsoleWin(QtUI* inUI, QWidget *parent)
 : QMainWindow(parent)
{
    mConsole = new Console(inUI, this);
    setCentralWidget(mConsole);
    setCaption(QString(inUI->model().archName()) + " Console");
}

ConsoleWin::~ConsoleWin()
{ }

void ConsoleWin::closeEvent(QCloseEvent *e)
{
    e->ignore();
}

QSize ConsoleWin::sizeHint() const
{
    return QSize(400, 200);
}

#include "console.moc"

