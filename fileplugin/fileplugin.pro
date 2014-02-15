#-------------------------------------------------
#
# Project created by QtCreator 2014-02-15T15:38:54
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = fileplugin
TEMPLATE = lib

DEFINES += FILEPLUGIN_LIBRARY

SOURCES += fileplugin.C

HEADERS += fileplugin.h\
        fileplugin_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
