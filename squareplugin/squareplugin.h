#ifndef SQUAREPLUGIN_H
#define SQUAREPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "iplugin.h"
#include "httprequesthandler.h"
#include "httprequest.h"
#include "squareplugin_global.h"

class SQUAREPLUGINSHARED_EXPORT SquarePlugin :
        public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
public:
    SquarePlugin(QObject *parent=0);
        HTTPRequestHandler *getHTTPRequestHandler(const HTTPRequest &requestData) const;
};

#endif // SQUAREPLUGIN_H
