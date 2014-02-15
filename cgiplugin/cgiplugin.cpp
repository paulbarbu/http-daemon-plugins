#include "cgiplugin.h"
#include "cgipluginhttprequesthandler.h"

CgiPlugin::CgiPlugin(QObject *parent) : QObject(parent)
{
}

HTTPRequestHandler *CgiPlugin::getHTTPRequestHandler(const QHash<QString, QVariant> &s) const
{
    return new CgiPluginHTTPRequestHandler(s);
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    Q_EXPORT_PLUGIN2(cgiplugin, CgiPlugin)
#endif
