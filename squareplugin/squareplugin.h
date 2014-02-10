#ifndef SQUAREPLUGIN_H
#define SQUAREPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "iplugin.h"
#include "httprequesthandler.h"
#include "httprequest.h"
#include "squareplugin_global.h"

#define SQUAREPLUGIN_IID "http-daemon.SquarePlugin/1.0"

class SQUAREPLUGINSHARED_EXPORT SquarePlugin :
        public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        Q_PLUGIN_METADATA(IID SQUAREPLUGIN_IID)
    #endif
public:
    SquarePlugin(QObject *parent=0);
        HTTPRequestHandler *getHTTPRequestHandler(const HTTPRequest &requestData,
                                                  const QHash<QString, QVariant> &s) const;
};

#endif // SQUAREPLUGIN_H
