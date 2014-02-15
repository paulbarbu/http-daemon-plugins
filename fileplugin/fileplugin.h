#ifndef FILEPLUGIN_H
#define FILEPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "iplugin.h"
#include "httprequest.h"
#include "fileplugin_global.h"

#define FILEPLUGIN_IID "http-daemon.FilePlugin/1.0"

class FILEPLUGINSHARED_EXPORT FilePlugin :
        public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        Q_PLUGIN_METADATA(IID FILEPLUGIN_IID)
    #endif
public:
    FilePlugin(QObject *parent=0);
    HTTPRequestHandler *getHTTPRequestHandler(const QHash<QString, QVariant> &s) const;
};

#endif // FILEPLUGIN_H
