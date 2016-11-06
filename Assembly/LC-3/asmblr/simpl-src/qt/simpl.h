/*
Copyright (c) 2006, Garrett Franklin Boyer
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

#ifndef SIMPL_H
#define SIMPL_H

#include <qmainwindow.h>
#include "simplview.h"
#include "qtui.h"
#include "console.h"
#include "runthread.h"
#include "ui/textstream.h"
#include "ui/commandui.h"

class QSettings;

/**
 * This is the main application window.  It should only have one instance
 * for a single QtUI.
 *
 * It contains the state panel and also the memory view, along with the
 * main menu bars.
 *
 * @see SimplView for the primary GUI code
 * @author Garrett Franklin Boyer <garryb@cc.gatech.edu>
 * @version 0.1
 */
class SimplWin : public QMainWindow
{
 Q_OBJECT
 public:
    /**
     * Construct given the central QtUI.
     */
    SimplWin(QtUI* ui);
    
    /**
     * Destroy.
     */
    virtual ~SimplWin();
    
    /**
     * Get the UI.
     */
    QtUI* ui() { return mUI; }
    
 private:
    /** The simulator model */
    Model* mModel;
    /** The simulator user interface */
    QtUI* mUI;
    /** The main widget */
    SimplView *mSimplView;
};

class QCloseEvent;

/**
 * Warning log window.
 */
class TextWindow
    : public QMainWindow
    , public TextStream
{
 Q_OBJECT
 
 public:
    /**
     * Create a warn log.
     * @param parent parent window
     * @param inUI - the QtUI object to attach to
     */
    TextWindow(QWidget *parent, QtUI* inUI, RunThread* inRunThread,
        Console* inConsole);
    
 public:

    /**
     * Add a warning to the log.
     * @param str - string to add
     */
    void add(const QString& str);
    
    /**
     * Override the show() function to set the cursor at the end of the log.
     */
    void show();
    
    /**
     * Calculate the size hint.
     * @return suggested size
     */
    QSize sizeHint() const;
    
    /**
     * Output a string, per the TextStream interface.
     * This method is what allows this widget to act as a console.
     */
    void fputs(const char *s);
    
    /**
     * Handles "edit text", which is not supported in this mode.
     */
    virtual String editRaw(const char *s)
     { err("Please use the console window instead."); return s; }
    
    /**
     * The CommandUI interface that the RunThread should send events to.
     */
    CommandUI* commandUI() { return mCommandUI; }
    
    /**
     * Print an error message.
     */
    void msg(Reporter::MsgType type, const char *str);
    
 protected:
    /**
     * Override the close event.
     */
    void closeEvent(QCloseEvent *e);
    
    /**
     * Handle an event.
     */
    bool event(QEvent *e);
    
    /**
     * Write out all queued-up text.
     */
    void writeQueuedText();

 protected slots:
    /**
     * Execute a command.
     */
    void executeCommand();
    
    /**
     * Handle when the fixed-pitch font is changed.
     */
    void handleFontChanged();
 
 private:
    QtUI* mUI;
    /**
     * The text of the log.
     */
    QTextEdit* mText;
    /**
     * The command line to edit.
     */
    QLineEdit* mCommandBox;
    /**
     * Command interface.
     */
    Ref<CommandUI> mCommandUI;
    /**
     * The thread used to execute commands.
     */
    RunThread* mRunThread;
    /**
     * Current line of text.
     */
    String mCurrentLine;
    /**
     * Buffer of unwritten lines.
     */
    Array<QString> mUnwrittenLines;
    /**
     * Mutex for accessing the buffer.
     */
    QMutex mMutex;
};

/**
 * Class to store the settings for the Simpl applicationo.
 */
class SimplSettings
 : public QObject
{
 Q_OBJECT
 
 public:
    SimplSettings(QtUI* ui);
    ~SimplSettings();
    
    QFontMetrics fixedFontMetrics() const
     { return QFontMetrics(mFixedFont); }
    const QFont& fixedFont() const
     { return mFixedFont; }
    void setFixedFont(QFont font);
    int charWidth(int numChars = 1) const
     { return fixedFontMetrics().width('_') * numChars; }
    
    QColor foregroundColor(int num, int bold) const
     { return mColorTable[num][bold]; }
    QColor backgroundColor(int num) const
     { return mColorTable[num][2]; }
    
    /**
     * Add all of the standard simulator-wide accelerator key bindings.
     * This is in order to ensure the user experience is the same, no matter
     * whether the user is typing them in the console window or a split
     * mem view or the main view.
     *
     * @param accel - the QAccel object to add accelerations to
     */
    void addAccelsTo(Q3Accel *accel);
    
 public slots:
    void userChangeFixedFont();
    
 signals:
    void fontChanged();
    
 private:
    static const QColor defaultColorTable;
 
 private:
    void readSettings();
    void writeSettings() const;
    void readColorSettings();
    void writeColorSettings() const;
    void readFont(QFont& font, QString path);
    
 private:
    QFont mFixedFont;
    QColor mColorTable[9][3];
    QSettings* mSettings;
    QtUI* mUI;
};

#endif // _SIMPL_H_
