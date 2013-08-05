#include "squareplugin.h"
#include "squarehttprequesthandler.h"

SquarePlugin::SquarePlugin(QObject *parent) : QObject(parent)
{
}

HTTPRequestHandler *SquarePlugin::getHTTPRequestHandler(const HTTPRequest &requestData) const
{
    return new SquareHTTPRequestHandler(requestData);
}

Q_EXPORT_PLUGIN2(squareplugin, SquarePlugin)
