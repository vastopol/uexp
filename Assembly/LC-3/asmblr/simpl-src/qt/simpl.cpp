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


#include "simpl.h"

#include <qlabel.h>

#include <qmessagebox.h>
#include <qtextedit.h>
#include <qmenubar.h>
#include <qevent.h>
#include "qlayout.h"
#include <ctype.h>

#include <qapplication.h>


#include <qsettings.h>

SimplWin::SimplWin(QtUI *inUI)
    : QMainWindow( 0, "Simpl" ),
      mModel(&inUI->model()),
      mUI(inUI),
      mSimplView(new SimplView(mUI, this, "mainView"))
{
    setCentralWidget(mSimplView);
    mSimplView->addMenus(this->menuBar());
    setIcon(QPixmap(INSTALLPATH"/hi16-app-simpl.png"));
    setCaption(QString().sprintf("Simpl %s Simulator Version %s",
        mModel->archName(), VERSION_STRING));
}

SimplWin::~SimplWin()
{

}

TextWindow::TextWindow(QWidget *parent, QtUI* inUI,
    RunThread* inRunThread, Console* inConsole)
 : QMainWindow(parent)
 , mUI(inUI)
 , mRunThread(inRunThread)
 , mCurrentLine("")
{
    QWidget *panel = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(panel, 0, 0);
    layout->setAutoAdd(true);
    
    setCaption("Textual Interface");
    
    mText = new KTextEdit(panel);
    mText->setTextFormat(Qt::LogText);
#ifndef QT4
    mText->setMaxLogLines(1000);
#endif
    mText->append("Welcome!");
    mText->append("Type in the below box to execute text commands.");
    mText->append("Try the 'help' command for options.");
    mText->setFont(inUI->fixedFont());
    mText->setReadOnly(true);
    
    mCommandBox = new QLineEdit(panel);
    
    setCentralWidget(panel);
    
    mCommandUI = new CommandUI(&inUI->model(), this, inConsole);
    
    connect(mCommandBox, SIGNAL(returnPressed()),
        this, SLOT(executeCommand()));
    mCommandBox->setFocus();
    mCommandBox->setText("help");
    mCommandBox->selectAll();
    
    mRunThread->setCommandUI(mCommandUI);
    
    connect(&mUI->settings(), SIGNAL(fontChanged()),
        this, SLOT(handleFontChanged()));
}

void TextWindow::handleFontChanged()
{
    mText->setFont(mUI->fixedFont());
}

void TextWindow::msg(Reporter::MsgType type, const char *str)
{
    const char *color;
    const char *pfx;
    
    switch(type)
    {
     case Reporter::Error: color = "#ff0000"; pfx = "XXX"; break;
     case Reporter::Warning: color = "#ff9900"; pfx = "!!!"; break;
     case Reporter::Note: color = "#000000"; pfx = "..."; break;
     default: abort();
    }
    
    add(
        QString().sprintf("<font color=\"%s\">[%s]</font> %s",
            color, pfx, str));
}
    
void TextWindow::fputs(const char* s)
{
    mMutex.lock();
    
    for (; *s; s++)
    {
        if (isprint(*s) || isspace(*s))
        {
            if (*s == '\n')
            {
                if (mCurrentLine.length() == 0)
                {
                    // HACK: Qt will ignore empty lines in append otherwise.
                    mCurrentLine = " ";
                }
                mUnwrittenLines.push_back(mCurrentLine.c_str());
                QApplication::postEvent(this, new UpdateEvent());
                mCurrentLine = "";
            }
            else if (*s == '&')
            {
                mCurrentLine += "&amp;";
            }
            else if (*s == '<')
            {
                mCurrentLine += "&lt;";
            }
            else if (*s == '>')
            {
                mCurrentLine += "&gt;";
            }
            else
            {
                mCurrentLine += *s;
            }
        }
    }
    mMutex.unlock();
}

bool TextWindow::event(QEvent *e)
{
    if (dynamic_cast<UpdateEvent*>(e))
    {
        writeQueuedText();
        return true;
    }
    else
    {
        return QObject::event(e);
    }
}

void TextWindow::writeQueuedText()
{
    mMutex.lock();
    for (unsigned i = 0; i < mUnwrittenLines.size(); i++)
    {
        mText->append(mUnwrittenLines[i]);
    }
    mUnwrittenLines.clear();
    mText->verticalScrollBar()->setValue(
        mText->verticalScrollBar()->maxValue());
    mMutex.unlock();
}

void TextWindow::executeCommand()
{
    if (!mRunThread->isExecuting())
    {
        add("&lt;&lt; <font color=\"#0000ff\">"
            + mCommandBox->text() + "</font>");
        mRunThread->execLine(mCommandBox->text().latin1());
        mCommandBox->selectAll();
    }
}

void TextWindow::show()
{
    QMainWindow::show();
}

void TextWindow::add(const QString& str)
{
    mMutex.lock();
    mUnwrittenLines.push_back(str);
    QApplication::postEvent(this, new UpdateEvent());
    mMutex.unlock();
}

void TextWindow::closeEvent(QCloseEvent *e)
{
    // Just hide the window
    // TODO: Send a signal saying this window is now closed, so the check
    // box can be updated.
    e->ignore();
    hide();
}

QSize TextWindow::sizeHint() const
{
    return QSize(500, 400);
}

// settings

SimplSettings::SimplSettings(QtUI* mUI)
 : QObject(mUI),
   mUI(mUI)
{
    mSettings = new QSettings();
    mSettings->setPath("simplc", "simplc");
    mSettings->beginGroup("/simpl");

    QColor table[9][3] =
    {
        { QColor(  0,  0,  0), QColor( 40, 40, 40), QColor( 40, 40, 40) },
        { QColor(160,  0,  0), QColor(255,  0,  0), QColor(255,  0,  0) },
        { QColor(  0,160,  0), QColor(  0,255,  0), QColor(  0,255,  0) },
        { QColor(160,128,  0), QColor(255,255,  0), QColor(255,255,  0) },
        { QColor(  0,  0,192), QColor(  0,  0,255), QColor(  0,  0,255) },
        { QColor(160,  0,160), QColor(255,  0,255), QColor(255,  0,255) },
        { QColor(  0,160,160), QColor(  0,255,255), QColor(  0,255,255) },
        { QColor(160,160,160), QColor(255,255,255), QColor(192,192,192) },
        { QColor(  0,  0,  0), QColor(  0,  0,  0), QColor(  0,  0,  0) }
    };
    
    for (int r = 0; r < 9; r++)
    {
       for (int c = 0; c < 3; c++)
       {
           mColorTable[r][c] = table[r][c];
       }
    }

    mFixedFont.setStyleHint(QFont::TypeWriter);
#if defined(Q_OS_MACX)
    mFixedFont.setFamily("Courier CE");
    mFixedFont.setPointSize(12);
#elif defined(Q_OS_WIN32)
    mFixedFont.setFamily("Courier New");
    mFixedFont.setPointSize(9);
#else
    mFixedFont.setFamily("fixed");
    mFixedFont.setPointSize(10);
#endif
    readSettings();
}

SimplSettings::~SimplSettings()
{
    writeSettings();
    delete mSettings;
}

void SimplSettings::setFixedFont(QFont font)
{
    mFixedFont = font;
}

void SimplSettings::readFont(QFont& font, QString path)
{
    QString value;
    int num;
    bool valid;
    QFont tempFont;
    
    if ((value = mSettings->readEntry(path + "/family")).isNull()) return;
    tempFont.setFamily(value);
    
    num = mSettings->readNumEntry(path + "/size", -1, &valid);
    if (!valid) return;
    tempFont.setPointSize(num);
    
    font = tempFont;
}

void SimplSettings::readSettings()
{
    readFont(mFixedFont, "/fonts/fixed");
    readColorSettings();
}

void SimplSettings::writeSettings() const
{
    mSettings->writeEntry("/fonts/fixed/family", mFixedFont.family());
    mSettings->writeEntry("/fonts/fixed/size", mFixedFont.pointSize());
    writeColorSettings();
}

void SimplSettings::readColorSettings()
{
    for (int colorNumber = 0; colorNumber < 9; colorNumber++)
    {
        for (int colorType = 0; colorType < 3; colorType++)
        {
            QString strColor = mSettings->readEntry(
                QString().sprintf("/colors/%d%d", colorType, colorNumber));
            
            if (!strColor.isNull())
            {
                QColor color = QColor(strColor);
                
                if (color.isValid())
                {
                    mColorTable[colorNumber][colorType] = QColor(strColor);
                }
            }
        }
    }
}

void SimplSettings::writeColorSettings() const
{
    for (int colorNumber = 0; colorNumber < 9; colorNumber++)
    {
        for (int colorType = 0; colorType < 3; colorType++)
        {
            mSettings->writeEntry(
                QString().sprintf("/colors/%d%d", colorType, colorNumber),
                mColorTable[colorNumber][colorType].name());
        }
    }
}

void SimplSettings::userChangeFixedFont()
{
#ifdef USE_KDE
    KFontDialog::getFont(mFixedFont, false);
#else
    mFixedFont = QFontDialog::getFont(0, mFixedFont);
#endif
    emit fontChanged();
    writeSettings();
}

void SimplSettings::addAccelsTo(Q3Accel *a)
{
    a->connectItem(a->insertItem(Key_F11), mUI, SLOT(step()));
    a->connectItem(a->insertItem(Key_F10), mUI, SLOT(next()));
    a->connectItem(a->insertItem(Key_F8), mUI, SLOT(run()));
    a->connectItem(a->insertItem(Key_F5), mUI, SLOT(run()));
    a->connectItem(a->insertItem(SHIFT+Key_F5), mUI, SLOT(run()));
    a->connectItem(a->insertItem(SHIFT+Key_F8), mUI, SLOT(finish()));
    a->connectItem(a->insertItem(CTRL+Key_N), mUI, SLOT(openSplit()));
    a->connectItem(a->insertItem(CTRL+Key_S), mUI, SLOT(openStack()));
    a->connectItem(a->insertItem(CTRL+Key_T), mUI, SLOT(showTextWindow()));
    a->connectItem(a->insertItem(SHIFT+Key_F12), mUI, SLOT(restore()));
    a->connectItem(a->insertItem(Key_F12), mUI, SLOT(save()));
}

#include "simpl.moc"
