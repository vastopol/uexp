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
/**
 * @file simplview.h
 *
 * This header file has definitions for most of the classes used in the
 * Simpl Qt user interface.  Although these are a lot of classes for one
 * file, it is for the benefit of compile time not to split this up.
 */


#ifndef SIMPLVIEW_H
#define SIMPLVIEW_H

#include "compat.h"
#include <qwidget.h>
#include <qlineedit.h>
#include <qmainwindow.h>

class QPainter;
class QLabel;
class QPushButton;
class Q3PopupMenu;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class KPopupMenu;
class QCheckBox;
class QMenuBar;

#include "model/model.h"
#include "qtui.h"

/**
 * A widget for entering and accepting text.  Although this is very similar
 * to a text box, the implicit method of "undo" is to tab or click off of
 * the widget.  However, to make it apparent that changes will be lost
 * unless accepted, the widget changes color (blue) to indicate
 * modification.
 */
class AcceptBox : public QLineEdit
{
 Q_OBJECT
 
 public:
    AcceptBox(QWidget *w, QtUI* inUI);
    ~AcceptBox();
    
 signals:
    void tryAccept();
    
 public slots:
    void setAcceptedText(const QString& str);
    void revert();
    
 private slots:
    void setChanged();
    void handleFontChanged();
    void accepted();
    
 private slots:
    void performAcceptance();
 
 private:
    static void setColor(QWidget *w, const QColor& c);
    
 private:
    QString mOldContents;
    QColor mNormalColor;
    QtUI* mUI;
};

/**
 * A ValBox is used to contain a value or expression that may or may
 * not be backed by the model.
 *
 * The user can enter numbers or expressions in any format which is
 * acceptable given the LocID type that this widget backs.
 *
 * When this acts as a number-enterer means that after acceptance, this box
 * will always display the value formatted in the default way.  However,
 * if this is an expression-type box, the expression entered by the user
 * will stay.
 */
class ValBox : public QWidget
{
 Q_OBJECT
 public:
    /**
     * Create a ValBox that is attached to a particular location of the model.
     */
    ValBox(QtUI *inUI, QWidget *parent, int inVal,
        const LocID& inLoc, int inDisplayType);
    /**
     * Create a ValBox that displays a value, where the value is manually
     * changed by an external entity.
     */
    ValBox(QtUI *inUI, QWidget *parent,
        const LocID& inLoc, int inDisplayType);
    /**
     * Create a ValBox that represents an expression that may change in
     * value as the model changes.
     */
    ValBox(QtUI *inUI, QWidget *parent, const QString& newContents,
        const LocID& inLoc);
    ~ValBox();
    
    /**
     * Get the current integer value.
     * For expression boxes, this will parse the current value of the widget.
     */
    GVal value() const;
    /**
     * Set the location that this represents.
     *
     * For attached ValBoxes, this will change what part of the machine
     * this box is backing.  Otherwise, the primary purpose of doing this
     * is to change how the box is formatted and interpreted.
     */
    void setLocation(const LocID& newLoc);
    /**
     * Get the location this is pointing to.
     *
     * If this is not an attached ValBox, it is possible that only the
     * type() is defined, but the actual id() is left intentionally
     * unspecified.
     */
    const LocID& location() const
     { return mLoc; }
    

 signals:
    /**
     * Attach to this signal if you are interested whenever the value changes
     * for any reason (even if it's just reflecting a change in the model).
     */
    void maybeChanged(GVal newValue);
    
 public slots:
    /**
     * Set the value being displayed.
     */
    void setValue(GVal newValue);
    /**
     * Set the value, but force an update.
     */
    void changeValue(GVal newVal);
    /**
     * Set the textual contents.  This is important only for expression-type
     * boxes.
     */
    void setContents(const QString& newContents);
    /**
     * Grab focus.
     */
    void setFocus();
    
 private slots:
    /**
     * Handle when the user presses enter.
     */
    void handleUserUpdated();
    /**
     * Handle when the model changes.  Only used if this is attached
     * (backed by the model).
     */
    void handleModelUpdated();
 
 private:
    void changeContents(const QString& contents);
    void makeBox();
    GVal parse(const QString& contents) const;
    void attach();
    
 private:
    /// The model.
    Model *mModel;
    /// Link to the central UI.
    QtUI *mUI;
    /// The cached value.
    mutable GVal mValue;
    /// The underlying location being represented.
    LocID mLoc;
    /// Whether the text box is attached to the location in mLoc.
    bool mAttached;
    /// Format to use for display.
    int mDisplayType;
    /// Text box contents.
    AcceptBox *mTextBox;
};

// TODO: Can ValBox and ValBox be the same class?
//  --> Yes they can. (The second one used to say ExprBox) 

/**
 * A register widget.
 *
 * This contains a label for the register, a view of it in hex, and also
 * optionally a view of it in symbolic or decimal form.
 */
class RegWidget : public QWidget
{
 Q_OBJECT
 public:
    /**
     * The main constructor.
     * @param inUI - the central QtUI object
     */
    RegWidget(QtUI *inUI, const LocID& loc, QWidget *parent, bool dual);
    ~RegWidget();
    
 private:
    Model* mModel;
    QtUI *mUI;
    LocID mLoc;
    QLabel *mName;
    ValBox *mDataBox;
    ValBox *mFancyBox;
};

/**
 * This panel contains the registers and the buttons used to skip through
 * the machine.
 */
class StatePanel : public QWidget
{
 Q_OBJECT
 public:
    /// Create a StatePanel connected to the given user interface and
    /// belonging to the parent.
    StatePanel(QtUI *inUI, QWidget *parent);
    ~StatePanel();
    
    /// Add all menus associated with the StatePanel to the given menu bar.
    void addMenus(QMenuBar *menuBar);
    
 protected slots:
    /// Handle when the font is changed (right now does nothing)
    void handleFontChanged();
    /// Handle when the model is updated
    void handleModelUpdated();
    
 private slots:
    // Menu-bar methods follow:
    /// Load without reinitialization.
    void loadNoReinit() { load(false, false); }
    /// Reload the last file with reinitialization.
    void reload() { load(true, true); }
    /// Reload without reinitialization.
    void reloadNoReinit() { load(false, true); }
    /// Load with reinitialization.
    void load() { load(true, false); }
    
    /// Save the state to a file.
    void saveFile();
    /// Handle whenever a warning occurs.
    void handleWarning();
    /// Handle when the "show error log" button is pushed.
    void handleErrorPushed();
    
    /// Handle when a menu bar item is pressed to change simulation mode.
    void toggleOption(int id);
    
 private:
    /// Pop up a dialog to load a file.
    void load(bool reinit, bool reloadLast);
    /// Create all the registers.
    void makeRegisters(QWidget* widget);
    
 private:
    QtUI *mUI;
    Model *mModel;
    QPushButton *mStepButton;
    QPushButton *mNextButton;
    QPushButton *mRunButton;
    QPushButton *mFinishButton;
    Q3PopupMenu *mStateMenu;
    QCheckBox *mWarningCheckbox;
    QLabel *mExecTotal;
    bool mNewWarnings;
};

/**
 * Memory list widget.
 *
 * This used to use a list box, but to allow larger memory spaces (say
 * 2^32) this has to do everything manually.
 *
 * Be careful in all math to allow for overflow.  Even adding a small
 * amount to any number could wrap around a GAddr, if the address happens
 * to be 0xffffffff.
 */
class MemList : public QWidget
{
 Q_OBJECT
 
 public:
    MemList(QtUI *inUI, LocID::Type inType, QWidget *parent);
    ~MemList();
    
    GAddr selectedAddr() const
     { return mSelectedAddr; }
    LocID selectedLocation() const
     { return LocID(mLocType, mSelectedAddr); }
    LocID location(GAddr addr) const
     { return LocID(mLocType, addr); }
    LocID::Type type() const
     { return mLocType; }
    GAddr addrIncrement() const
     { return mAddrIncrement; }
    int options() const
     { return mOptions; }
    
    /** The first address displayed. */
    GAddr minAddr() const
     { return mTopAddr; }
    /** The last FULL address displayed. */
    GAddr maxAddr() const
     { return mTopAddr + (fullLineCount() - 1) * addrIncrement(); }

    void addOptionMenuItems(Q3PopupMenu* menu, bool useChecks = false);
    void addTypeMenuItems(Q3PopupMenu* menu, bool useChecks = false);
    
    void setType(LocID::Type type);
    
    QSize sizeHint() const
     { return QSize(20, lineHeight()); }
    
 signals:
    void highlighted();
    void accepted();
    
 public slots:
    void selectAddr(GAddr addr);
    void up();
    void down();
    void toggleBreak();
    void toggleBlack();

 protected slots:
    void handleModelUpdated(bool);
    void handleFontChanged();
    void doPopupMenu();
    void toggleOption(int id);

 protected:
    void rawSetType(LocID::Type type);
    void keyPressEvent(QKeyEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void paintEvent(QPaintEvent *pe);
 
 protected:
    int indexFromAddr(GAddr addr) const
     { return mModel->wrapAddr(addr - mTopAddr) / addrIncrement(); }
    GAddr addrFromY(int y) const
     { return mModel->wrapAddr(mTopAddr + (y / lineHeight()) * addrIncrement()); }
    int yFromAddr(GAddr addr) const
     { return mModel->wrapAddr(addr - mTopAddr) / addrIncrement() * lineHeight(); }
    // no partial lines
    int fullLineCount() const
     { return height() / lineHeight(); }
    int lineCount() const
     { return (height() - 1) / lineHeight() + 1; }
    int lineHeight() const
     { return QFontMetrics(mUI->fixedFont()).height(); }
    int lineWidth() const
     { return width(); }
    bool withinRange(GAddr nonaligned, GAddr aligned) const
     { return nonaligned >= aligned
              && nonaligned <= aligned + addrIncrement() - 1; }
    
    String format(GAddr addr) const;
    bool hasChanged(GAddr addr) const;
    void updateCache(GAddr addr) const;
    int unique(GAddr addr) const;
    void paintTick(QPainter* painter, GAddr addr) const;
    void paint(QPainter* painter, GAddr addr);
    void paint(GAddr addr);
    void paintAll(const QRect& rect, bool lazy = false);
    void selectStatic(GAddr addr);
    void handleMouseCoord(int y);
    void changeViewport(GAddr top, GAddr addr);
    void repaintChangedParts()
#ifdef QT4
     { update(); }
#else
     { paintAll(rect(), true); }
#endif
    
 private:
    Model* mModel;
    QtUI* mUI;
    
    LocID::Type mLocType;
    GAddr mSelectedAddr;
    GAddr mTopAddr;
    
    int mOptions;
    GAddr mAddrIncrement;
    
    mutable Array<int> mLastUnique;
};

/**
 * The thin widget at the top used to change the selected memory value.
 */
class MemChanger : public QWidget
{
 Q_OBJECT
 public:
    MemChanger(QtUI *inUI, QWidget *parent, MemList* inMemList = 0);
    ~MemChanger();
 signals:
    void addressChanged(GAddr addr);
 public slots:
    /// Change the address being followed (will always cause updates).
    void changeAddress(GAddr addr);
    /// Set the contents of the "Track:" widget.
    void setTrack(const QString& follow);
    void focusEdit();
    void centerTracked();
 private slots:
    /**
     * Handles when the tracked address (the text or underyling value) changed.
     */
    void handleTrackMaybeChanged();
    /**
     * Handles when the address changes.
     */
    void handleAddressMaybeChanged();
    /**
     * Handles when the user has entered a symbol.
     */
    void handleSymbolUserChanged();
    /**
     * Handles when the model updates.
     */
    void handleModelUpdated();
 private:
    void doUpdateAddress();
    void doUpdateValue();
    void doUpdateFancy();
    void doUpdateSymbol();
    /**
     * Returns the value type of the memory list.
     */
    LocID::Type valueType()
     { return (mMemList) ? mMemList->type() : LocID::MemWord; }
 private:
    Model *mModel;
    QtUI *mUI;
    ValBox *mTrackBox;
    ValBox *mAddressBox;
    ValBox *mValBox;
    ValBox *mFancyBox;
    AcceptBox *mSymbolBox;
    /** Keep track of old address, so can alert when TrackBox changes. */
    MemList* mMemList;
};

/**
 * Interface for viewing and changing the memory.
 *
 * @see MemList for the list widget
 * @see MemChanger for the address selection and value modificatioin
 */
class MemView : public QWidget
{
 Q_OBJECT
 public:
    /// Constructor.
    MemView(QtUI *inUI, LocID::Type type, QWidget *parent);
    /// Destructor.
    ~MemView();
    /// Override the default size.
    QSize sizeHint() const;
    /// Override the minimum size.
    QSize minimumSizeHint() const;
    /// Set the expression being traced.
    void setTrack(const QString& trackedExpression)
     { mMemChanger->setTrack(trackedExpression); }
    /// Add all relevant menus.
    void addMenus(QMenuBar *menuBar);
 private slots:
    /// Handle when something about the model has updated.
    void handleModelUpdated();
    /// Handle when a new address is highlighted on the list.
    void handleHighlight();
 public slots:
    /// Focus this widget for editing the value.
    void focusEdit() { mMemChanger->focusEdit(); }
 signals:
    /// Signal emitted when the user has selected a new memory address.
    void selectionChanged(GAddr);
 private:
    QtUI *mUI;
    Model *mModel;
    MemChanger *mMemChanger;
    MemList *mMemList;
};

/**
 * Split memory-view window.
 *
 * You may instantiate as many of these as you like.
 */
class MemWin : public QMainWindow
{
 Q_OBJECT
 public:
    /**
     * Construct a memory window.
     * @param ui - the QtUI
     * @param parent - the parent widget
     */
    MemWin(QtUI* ui, QWidget *parent, LocID::Type type);
    
    /**
     * Destructor.
     */
    virtual ~MemWin();
    
 public:
    /// Set the expression being traced.
    void setTrack(const QString& trackedExpression)
     { mMemView->setTrack(trackedExpression); }
    
 private:
    /** Pointer to Qt UI */
    QtUI *mUI;
    /** Pointer to the memory view */
    MemView *mMemView;
};

/**
 * This is the main GUI view.
 *
 * @see MemView for the memory viewing and modification widget
 * @see StatePanel for the panel with registers
 * @author Garrett Franklin Boyer
 */
class SimplView : public QWidget
{
 Q_OBJECT
 public:
    SimplView(QtUI *inUI, QWidget *parent = 0, const char *name = 0);
    ~SimplView();
    
    /// Override the widget size.
    QSize sizeHint() const;
    /** Add menus to a window's menu bar. */
    void addMenus(QMenuBar *menuBar);
    
 private:
    Model* mModel;
    QtUI *mUI;
    MemView *mMemView;
    StatePanel *mStatePanel;
};




#endif
