#-------------------------------------------------
#
# Project created by QtCreator 2011-11-21T15:02:54
#
#-------------------------------------------------

QT       += core gui

TARGET = Qt-Device-Chat
TEMPLATE = app

INCLUDEPATH += ./qserialdevice/qserialdevice/
INCLUDEPATH += ./qserialdevice/qserialdeviceenumerator/

SOURCES += main.cpp\
        mainwindow.cpp \
    serialconnectwindow.cpp

HEADERS  += mainwindow.h \
    serialconnectwindow.h

FORMS    += mainwindow.ui \
    connectwindow.ui

CONFIG(debug, debug|release) {
    LIBS += -lqserialdeviced
} else {
    LIBS += -lqserialdevice
}

win32 {
    LIBS += -lsetupapi -luuid -ladvapi32
}
unix:!macx {
    LIBS += -ludev
}
