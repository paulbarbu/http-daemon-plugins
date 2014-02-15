#ifndef LOGINPLUGIN_H
#define LOGINPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "iplugin.h"
#include "httprequest.h"
#include "loginplugin_global.h"

#define LOGINPLUGIN_IID "http-daemon.LoginPlugin/1.0"

class LOGINPLUGINSHARED_EXPORT LoginPlugin :
        public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        Q_PLUGIN_METADATA(IID LOGINPLUGIN_IID)
    #endif
public:
    LoginPlugin(QObject *parent=0);
    HTTPRequestHandler *getHTTPRequestHandler(const QHash<QString, QVariant> &s) const;
};

#endif // LOGINPLUGIN_H
