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
    LoginPlugin(QObject *parent=0);
    HTTPRequestHandler *getHTTPRequestHandler(const HTTPRequest &requestData) const;
};

#endif // LOGINPLUGIN_H
