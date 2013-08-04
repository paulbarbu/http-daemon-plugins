#include "squareplugin.h"
#include "squarehttprequesthandler.h"

SquarePlugin::SquarePlugin()
{
}

HTTPRequestHandler *SquarePlugin::getHTTPRequestHandler(const HTTPRequest &requestData)
{
    return new SquareHTTPRequestHandler(requestData);
}

Q_EXPORT_PLUGIN2(squareplugin, SquarePlugin)
