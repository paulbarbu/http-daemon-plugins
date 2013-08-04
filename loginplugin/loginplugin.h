#ifndef LOGINPLUGIN_H
#define LOGINPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "iplugin.h"
#include "httprequest.h"
#include "loginplugin_global.h"

class LOGINPLUGINSHARED_EXPORT LoginPlugin :
        public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
public:
    LoginPlugin();
    HTTPRequestHandler *getHTTPRequestHandler(const HTTPRequest &requestData);
};

#endif // LOGINPLUGIN_H
