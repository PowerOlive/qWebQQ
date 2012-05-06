#-------------------------------------------------
#
# Project created by QtCreator 2012-05-04T11:46:30
#
#-------------------------------------------------

QT       += core gui network

TARGET = InsecureQQ
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    util.cpp \
    qq.cpp \
    talkdialog.cpp \
    displaycapcha.cpp \
    relogin.cpp \
    remotelogviewer.cpp \
    preferencedialog.cpp \
    encrypter.cpp \
    editablelabel.cpp

HEADERS  += widget.h \
    util.h \
    qq.h \
    talkdialog.h \
    displaycapcha.h \
    relogin.h \
    remotelogviewer.h \
    preferencedialog.h \
    encrypter.h \
    editablelabel.h

FORMS    += \
    widget.ui \
    talkdialog.ui \
    displaycapcha.ui \
    relogin.ui \
    remotelogviewer.ui \
    preferencedialog.ui \
    editablelabel.ui

RESOURCES += \
    resources.qrc

LIBS += "-lqjson"
