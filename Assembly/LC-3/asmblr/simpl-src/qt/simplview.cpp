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

// TODO: Label stack frames better in DUMP mode


#include "simplview.h"
#include "simpl.h"
#include "console.h"
#include <qlayout.h>
#include <qtooltip.h>
#include <qcursor.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qdatetime.h>
#include <qapplication.h>
#include <qpalette.h>
#include <qsizepolicy.h>

// TODO:
//  - optimize drawing on Qt 4

MemList::MemList(QtUI *inUI, LocID::Type inType, QWidget *parent)
 : QWidget(parent)
 , mModel(&inUI->model())
 , mUI(inUI)
 , mSelectedAddr(0)
 , mTopAddr(0)
 , mOptions(0) // TODO: Handle options
{
    rawSetType(inType);
    
    const Array<Option>& optionList = mModel->formatOptions();
    
    for (size_t i = 0; i < optionList.size(); i++)
    {
        if (optionList[i].isDefault())
        {
            mOptions |= optionList[i].val();
        }
    }
    
    connect(mUI, SIGNAL(updated(bool)), this, SLOT(handleModelUpdated(bool)));
    connect(&mUI->settings(), SIGNAL(fontChanged()),
        this, SLOT(handleFontChanged()));
    
    // Not in QT 4
    setBackgroundMode(Qt::NoBackground);
    
    setMinimumSize(QSize(20, lineHeight()));
    setSizePolicy(QSizePolicy::MinimumExpanding,
        QSizePolicy::MinimumExpanding);
}

MemList::~MemList()
{
}

void MemList::paintEvent(QPaintEvent* pe)
{
    paintAll(pe->rect());
}

String MemList::format(GAddr addr) const
{
    if (type() == LocID::MemByte)
    {
        return mModel->dumpLine(addr, options(), mSelectedAddr);
    }
    else
    {
        return mModel->formatLine(location(addr), options());
    }
}

void MemList::paintTick(QPainter* painter, GAddr addr) const
{
    static const int lengths[] = { 18, 6, 12, 6 };
    const int numLengths = 4;
    const int distance = 8;
    GAddr tickNum = addr / addrIncrement();
    
    if ((tickNum % distance) == 0)
    {
        // paint rectangular boxes to help the brain to track scrolling
        QColor color = QColor(0, 0, 0);
        painter->setPen(QColor(color));
        painter->drawLine(0, 0, lengths[(tickNum / distance) % numLengths], 0);
        painter->setPen(QColor(0, 0, 0));
    }
}

void MemList::paint(QPainter *painter, GAddr addr)
{
    int w = lineWidth();
    int h = lineHeight();
    int xcol0 = 0;
    int xcol1 = 8;
    int xcol2 = 16;
    QColor color;
    
    addr = mModel->wrapAddr(addr);
    
    String s = format(addr);
	    
    painter->save();
    painter->translate(0,
        lineHeight() * mModel->wrapAddr(addr - mTopAddr) / addrIncrement());
    
    if (addr == mSelectedAddr)
    {
        color = QColor(192, 192, 192);
    }
    else
    {
        color = QColor(255, 255, 255);
    }
    
    painter->setBrush(color);
    painter->fillRect(0, 0, lineWidth(), h, color);
    painter->setFont(mUI->fixedFont()); // WALDO-PAPER
    
    ConsoleOutput::interpColors(&mUI->settings(), painter, 20, 0,
        s.begin(), addr != mSelectedAddr);
    
    paintTick(painter, addr);
    
    if (mLocType == LocID::MemInstr && mModel->isBlack(addr))
    {
        // put a black box if this is blackboxed
        painter->setBrush(QColor(0,0,0));
        painter->drawRect(xcol0,0,xcol1-xcol0,h);
    }
    if (mLocType == LocID::MemInstr && mModel->isBreak(addr))
    {
        // put a red box if it is a breakpoint
        painter->setBrush(QColor(255,0,0));
        painter->drawRect(xcol1,0,xcol2-xcol1,h);
    }
    for (unsigned i = mModel->stackSize(); --i > 0;)
    {
        Model::StackEntry entry = Model::StackEntry(*mModel, i); // good
        
        if (entry.sp == addr+addrIncrement())
        {
            // label stack frames
            painter->drawLine(0, h-1, w, h-1);
            int v = entry.subAddr;
            QString toDisplay = mModel->format(LocID::PC, v).c_str();
            QFontMetrics metrics(mUI->fixedFont());
            painter->drawText(
                w - metrics.width(toDisplay) - 10,
                metrics.ascent(),
                toDisplay);
            break;
        }
        else if (entry.retAddr == addr)
        {
            // label return locations
            QPolygon places;
            places.putPoints(0, 3, xcol1, 0, xcol1, h-1, xcol0, h/2);
            painter->setBrush(QColor(255,0,255));
            painter->setPen(QColor(0,0,0));
            painter->drawPolygon(places);
            break;
        }
        else if (entry.subAddr == addr)
        {
            // label return locations
            QPolygon places;
            places.putPoints(0, 3, xcol0, 0, xcol0, h-1, xcol1, h/2);
            painter->setBrush(QColor(255,0,255));
            painter->setPen(QColor(0,0,0));
            painter->drawPolygon(places);
            break;
        }
    }
    if (mLocType == LocID::MemInstr && withinRange(mModel->pc(), addr))
    {
        // put a green box if this is PC
        QPolygon places;
        places.putPoints(0, 3, xcol1, 0, xcol1, h-1, xcol2, h/2);
        painter->setBrush(QColor(0,255,0));
        painter->setPen(QColor(0,0,0));
        painter->drawPolygon(places);
    }
    
    QRect bound = painter->clipRegion().boundingRect();
    
    if ((bound.width() >= w && bound.height() >= height())
            || (bound.width() == 0 && bound.height() == 0))
        updateCache(addr);
    //fprintf(stderr, "%d %d %d %d\n", bound.width(), w, bound.height(), height());
    
    painter->restore();
}

int MemList::unique(GAddr addr) const
{
    int val = 0;
    
    addr = mModel->wrapAddr(addr);
    
    for (GAddr b = addrIncrement(); b--;)
    {
        val = (val >> 8) ^ (val >> 27);
        val ^= mModel->get(LocID(LocID::MemByte, addr + b));
    }
    
    if (withinRange(mModel->pc(), addr))
    {
        val ^= 0x10000000;
    }
    
    unsigned i = mModel->stackSize();
    unsigned j = (i < 4) ? (0) : (i - 4);
    while (i-- > j)
    {
        Model::StackEntry entry(*mModel, i);
        if (entry.sp == addr+addrIncrement())
        {
            val ^= 0x20000000;
        }
        else if (entry.retAddr == addr)
        {
            val ^= 0x40000000;
        }
    }
    
    if (withinRange(addr, mSelectedAddr))
    {
        val ^= 0x80000000 ^ mSelectedAddr;
    }

    return val;
}

bool MemList::hasChanged(GAddr addr) const
{
    int i = indexFromAddr(addr);
    
    if (i >= 0 && i < int(mLastUnique.size()))
    {
        return mLastUnique[i] != unique(addr);
    }
    else
    {
        return true;
    }
}

void MemList::updateCache(GAddr addr) const
{
    int i = indexFromAddr(addr);
    mLastUnique.resize(lineCount());
    
    //fprintf(stderr, "Trying Updating Cache\n");
    if (i >= 0 && i < int(mLastUnique.size()))
    {
        mLastUnique[i] = unique(addr);
        //fprintf(stderr, "Updating Cache\n");
    }
}

void MemList::paint(GAddr addr)
{
    QPainter painter(this);
    paint(&painter, addr);
}

void MemList::paintAll(const QRect& rect, bool lazy)
{
    // TODO(gboyer): What do we do when lineCount() is larger than the number
    // of lines? (see ./gt8qt in dump-bytes mode)
    QPainter painter(this);
    int rTop = rect.top();
    int rBottom = rect.bottom();
    int lh = lineHeight();
    int i;
    
    // TODO: Move the bitblt code here, trying to detect if the viewport has
    // simply shifted.
    
    //fprintf(stderr, "Paint\n");
    
    for (i = lineCount(); i--;)
    {
        GAddr addr = mModel->wrapAddr(mTopAddr + i * addrIncrement());
        int lTop = lh * i;
        int lBottom = lTop + lh - 1;
        bool canRepaint = rTop <= lBottom && lTop <= rBottom;
        
        // Qt 4 broke lazy painting.  It is making me furious.  I'm giving up.
        if (canRepaint && !(lazy && !hasChanged(addr)))
        {
            paint(&painter, addr);
        }
    }
}

// external actions

void MemList::selectAddr(GAddr addr)
{
    GAddr old = mSelectedAddr;
    
    addr = mModel->wrapAddr(addr);    // wrap-around into the address space
    addr -= (addr % addrIncrement()); // align it to nearest line
    
    if (old != addr)
    {
        GAddr firstFull = minAddr();
        GAddr difference = (fullLineCount() - 1) * addrIncrement();
        GAddr lastFull = minAddr() + difference;
        GAddr safety = addrIncrement() * min(fullLineCount() / 4, 3);
        GAddr newTop;

        // Logic for "smart selection"

        if (addr < firstFull || addr > lastFull)
        {
            newTop = addr - (fullLineCount() / 2) * addrIncrement();
        }
        else if (addr < firstFull + safety)
        {
            newTop = addr - safety;
        }
        else if (addr + safety > lastFull)
        {
            newTop = addr + safety - difference;
        }
        else
        {
            // don't move it
            newTop = firstFull;
        }

        // if by chance it's out of the range, orient to the middle
        if (addr < newTop
            || addr > newTop + difference)
        {
            newTop = addr - addrIncrement() * (fullLineCount() / 2);
        }

        changeViewport(newTop, addr);
        
        emit highlighted();
    }
}

void MemList::changeViewport(GAddr newTop, GAddr addr) {
    GAddr difference = (fullLineCount() - 1) * addrIncrement();
    GAddr oldTop = mTopAddr;
    
    newTop = mModel->wrapAddr(newTop);
    addr = mModel->wrapAddr(addr);

    // make sure it doesn't straddle the boundary
    if (newTop > mModel->lastAddress() - difference)
    {
        if (addr < difference)
        {
            newTop = 0;
        }
        else
        {
            newTop = mModel->lastAddress() + 1 - difference - addrIncrement();
        }
    }
    
    mTopAddr = newTop;
    mSelectedAddr = addr;
    
    // TODO: If paintAll handles viewport movements, relay this code there
    if (newTop == oldTop)
    {
        repaintChangedParts();
    }
    else
    {
        update();
    }
}

void MemList::handleMouseCoord(int y)
{
    GAddr delta = 4 * addrIncrement(); // default scroll amount
    
    if (y < 0)
    {
       changeViewport(minAddr() - delta, minAddr() - delta);
    }
    else if (y > height())
    {
       changeViewport(minAddr() + delta, maxAddr() + delta);
    }
    else
    {
        selectStatic(addrFromY(y));
    }
}

void MemList::selectStatic(GAddr addr)
{
    addr = mModel->wrapAddr(addr);
    mSelectedAddr = addr;
    repaintChangedParts();
    emit highlighted();
}


void MemList::up()
{
    selectAddr(mSelectedAddr - addrIncrement());
}

void MemList::down()
{
    selectAddr(mSelectedAddr + addrIncrement());
}

void MemList::toggleBreak()
{
    mModel->toggleBreak(selectedAddr());
}

void MemList::toggleBlack()
{
    mModel->toggleBlack(selectedAddr());
}

void MemList::rawSetType(LocID::Type type)
{
    mLocType = type;

    if (type == LocID::MemByte)
    {
        mAddrIncrement = mModel->calcDumpBytes();
    }
    else
    {
        mAddrIncrement = mModel->bytesOf(type);
    }
}

void MemList::setType(LocID::Type type)
{
    rawSetType(type);
    selectAddr(mSelectedAddr - (mSelectedAddr % addrIncrement()));
}

// internal events

void MemList::handleModelUpdated(bool force)
{
    if (force) update();
    else repaintChangedParts();
}

void MemList::keyPressEvent(QKeyEvent* event)
{
    bool ignored = false;
    
    if (event->key() == Qt::Key_Return)
    {
        emit accepted();
    }
    else if (event->key() == Qt::Key_Up)
    {
        selectAddr(mSelectedAddr - addrIncrement());
    }
    else if (event->key() == Qt::Key_Down)
    {
        selectAddr(mSelectedAddr + addrIncrement());
    }
    else if (event->key() == Qt::Key_PageUp)
    {
        selectAddr(mSelectedAddr - addrIncrement() * fullLineCount());
    }
    else if (event->key() == Qt::Key_PageDown)
    {
        selectAddr(mSelectedAddr + addrIncrement() * fullLineCount());
    }
    else
    {
        event->ignore();
        ignored = true;
    }
    if (!ignored)
    {
        event->accept();
    }
}

void MemList::wheelEvent(QWheelEvent* event)
{
    int wheelDelta = event->delta();
    int sign;
    
    if (wheelDelta < 0)
    {
       wheelDelta = -wheelDelta;
       sign = -1;
    }
    else
    {
       sign = 1;
    }
    
    int realDelta = -sign * addrIncrement() * ((wheelDelta + 29) / 30);
    GAddr newTop = mTopAddr + realDelta;
    
    changeViewport(newTop, mSelectedAddr);
    
    event->accept();
}

void MemList::mouseDoubleClickEvent(QMouseEvent* event)
{
    handleMouseCoord(event->y());
    setFocus();
    emit accepted();
    event->accept();
}

void MemList::mousePressEvent(QMouseEvent* event)
{
    handleMouseCoord(event->y());
    setFocus();
    event->accept();
}

void MemList::mouseMoveEvent(QMouseEvent* event)
{
    GAddr addr = addrFromY(event->y());

    if (addr != mSelectedAddr)
    {
        handleMouseCoord(event->y());
    }

    event->accept();
}

void MemList::contextMenuEvent(QContextMenuEvent* event)
{    
    selectAddr(addrFromY(event->y()));
    doPopupMenu();
    event->accept();
}

void MemList::handleFontChanged()
{
    handleModelUpdated(true);
}

void MemList::doPopupMenu()
{
    GAddr addr = selectedAddr();
    KPopupMenu* menu = new KPopupMenu(this);
    
    menu->insertTitle(QString().sprintf("Address x%04X", addr));
    menu->insertItem(
        mModel->isBreak(addr) ? "Remove &breakpoint" : "Mark as &breakpoint",
        this, SLOT(toggleBreak()), Key_F9);
    menu->insertItem(
        mModel->isBlack(addr) ? "Un-blackb&ox" : "Blackb&ox (never trace) this routine",
        this, SLOT(toggleBlack()), SHIFT+Key_F9);
    menu->insertSeparator();
    menu->insertItem("Change &value",
        parent()->parent(), SLOT(focusEdit()), Key_Enter);
    
    menu->insertTitle("View memory as...");
    addTypeMenuItems(menu, true);
    
    menu->insertTitle("Format");
    addOptionMenuItems(menu, true);
    
    menu->exec(QCursor::pos());
}

// "customizability"

void MemList::addOptionMenuItems(Q3PopupMenu* menu, bool useChecks)
{
    const Array<Option>& optionList =
        mModel->formatOptions();
    
    if (useChecks) menu->setCheckable(true);
    
    for (size_t i = 0; i < optionList.size(); i++)
    {
        int id = menu->insertItem(QString(optionList[i].name()) + " (toggle)",
            this, SLOT(toggleOption(int)),
            CTRL+SHIFT+UNICODE_ACCEL+optionList[i].accel(), 40 + i);
        if (useChecks)
        {
            menu->setItemChecked(id, (mOptions & optionList[i].val()) != 0);
        }
    }
}

void MemList::addTypeMenuItems(Q3PopupMenu* menu, bool useChecks)
{
    const char* names[] = {
        "View as &instructions",
        "View as &data words",
        "View as data &bytes"
    };
    LocID::Type types[] = { LocID::MemInstr, LocID::MemWord, LocID::MemByte };
    char accels[] = { 'I', 'D', 'B' };
    
    if (useChecks) menu->setCheckable(true);
    
    for (int i = 0; i < 3; i++)
    {
        int id = menu->insertItem(names[i], this,
            SLOT(toggleOption(int)),
            CTRL+SHIFT+UNICODE_ACCEL+accels[i],
            35 + types[i]);
        
        if (mLocType == types[i] && useChecks)
        {
            menu->setItemChecked(id, true);
        }
    }
}

void MemList::toggleOption(int option)
{
    if (option < 35) fprintf(stderr, "MemList::toggleOption called with %d\n!", option);
    
    if (option < 40)
    {
        setType(LocID::Type((option) - 35));
    }
    else
    {
        int flag = mModel->formatOptions()[option - 40].val();
        
        mOptions ^= flag;
        
        handleModelUpdated(true);
    }
}



///////////////////////////////////////////////////////



// Central space for tooltips.

namespace Tips
{
    const char *step =
        "Execute the current instruction. (F11)";
    const char *next =
        "Execute the next instruction, but do not trace\n"
        "into other subroutines (ie, 'step over'). (F10)";
    const char *run =
        "Run until a breakpoint is encountered. (F8)";
    const char *finish =
        "Run until the end of this subroutine.\n"
        "This option may be disabled if the program\n"
        "is not currently in a subroutine. (Shift-F8)";
    const char *formats =
        "Expressions can be specified in multiple formats:\n"
        " Literals: 12 x3AF2 SOME_LABEL 0b10110101 0777 pc '\\n' <register>\n"
        " Operations: add(+) subtract(-) multiply(*) divide(/) negate(-)\n"
        " Dereference: @(someByteAddress) *(someWordAddress) ^(someInstructionAddress)\n";
};

//-------------------------------------------------------------------------
// AcceptBox definition
//-------------------------------------------------------------------------

AcceptBox::AcceptBox(QWidget *parent, QtUI* inUI)
 : QLineEdit(parent)
 , mUI(inUI)
{
    mNormalColor = palette().active().text();
    connect(this, SIGNAL(textChanged(const QString&)),
        this, SLOT(setChanged()));
    connect(this, SIGNAL(returnPressed()),
        this, SLOT(performAcceptance()));
    connect(this, SIGNAL(lostFocus()), this, SLOT(revert()));
    // DEM?  No, it has to be this way (except maybe fontChanged)
    connect(&mUI->settings(), SIGNAL(fontChanged()),
        this, SLOT(handleFontChanged()));
    handleFontChanged();
}

AcceptBox::~AcceptBox()
{
}

void AcceptBox::handleFontChanged()	
{
    setFont(mUI->fixedFont());
}

void AcceptBox::setAcceptedText(const QString& str)
{
    setText(str);
    accepted();
}

void AcceptBox::accepted()
{
    mOldContents = text();
    setColor(this, mNormalColor);
}

void AcceptBox::setChanged()
{
    setColor(this, QColor(0, 0, 255));
}

void AcceptBox::revert()
{
    setAcceptedText(mOldContents);
}

void AcceptBox::setColor(QWidget *w, const QColor& c)
{
    QPalette p = w->palette();
    QColorGroup g = p.active();
    g.setColor(QColorGroup::Text, c);
    p.setActive(g);
    w->setPalette(p);
}

void AcceptBox::performAcceptance()
{
    tryAccept();
    accepted();
}


//-------------------------------------------------------------------------
// ValBox definition
//-------------------------------------------------------------------------

ValBox::ValBox(QtUI *inUI, QWidget *parent, const LocID& inLoc,
    int inDisplayType)
 : QWidget(parent)
 , mModel(&inUI->model())
 , mUI(inUI)
 , mValue(mModel->get(inLoc) - 1)
 , mLoc(inLoc)
 , mAttached(false)
 , mDisplayType(inDisplayType)
{
    makeBox();
    attach();
}

ValBox::ValBox(QtUI *inUI, QWidget *parent,
        const QString& newContents, const LocID& inLoc)
 : QWidget(parent)
 , mModel(&inUI->model())
 , mUI(inUI)
 , mValue(0)
 , mLoc(inLoc)
 , mAttached(false)
 , mDisplayType(-1)
{
    makeBox();
    setContents(newContents);
    attach();
}

ValBox::ValBox(QtUI *inUI, QWidget *parent, int inVal,
        const LocID& inLoc, int inDisplayType)
 : QWidget(parent)
 , mModel(&inUI->model())
 , mUI(inUI)
 , mValue(inVal - 1)
 , mLoc(inLoc)
 , mAttached(false)
 , mDisplayType(inDisplayType)
{
    makeBox();
    changeValue(inVal);
}

void ValBox::makeBox()
{
    QBoxLayout* layout = new QHBoxLayout(this, 0, 0);
    layout->setAutoAdd(true);
    mTextBox = new AcceptBox(this, mUI);
    QToolTip::add(mTextBox, Tips::formats);
    connect(mTextBox, SIGNAL(tryAccept()), this, SLOT(handleUserUpdated()));
}

ValBox::~ValBox()
{
}

void ValBox::attach()
{
    if (!mAttached)
    {
        if (mDisplayType != -1)
        {
            changeValue(mModel->get(mLoc));
        }
        
        connect(mUI, SIGNAL(updated(bool)), this, SLOT(handleModelUpdated()));
        mAttached = true;
    }
}

void ValBox::handleModelUpdated()
{
    if (mDisplayType == -1)
    {
        GVal old = mValue;
        
        if (value() != old)
        {
            emit maybeChanged(mValue);
        }
    }
    else
    {
        GVal newValue = mModel->get(mLoc);
        
        if (mValue != newValue)
        {
            changeValue(mModel->get(mLoc));
        }
    }
}

GVal ValBox::value() const
{
    if (mDisplayType == -1)
    {
        mValue = parse(mTextBox->text());
    }
    
    // TODO(garryb): Potentially return the mAttached value.

    return mValue;
}

void ValBox::setValue(GVal newValue)
{
    if (mValue != newValue)
    {
        if (mAttached && mDisplayType != -1 && mModel->get(mLoc) != newValue)
        {
            mModel->set(mLoc, newValue);
        }
        changeValue(newValue);
    }
}

void ValBox::setContents(const QString& contents)
{
    if (contents != mTextBox->text())
    {
        //printf("New contents %s\n", contents.latin1());
        changeContents(contents);
    }
}

void ValBox::changeValue(GVal newValue)
{
    mValue = newValue;
    
    if (mDisplayType != -1)
    {
        mTextBox->setAcceptedText(
            mModel->format(mLoc, mValue, mDisplayType).c_str());
    }
    
    emit maybeChanged(newValue);
}

void ValBox::handleUserUpdated()
{
    // TODO: Handle invalid expressions (displayType == -1)
    // by not accepting them.
    changeContents(mTextBox->text());
}

void ValBox::changeContents(const QString& contents)
{
    GVal val;
    
    if (!Failed(mModel->parse(contents.latin1(), mLoc, &val)))
    {
        mTextBox->setEdited(false);
        mTextBox->setAcceptedText(contents);
        setValue(val);
    }
    else
    {
        mTextBox->revert();
    }
}

GVal ValBox::parse(const QString& contents) const
{
    GVal val;

    if (Failed(mModel->parse(contents.latin1(), mLoc, &val)))
    {
        val = mValue;
    }
    
    return val;
}

void ValBox::setFocus()
{
    mTextBox->setFocus();
    mTextBox->setSelection(0, mTextBox->text().length());
}

void ValBox::setLocation(const LocID& newLoc)
{
    mLoc = newLoc;
    handleModelUpdated();
}

//-------------------------------------------------------------------------
// MemChanger definition
//-------------------------------------------------------------------------

MemChanger::MemChanger(QtUI *inUI, QWidget* parent, MemList* memList)
 : QWidget(parent),
   mModel(&inUI->model()),
   mUI(inUI),
   mMemList(memList)
{
    GAddr mInitialAddr = mModel->pc();
    QGridLayout *grid = new QGridLayout(this, 1, 8, 2);

    // Button to reposition cursor on tracked value
    QPushButton *trackButton = new QPushButton("Track:", this);
    connect(trackButton, SIGNAL(pressed()), this, SLOT(centerTracked()));
    grid->addWidget(trackButton, 0, 0);
    
    // Text box to specify what to track
    mTrackBox = new ValBox(mUI, this, "pc", LocID::PC);
    grid->addWidget(mTrackBox, 0, 1);
    QToolTip::remove(mTrackBox); // remove the default tooltip
    QToolTip::add(mTrackBox, "You can enter in an expression here, and every\n"
                             "time the state changes, the expression will be\n"
                             "re-evaluated.\n\n"
                             "To track the stack, for instance, you can use\n"
                             "r6 (or $sp or the appropriate register).");

    // Label: "Addr: "
    QLabel *addrLab = new QLabel("-> Addr: ", this);
    addrLab->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    grid->addWidget(addrLab, 0, 2);

    // Text box: Current address
    mAddressBox = new ValBox(mUI, this, mInitialAddr, LocID::PC,
        Format::DisplayHex);
    grid->addWidget(mAddressBox, 0, 3); // TODO: Switch to 4
    addrLab->setBuddy(mAddressBox);
    QToolTip::add(mAddressBox, "Currently highlighted address.");

    // Text box for adding symbols
    mSymbolBox = new AcceptBox(this, mUI);
    grid->addWidget(mSymbolBox, 0, 4); // TODO: Switch to 3
    QToolTip::add(mSymbolBox,
        "Use this box to add a symbol name for the selected address.");

    // Label "Value:"
    QLabel *valLab = new QLabel("-> Value: ", this);
    valLab->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    grid->addWidget(valLab, 0, 5);
    
    // Text box to change value (displayed in hex)
    mValBox = new ValBox(mUI, this,
        LocID(valueType(), mInitialAddr), Format::DisplayHex);
    grid->addWidget(mValBox, 0, 6);
    
    // Text box to change value (displayed in decimal/label format)
    mFancyBox = new ValBox(mUI, this,
        LocID(valueType(), mInitialAddr), Format::DisplayDecSym);
    grid->addWidget(mFancyBox, 0, 7);
    
    // Upkeep of connections, etc.
    
    // Track box is attached (to look at underlying expressions).
    connect(mTrackBox, SIGNAL(maybeChanged(GVal)),
        this, SLOT(handleTrackMaybeChanged()));
    
    // Address box can only be changed by the user or by the functions we
    // expose, so we are fine with using maybeChanged.
    connect(mAddressBox, SIGNAL(maybeChanged(GVal)),
        this, SLOT(handleAddressMaybeChanged()));
    
    // The value box is attached.  We do not care if it changes!
    
    // The fancy box is attached.  We do not care if it changes.
    
    // The symbol box is changed only by the user.
    connect(mSymbolBox, SIGNAL(tryAccept()),
        this, SLOT(handleSymbolUserChanged()));
    
    // Handle updates from the model.
    connect(mUI, SIGNAL(updated(bool)), this, SLOT(handleModelUpdated()));

    for (int i = 0; i < 8; i++)
        grid->setColStretch(i, 20);

    grid->setColStretch(0, 0);
    grid->setColStretch(2, 0);
    grid->setColStretch(5, 0);

    handleModelUpdated();
    mMemList->selectAddr(mInitialAddr);

    connect(mMemList, SIGNAL(accepted()), this, SLOT(focusEdit()));
}

MemChanger::~MemChanger()
 { }

void MemChanger::focusEdit()
{
    mValBox->setFocus();
}

void MemChanger::handleModelUpdated()
{
    doUpdateSymbol();
}

void MemChanger::handleTrackMaybeChanged()
{
    //fprintf(stderr, "Setting address trackmaybechanged\n");
    mAddressBox->setValue(mTrackBox->value());
}

void MemChanger::handleAddressMaybeChanged()
{
    mFancyBox->setLocation(LocID(valueType(), mAddressBox->value()));
    mValBox->setLocation(LocID(valueType(), mAddressBox->value()));
    doUpdateSymbol();
    
    emit addressChanged(mAddressBox->value());
}

void MemChanger::handleSymbolUserChanged()
{
    const char *s = mSymbolBox->text().latin1();

    if (s[0] != '<' && s[0] != '\0')
    {
        mModel->addSym(s, mAddressBox->value());
    }

    mUI->performGlobalUpdateNow(true);
}

void MemChanger::doUpdateSymbol()
{
    GAddr addr = mAddressBox->value();
    
    const char *s = mModel->getname(addr);
    
    mSymbolBox->setAcceptedText(s ? s : "<no label>");
}

void MemChanger::changeAddress(GAddr addr)
{
    //fprintf(stderr, "Setting address changeAddress\n");
    if (addr != mAddressBox->value())
    {
        mAddressBox->setValue(addr);
    }
}

void MemChanger::centerTracked()
{
    //fprintf(stderr, "Setting address centerTracked\n");
    mAddressBox->setValue(mTrackBox->value());
}

void MemChanger::setTrack(const QString& trackExpression)
{
    mTrackBox->setContents(trackExpression);
}

//-------------------------------------------------------------------------
// MemView definition
//-------------------------------------------------------------------------

MemView::MemView(QtUI *inUI, LocID::Type type, QWidget *parent)
 : QWidget(parent, "MemView"),
   mUI(inUI),
   mModel(&inUI->model())
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this, 0, 0);
    mainLayout->setAutoAdd(true);
    
    Q3HBox *memChangeBox = new Q3HBox(this);
    Q3HBox *south = new Q3HBox(this);
    
    mMemList = new MemList(mUI, type, south);
    mMemChanger = new MemChanger(mUI, memChangeBox, mMemList);
    
    connect(mMemList, SIGNAL(highlighted()),
        this, SLOT(handleHighlight()));
    
    connect(mUI, SIGNAL(updated(bool)), this, SLOT(handleModelUpdated()));
    
    Q3Accel *a = new Q3Accel(this);
    
    a->connectItem(a->insertItem(Key_Down), mMemList, SLOT(down()));
    a->connectItem(a->insertItem(Key_Up), mMemList, SLOT(up()));
    a->connectItem(a->insertItem(Key_F9), mMemList, SLOT(toggleBreak()));
    a->connectItem(a->insertItem(SHIFT+Key_F9), mMemList, SLOT(toggleBlack()));
    
    connect(mMemChanger, SIGNAL(addressChanged(GAddr)),
        mMemList, SLOT(selectAddr(GAddr)));
}

MemView::~MemView() { }

void MemView::addMenus(QMenuBar *menu)
{
    Q3PopupMenu *format = new Q3PopupMenu(this);
    
    menu->insertItem("F&ormat", format);
    
    mMemList->addOptionMenuItems(format);
    format->insertSeparator();
    mMemList->addTypeMenuItems(format);
    format->insertSeparator();
    format->insertItem("Change &font...", &mUI->settings(),
        SLOT(userChangeFixedFont()));
}

void MemView::handleHighlight()
{
    GAddr addr = mMemList->selectedAddr();
    
    selectionChanged(addr);
    mMemChanger->changeAddress(addr);
}

void MemView::handleModelUpdated()
{
}

QSize MemView::sizeHint() const
{
    return QSize(mUI->fixedFontWidth()*85,300);
}

QSize MemView::minimumSizeHint() const
{
    return QSize(mUI->fixedFontWidth()*50,
                 QWidget::minimumSizeHint().height());
}

//-------------------------------------------------------------------------
// MemWin definition
//-------------------------------------------------------------------------

MemWin::MemWin(QtUI *inUI, QWidget *parent, LocID::Type type)
 : QMainWindow(parent, "memWin"),
   mUI(inUI),
   mMemView(new MemView(mUI, type, this))
{
    setCentralWidget(mMemView);
    mUI->addAccelsTo(new Q3Accel(this));
    setCaption("Memory View");
}

MemWin::~MemWin()
{
}

//-------------------------------------------------------------------------
// RegWidget definition
//-------------------------------------------------------------------------

RegWidget::RegWidget(QtUI *inUI, const LocID& inLoc, QWidget *parent, bool dual)
 : QWidget(parent),
   mModel(&inUI->model()),
   mUI(inUI),
   mLoc(inLoc)
{
    QGridLayout *grid = new QGridLayout(this, 1, dual ? 3 : 2, 0, 1);
    QString text;
    
    if (mLoc.type() == LocID::Reg)
        text = mModel->formatDest(mLoc);
    else
        text = "PC";
    
    mDataBox = new ValBox(mUI, this, mLoc, Format::DisplayHex);
    grid->addWidget(mDataBox, 0, 1);
    
    mName = new QLabel(mDataBox, text, this, "nameLabel");
    mName->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    grid->addWidget(mName, 0, 0);
    
    if (dual)
    {
        mFancyBox = new ValBox(mUI, this, mLoc, Format::DisplayDecSym);
        grid->addWidget(mFancyBox, 0, 2);
        grid->setColStretch(1, 20);
        grid->setColStretch(2, 25);
    }
    else
    {
        mFancyBox = 0;
        grid->setColStretch(1, 45);
    }
}

RegWidget::~RegWidget()
{
}

//-------------------------------------------------------------------------
// StatePanel definition
//-------------------------------------------------------------------------

StatePanel::StatePanel(QtUI *inUI, QWidget *parent)
 : QWidget(parent, "StatePanel"),
   mUI(inUI),
   mModel(&inUI->model()),
   mNewWarnings(false)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this, 0, 0);
    mainLayout->setAutoAdd(true);
    
    Q3HBox *nsrBar = new Q3HBox(this);
    QWidget *south = new QWidget(this);
    
    MAKE_BUTTON(mStepButton, "&Step", "goto", Tips::step, nsrBar);
    MAKE_BUTTON(mNextButton, "&Next Line", "down", Tips::next, nsrBar);
    MAKE_BUTTON(mRunButton, "&Run", "run", Tips::run, nsrBar);
    MAKE_BUTTON(mFinishButton, "F&inish Routine", "finish", Tips::finish, nsrBar);
    
    makeRegisters(south);
    
    connect(mWarningCheckbox, SIGNAL(clicked()), this, SLOT(handleErrorPushed()));
    connect(mStepButton, SIGNAL(clicked()), mUI, SLOT(step()));
    connect(mNextButton, SIGNAL(clicked()), mUI, SLOT(next()));
    connect(mRunButton, SIGNAL(clicked()), mUI, SLOT(run()));
    connect(mFinishButton, SIGNAL(clicked()), mUI, SLOT(finish()));
    
    connect(mUI, SIGNAL(updated(bool)), this, SLOT(handleModelUpdated()));
    
    connect(mUI, SIGNAL(warning()), this, SLOT(handleWarning()));
    
    handleModelUpdated();
}

StatePanel::~StatePanel() { }

void StatePanel::makeRegisters(QWidget* widget)
{
    String layoutStr = mModel->panelLayout();
    const char *it = layoutStr.begin();
    
    int rows = atoi(it);
    it = strchr(it, ':') + 1;
    int cols = atoi(it);
    it = strchr(it, ':') + 1;

    QGridLayout* grid = new QGridLayout(widget, rows, cols, 2, 2);
    
    int row = 0;
    int col = 0;
    

    while (*it)
    {
        const char *end;
        
        for (end = it; *end && *end != ',' && *end != '|'; end++)
         { }
        
        if (*it == 'p' || isdigit(*it))
        {
            LocID loc;
            
            if (*it == 'p') loc = LocID::PC;
            else loc = LocID(LocID::Reg, atoi(it));
            
            RegWidget* reg = new RegWidget(mUI, loc, widget,
                end[-1] == '+');
            grid->addWidget(reg, row, col);
        }
        else if (*it == 'w')
        {
            // TODO: Better name than "text info"
            mWarningCheckbox = new QCheckBox("Text Window", widget);
            grid->addWidget(mWarningCheckbox, row, col);
        }
        else if (*it == 'e')
        {
            mExecTotal = new QLabel(widget);
            mExecTotal->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
            grid->addWidget(mExecTotal, row, col);
        }
        
        it = end;
        
        if (*it == ',')
        {
            col++;
            it++;
        }
        else if (*it == '|')
        {
            col = 0;
            row++;
            it++;
        }
    }
}

void StatePanel::addMenus(QMenuBar *menu)
{
    Q3PopupMenu* fileMenu = new Q3PopupMenu(this);
    menu->insertItem("&File", fileMenu);
    fileMenu->insertItem("&Load (and reinit)...", this,
        SLOT(load()), CTRL+Key_O);
    fileMenu->insertItem("Load (w/o reinit)...", this,
        SLOT(loadNoReinit()), CTRL+SHIFT+Key_O);
    fileMenu->insertItem("&Reload last file", this,
        SLOT(reload()), CTRL+Key_L);
    fileMenu->insertItem("&Reload last (w/o reinit)", this,
        SLOT(reloadNoReinit()), CTRL+SHIFT+Key_L);
    fileMenu->insertSeparator();
    fileMenu->insertItem("&Save memory as file...", this,
        SLOT(saveFile()));
    fileMenu->insertSeparator();
    fileMenu->insertItem("&Quit", qApp, SLOT(quit()));
    
    mStateMenu = new Q3PopupMenu(this);
    menu->insertItem("S&tate", mStateMenu);
    mStateMenu->setCheckable(true);
    mStateMenu->insertItem("Step", mUI, SLOT(step()), Key_F11);
    mStateMenu->insertItem("Next (step over)", mUI, SLOT(next()), Key_F10);
    mStateMenu->insertItem("Run (continue)", mUI, SLOT(run()), Key_F8);
    mStateMenu->insertItem("Finish subroutine", mUI, SLOT(finish()),
        SHIFT+Key_F8);
    mStateMenu->insertSeparator();
    mStateMenu->insertItem("&Save state (begin fork)",
        mUI, SLOT(save()), Key_F12);
    mStateMenu->insertItem("&Restore (resume at last fork)",
        mUI, SLOT(restore()), SHIFT+Key_F12);
    mStateMenu->insertSeparator();
    mStateMenu->insertItem("Re&initialize", mUI, SLOT(reinit()),
        ALT+SHIFT+Key_0);
    mStateMenu->insertItem("R&andomize", mUI, SLOT(randomize()),
        ALT+SHIFT+Key_R);
    mStateMenu->insertSeparator();
    
    const Array<Option>& optionList = mModel->runOptions();
    
    for (size_t i = 0; i < optionList.size(); i++)
    {
        mStateMenu->insertItem(
            QString(optionList[i].name()) + " (toggle)",
            this, SLOT(toggleOption(int)),
            ALT+SHIFT+UNICODE_ACCEL+(optionList[i].accel()&~0x20), 40 + i);
    }
    
    Q3PopupMenu* viewMenu = new Q3PopupMenu(this);
    menu->insertItem("&Window", viewMenu);
    viewMenu->insertItem("&New memory view",
        mUI, SLOT(openSplit()), CTRL+Key_N);
    viewMenu->insertItem("Show &stack",
        mUI, SLOT(openStack()), CTRL+Key_S);
    viewMenu->insertItem("Show &text window",
        mUI, SLOT(showTextWindow()), CTRL+Key_T);
}

void StatePanel::handleFontChanged()
{
    // TODO: Not smart enough to change font yet.
}

void StatePanel::handleErrorPushed()
{
    mNewWarnings = false;
    mWarningCheckbox->setText("Text Window");
    if (mWarningCheckbox->isChecked())
    {
        mUI->showTextWindow();
    }
    else
    {
        mUI->hideTextWindow();
    }
}

void StatePanel::toggleOption(int id)
{
    const Array<Option>& optionList = mModel->runOptions();
    
    if (id < 40 || id >= int(40 + optionList.size()))
    {
        fprintf(stderr, "Software Warning: Invalid StatePanel::toggleOption(%d)\n", id);
    }
    else
    {
        int flag = optionList[id - 40].val();
        bool newState = !mModel->runMode(flag);
        mModel->setRunMode(flag, newState);
        mStateMenu->setItemChecked(id, newState);
    }
}

void StatePanel::handleWarning()
{
    mNewWarnings = true;
    if (!mWarningCheckbox->isChecked())
        mWarningCheckbox->setText("*Warnings*");
}

void StatePanel::load(bool reinit, bool reloadLast)
{
    if (mUI->isExecuting())
    {
        QMessageBox::warning(this, "Simpl",
            QString("Please pause execution first."));
        return;
    }
    
    QString name;
    
    if (reloadLast && mModel->lastFileLoaded() != "")
    {
        name = mModel->lastFileLoaded().c_str();
    }
    else
    {
    #ifdef USE_KDE
        name =
            KFileDialog::getOpenFileName(
                ":filedir",
                "*.obj *.bin *.hex *.asm *.s *.lc|All Simpl Files (obj,bin,hex,asm,s,lc)\n"
                "*.obj|Object Files (*.obj)\n"
                "*.bin|Binary Files (*.bin)\n"
                "*.hex|Hex Files (*.hex)\n"
                "*.asm *.s|Assembly Files (*.asm, *.s)\n"
                ,
                this, "Select a Program to Load");
    #else
        name =
            QFileDialog::getOpenFileName(
                QString::null,
                "All Simpl Files (*.obj *.bin *.hex *.asm *.s *.lc);;"
                "Object Files (*.obj);;"
                "Binary Files (*.bin);;"
                "DisplayHex Files (*.hex);;"
                "Assembly Files (*.asm *.s)"
                ,
                this);
    #endif
    }
    
    if (!name.isNull())
    {
        if (reinit) mUI->reinit();
        if (mModel->load(name.latin1()) < 0)
        {
            if (mNewWarnings)
            {
            /*
                mUI->hideTextWindow();
                mWarningCheckbox->setChecked(true);
                handleErrorPushed();
                */
            }
            else
            {
                QMessageBox::warning(this, "Simpl",
                    QString("Could not load file ") + name + QString("."));
            }
        }
        else
        {
            mUI->save();
            mUI->performGlobalUpdateNow(true);
        }
    }
}

void StatePanel::saveFile()
{
    // TODO: Specify what range(s) to save.
    // TODO: Ignore blank ranges

    if (mModel->lastAddress() > 1048575)
    {
        QMessageBox::warning(this, "Simpl",
            QString("Sorry, there is too much memory to save.  "
                    "The file would be too large."));
    }
    else
    {
        if (mUI->isExecuting())
        {
            QMessageBox::warning(this, "Simpl",
                QString("Please pause execution before saving."));
            return;
        }
        
        QString name;
        
    #ifdef USE_KDE
        name =
            KFileDialog::getSaveFileName(
                ":filedir",
                "*.obj *.bin *.hex *.asm *.s *.lc"
                    "|All Simpl Files (obj,bin,hex,asm,s,lc)\n"
                "*.obj|Object Files (*.obj)\n"
                "*.bin|Binary Files (*.bin)\n"
                "*.hex|Hex Files (*.hex)\n"
                "*.asm *.s|Assembly Files (*.asm, *.s)\n"
                ,
                this, "Select a File to Save");
    #else
        name =
            QFileDialog::getSaveFileName(
                QString::null,
                "All Simpl Files (*.obj *.bin *.hex *.asm *.s *.lc);;"
                "Object Files (*.obj);;"
                "Binary Files (*.bin);;"
                "DisplayHex Files (*.hex);;"
                "Assembly Files (*.asm *.s)"
                ,
                this);
    #endif
        
        if (!name.isNull())
        {
            mModel->saveFile(name.latin1());
        }
    }
}

void StatePanel::handleModelUpdated()
{
    bool b = !mUI->isExecuting();
    
    mRunButton->setText(b?"&Run":"&Pause");
    
    mStepButton->setEnabled(b);
    mNextButton->setEnabled(b);
    
    QString finishStr;
    
    if (mModel->stackSize() > 1)
    {
        mFinishButton->setEnabled(b);
        finishStr = "&Finish "
            + mModel->format(LocID::PC, mModel->subroutineAddr());
    }
    else
    {
        mFinishButton->setEnabled(false);
        finishStr = "&Finish Routine";
    }
    
    if (finishStr != mFinishButton->text())
    {
        mFinishButton->setText(finishStr);
    }
    
    mExecTotal->setText(QString("Exec: %1").arg(mModel->totalCount()));
}

//-------------------------------------------------------------------------
// SimplView definition
//-------------------------------------------------------------------------

SimplView::SimplView(QtUI *inUI, QWidget *parent, const char *name)
 : QWidget(parent, name),
   mModel(&inUI->model()),
   mUI(inUI)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this, 0, 0);
    mainLayout->setAutoAdd(true);
    
    //new KSeparator(this);
    mMemView = new MemView(mUI, LocID::MemInstr, this);
    mStatePanel = new StatePanel(mUI, this);
}

SimplView::~SimplView() { }

void SimplView::addMenus(QMenuBar *menu)
{
    mStatePanel->addMenus(menu);
    mMemView->addMenus(menu);
}

QSize SimplView::sizeHint() const
{
    return QSize(mUI->fixedFontWidth()*85,500);
}

#include "simplview.moc"

