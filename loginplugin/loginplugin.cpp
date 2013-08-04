#include "loginplugin.h"
#include "loginhttprequesthandler.h"

LoginPlugin::LoginPlugin()
{
}

HTTPRequestHandler *LoginPlugin::getHTTPRequestHandler(const HTTPRequest &requestData)
{
    return new LoginHTTPRequestHandler(requestData);
}

Q_EXPORT_PLUGIN2(loginplugin, LoginPlugin)
