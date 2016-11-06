#ifndef COMPAT_H
#define COMPAT_H

#include "config.h"

#ifndef QT4
 #include <qpointarray.h>
 #define QPolygon QPointArray
 #include <qaccel.h>
 #include <qlistbox.h>
 typedef QListBox Q3ListBox;
 typedef QListBoxItem Q3ListBoxItem;
 #include <qhbox.h>
 #include <qvbox.h>
 #include <qaccel.h>
 #include <qframe.h>
 #include <qpopupmenu.h>

 #define Q3HBox QHBox
 #define Q3VBox QVBox
 #define Q3PopupMenu QPopupMenu
 #define Q3Accel QAccel
#else
 #include <qpolygon.h>
 #include <q3accel.h>
 #define QAccel Q3Accel
 #include <q3listbox.h>
 typedef Q3ListBoxItem QListBoxItem;
 #include <q3hbox.h>
 #include <q3vbox.h>
 #include <q3accel.h>
 #include <q3frame.h>
 #include <q3popupmenu.h>
 using namespace Qt;
#endif

// KDE Compatability
#ifdef USE_KDE
 #include <kfontdialog.h>
 #include <kmenubar.h>
 #include <ktextedit.h>
 #include <kpopupmenu.h>
 #include <kfiledialog.h>
 #include <kpushbutton.h>
 #include <kguiitem.h>
 #define MAKE_BUTTON(but, name, icon, tip, parent) \
    (but=new KPushButton(KGuiItem(name, icon), parent),\
     QToolTip::add(but, tip))
#else
 #define KTextEdit QTextEdit
 #include <qtextedit.h>
 #define KMenuBar QMenuBar
 #include <qfontdialog.h>
 #include <qfiledialog.h>
 #define MAKE_BUTTON(but, name, icon, tip, parent) \
    (but=new QPushButton(name, parent),\
     QToolTip::add(but, tip))
 class KPopupMenu : public Q3PopupMenu
 {
  public:
     KPopupMenu(QWidget *parent) : Q3PopupMenu(parent) { }
     
     void insertTitle(const QString &s)
     { insertSeparator();
       setItemEnabled(insertItem("-> " + s), false); }
 };
#endif

// Event to send to self to refresh
class UpdateEvent
 : public QEvent
{
 // This breaks stuff... Q OBJECT
 public:
    UpdateEvent()
     : QEvent(QEvent::User)
     { }
};

#endif
