#-------------------------------------------------
#
# Project created by QtCreator 2013-08-04T18:04:40
#
#-------------------------------------------------

QT       -= gui

TARGET = loginplugin
TEMPLATE = lib

DEFINES += LOGINPLUGIN_LIBRARY

SOURCES += loginplugin.cpp \
	loginhttprequesthandler.cpp

HEADERS += loginplugin.h\
	loginplugin_global.h \
	loginhttprequesthandler.h


#TODO: remove this since those platforms are not targeted
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
