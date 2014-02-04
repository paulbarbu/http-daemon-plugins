#include "squareplugin.h"
#include "squarehttprequesthandler.h"

SquarePlugin::SquarePlugin(QObject *parent) : QObject(parent)
{
}

HTTPRequestHandler *SquarePlugin::getHTTPRequestHandler(const HTTPRequest &requestData) const
{
    return new SquareHTTPRequestHandler(requestData);
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    Q_EXPORT_PLUGIN2(squareplugin, SquarePlugin)
#endif
