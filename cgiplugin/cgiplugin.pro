#-------------------------------------------------
#
# Project created by QtCreator 2014-02-10T18:59:54
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = cgiplugin
TEMPLATE = lib

DEFINES += CGIPLUGIN_LIBRARY

SOURCES += cgiplugin.cpp \
    cgipluginhttprequesthandler.cpp \
    cgiresponseparser.cpp \
    cgiresponse.cpp

HEADERS += cgiplugin.h\
		cgiplugin_global.h \
    cgipluginhttprequesthandler.h \
    cgiresponseparser.h \
    cgiresponse.h

QMAKE_CXXFLAGS += -std=c++11

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