#include "runthread.h"
#include "console.h"
#include <qapplication.h>

RunThread::RunThread(QtUI *inUI)
 : mModel(&inUI->model()), mUI(inUI), mShouldRun(Idle)
 { }

RunThread::~RunThread()
 { }


void RunThread::run()
{
    for (;;)
    {
        while (!mShouldRun) waitForSomething();
        
        if (mShouldRun == Die) break;
        
        qApp->lock();
        mUI->handleModelUpdated();
        qApp->unlock();
        
        
        mModelMutex.lock();
        mModel->halt(false);
        
        switch (mShouldRun)
        {
         case Command:
            if (mCommandUI)
            {
                mCommandUI->execLine(mCommand);
            }
            break;
         default:
            printf("Unknown Action\n");
            break;
        }
        mModelMutex.unlock();
        
        // program is exiting - thread must die
        if (mShouldRun == Die) break;
        
        mShouldRun = Idle;
        qApp->lock();
        mUI->scheduleUpdate();
        qApp->unlock();
    }
}

void RunThread::halt()
{
    mModel->halt();
    mRunCond.wakeOne(); // in case it's in I/O
    mModelMutex.lock();
    /* grab and release lock to ensure that the mModel is no longer running */
    mModelMutex.unlock();
}

int RunThread::readChar()
{
    if (mShouldRun)
    {
        while (!mUI->hasInput() && !mModel->isHalted()) waitForSomething();
        return mUI->pullInputChar();
    }
    else
    {
        mUI->err("Internal Error: Reading a character when not running!");
        return EOF;
    }
}

void RunThread::handleInput()
{
    mRunCond.wakeOne();
}

#include "runthread.moc"
