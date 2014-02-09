#-------------------------------------------------
#
# Project created by QtCreator 2013-08-04T17:35:00
#
#-------------------------------------------------

QT       -= gui
QT += network

TARGET = squareplugin
TEMPLATE = lib

DEFINES += SQUAREPLUGIN_LIBRARY

SOURCES += squareplugin.cpp \
	squarehttprequesthandler.cpp

HEADERS += squareplugin.h\
		squareplugin_global.h \
	squarehttprequesthandler.h

QMAKE_CXXFLAGS += -std=c++11

unix:!symbian {
	maemo5 {
		target.path = /opt/usr/lib
	} else {
		target.path = /usr/lib
	}
	INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/release/ -lhttp-daemon-components
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/debug/ -lhttp-daemon-components
else:unix: LIBS += -L$$PWD/../../build-http-daemon-components-Desktop-Debug/ -lhttp-daemon-components

INCLUDEPATH += $$PWD/../../http-daemon-components
DEPENDPATH += $$PWD/../../http-daemon-components

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/release/http-daemon-components.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/debug/http-daemon-components.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../build-http-daemon-components-Desktop-Debug/libhttp-daemon-components.a
