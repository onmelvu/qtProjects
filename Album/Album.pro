QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    confirmpage.cpp \
    main.cpp \
    mainwindow.cpp \
    picbutton.cpp \
    picshow.cpp \
    proTreeItem.cpp \
    proopenthread.cpp \
    prosetpage.cpp \
    protree.cpp \
    protreethread.cpp \
    protreewidget.cpp \
    removeprodialog.cpp \
    wizard.cpp

HEADERS += \
    confirmpage.h \
    const.h \
    mainwindow.h \
    picbutton.h \
    picshow.h \
    proTreeItem.h \
    proopenthread.h \
    prosetpage.h \
    protree.h \
    protreethread.h \
    protreewidget.h \
    removeprodialog.h \
    wizard.h

FORMS += \
    confirmpage.ui \
    mainwindow.ui \
    picshow.ui \
    prosetpage.ui \
    protree.ui \
    removeprodialog.ui \
    wizard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

RC_ICONS = "icon/bulm.ico"
