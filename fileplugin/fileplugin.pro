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

SOURCES += \
	fileplugin.cpp \
	filehttprequesthandler.cpp

HEADERS += fileplugin.h\
	fileplugin_global.h \
	filehttprequesthandler.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/release/ -lhttp-daemon-components
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/debug/ -lhttp-daemon-components
else:unix: LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/ -lhttp-daemon-components

INCLUDEPATH += $$PWD/../../http-daemon-components
DEPENDPATH += $$PWD/../../http-daemon-components

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/release/libhttp-daemon-components.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/debug/libhttp-daemon-components.a
else:unix: PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/libhttp-daemon-components.a
