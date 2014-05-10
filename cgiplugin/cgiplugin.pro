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
	cgiresponseparser.cpp \
	cgihttprequesthandler.cpp

HEADERS += cgiplugin.h\
	cgiplugin_global.h \
	cgiresponseparser.h \
	cgihttprequesthandler.h

QMAKE_CXXFLAGS += -std=c++11

CONFIG(release, debug|release){
	DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
	message("Release mode: No debug or warning messages from Qt")
	QMAKE_CXXFLAGS += -O3
}

unix {
	target.path = /usr/lib
	INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/release/ -lhttp-daemon-components
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/debug/ -lhttp-daemon-components
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/ -lhttp-daemon-components
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Release/ -lhttp-daemon-components

INCLUDEPATH += $$PWD/../../http-daemon-components
DEPENDPATH += $$PWD/../../http-daemon-components

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/release/libhttp-daemon-components.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/debug/libhttp-daemon-components.a
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Release/libhttp-daemon-components.a
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/libhttp-daemon-components.a
