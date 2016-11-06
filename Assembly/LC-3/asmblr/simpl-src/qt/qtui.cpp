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
#include "simpl.h"
#include <qevent.h>
#include "console.h"
#include "runthread.h"
#ifdef USE_KDE
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#else
#include <qapplication.h>
#endif
#include <qmessagebox.h>

#ifdef USE_KDE
static const char description[] =
    I18N_NOOP("A generic simulator");

static KCmdLineOptions options[] =
{
    //{ "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};
#endif

static const char version[] = "0.1";

QtUI::QtUI()
 : SimUI(),
   mConsole(0), mTextWindow(0), mRunThread(0), mSettings(0)
{
    mUpdateCount = 0;
}

QtUI::~QtUI()
{
    killThread();
}

void QtUI::killThread()
{
    if (mRunThread)
    {
        mRunThread->die();
        mRunThread->wait();
        delete mRunThread;
        mRunThread = 0;
    }
}

void QtUI::msg(MsgType type, const char *s)
{
    
    if (mTextWindow)
    {
        qApp->lock();
        mTextWindow->msg(type, s);
        if (type == Warning)
        {
            emit warning();
        }
        else if (type == Error)
        {
            showTextWindow();
        }
        qApp->unlock();
    }
    else
    {
        const char *pref;
        
        if (type == Note) pref = "[.] ";
        else if (type == Warning) pref = "[!] ";
        else pref = "[X] ";

        printf("%s%s\n", pref, s);
    }
}

/* todo: make this not an ugly hack */
void QtUI::parseArgs(int &argc, char **argv)
{
#ifdef USE_KDE
    KAboutData *about =
      new KAboutData("simpl", I18N_NOOP("Simpl"), version, description,
                     KAboutData::License_BSD, "(C) %{YEAR} Garrett Franklin Boyer", 0,
                     0, "garryb@cc.gatech.edu");
    about->addAuthor( "Garrett Franklin Boyer", 0, "garryb@cc.gatech.edu" );
    KCmdLineArgs::init(argc, argv, about);
    KCmdLineArgs::addCmdLineOptions( options );
    mApp = new KApplication();
#else
    mApp = new QApplication(argc, argv);
#endif
}

void QtUI::install()
{    
    mMainWin = 0;

    mSettings = new SimplSettings(this);

    mRunThread = new RunThread(this);
    mRunThread->start();
    
    mMainWin = new SimplWin(this);
    mApp->setMainWidget( mMainWin );
    
    ConsoleWin *mConsoleWin = new ConsoleWin(this, 0);
    mConsole = mConsoleWin->console();
    mConsoleWin->show();
    mMainWin->show();
    
    mTextWindow = new TextWindow(0, this, mRunThread, mConsole);
    
    connect(mConsole, SIGNAL(charTyped()),
        mRunThread, SLOT(handleInput()));
}

void QtUI::loop()
{
    mModel->save();
    (void) mApp->exec();
}

void QtUI::uninstall()
{
    killThread();
}

void QtUI::pauseExec()
{
    mRunThread->halt();
}

bool QtUI::isExecuting() const
{
    return mRunThread->isExecuting();
}

void QtUI::handleInstructionExecuted()
{
    if (mModel->totalCount() % 16384 == 0
        && lastUpdate.msecsTo(QTime::currentTime()) > 120)
    {
        handleModelUpdated();
        lastUpdate = QTime::currentTime();
    }
}

void QtUI::handleInstructionSetExecuted()
{
}

void QtUI::scheduleUpdate()
{
    mUpdateCount--;
    handleModelUpdated();
}

void QtUI::handleModelUpdated()
{
    QApplication::postEvent(this, new UpdateEvent());
}

bool QtUI::event(QEvent *e)
{
    UpdateEvent *ue = dynamic_cast<UpdateEvent*>(e);
    //printf("Got an event\n");
    if (ue)
    {
        if (mUpdateCount != mModel->updateCount())
        {
            performGlobalUpdateNow(false);
            //printf(" - Updating %d\n", mUpdateCount);
        }
        else
        {
            //printf(" - Not Updating %d\n", mUpdateCount);
        }
        return true;
    }
    else
    {
        //printf("Wrong type\n");
        return QObject::event(e);
    }
}

void QtUI::performGlobalUpdateNow(bool noCache)
{
    mUpdateCount = mModel->updateCount();
    updated(noCache);
}

int QtUI::readChar()
{
    scheduleUpdate();
    int c = mRunThread->readChar();
    return c;
}

int QtUI::writeChar(int c)
{
    if (mConsole) mConsole->postWriteChar(c);
    return 0;
}

void QtUI::step()
{
    if (isExecuting())
        pauseExec();
    mRunThread->execLine("step");
}

void QtUI::next()
{
    if (isExecuting())
        pauseExec();
    mRunThread->execLine("next");
}

void QtUI::run()
{
    if (isExecuting())
    {
        // button becomes "Pause" while running
        pauseExec();
    }
    else
    {
        mRunThread->execLine("run");
    }
}

void QtUI::finish()
{
    if (isExecuting()) return;
    mRunThread->execLine("finish");
}

void QtUI::save()
{
    mModel->save();
}

void QtUI::restore()
{
    if (isExecuting())
    {
        pauseExec();
    }
    mModel->restore();
}

void QtUI::reinit()
{
    if (isExecuting()) return;
    mModel->reinit();
}

void QtUI::randomize()
{
    if (isExecuting()) return;
    mModel->randomize();
}

void QtUI::openSplit()
{
    (new MemWin(this, 0, LocID::MemInstr))->show();
}

void QtUI::openStack()
{
    MemWin *memWin = new MemWin(this, 0, LocID::MemWord);
    memWin->setTrack(
        mModel->formatDest(mModel->stackPointerLocation()).c_str());
    memWin->show();
}

void QtUI::showTextWindow()
{
    mTextWindow->show();
}

void QtUI::hideTextWindow()
{
    mTextWindow->hide();
}

bool QtUI::hasInput() const
{
    return mConsole->hasInput();
}

int QtUI::pullInputChar()
{
    return mConsole->pullInputChar();
}

const QFont& QtUI::fixedFont() const
{
    return mSettings->fixedFont();
}

int QtUI::fixedFontWidth() const
{
    return mSettings->charWidth();
}

void QtUI::addAccelsTo(Q3Accel* accel) const
{
    mSettings->addAccelsTo(accel);
}

#include "qtui.moc"

