#ifndef CGIPLUGIN_H
#define CGIPLUGIN_H

#include <QtPlugin>

#include "iplugin.h"
#include "httprequest.h"
#include "cgiplugin_global.h"

#define CGIPLUGIN_IID "http-daemon.CgiPlugin/1.0"

class CGIPLUGINSHARED_EXPORT CgiPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        Q_PLUGIN_METADATA(IID CGIPLUGIN_IID)
    #endif
public:
    CgiPlugin(QObject *parent=0);
    HTTPRequestHandler *getHTTPRequestHandler(const HTTPRequest &requestData, const QHash<QString, QVariant> &s) const;
};

#endif // CGIPLUGIN_H
