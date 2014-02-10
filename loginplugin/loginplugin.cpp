#include "loginplugin.h"
#include "loginhttprequesthandler.h"

LoginPlugin::LoginPlugin(QObject *parent) : QObject(parent)
{
}

HTTPRequestHandler *LoginPlugin::getHTTPRequestHandler(const HTTPRequest &requestData,
                                                       const QHash<QString, QVariant> &s) const
{
    Q_UNUSED(s);

    return new LoginHTTPRequestHandler(requestData);
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    Q_EXPORT_PLUGIN2(loginplugin, LoginPlugin)
#endif
