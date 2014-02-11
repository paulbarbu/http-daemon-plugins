#ifndef CGIPLUGINHTTPREQUESTHANDLER_H
#define CGIPLUGINHTTPREQUESTHANDLER_H

#include "httprequesthandler.h"
#include "cgiresponse.h"

class CgiPluginHTTPRequestHandler : public HTTPRequestHandler
{
public:
    explicit CgiPluginHTTPRequestHandler(const HTTPRequest &requestData, const QHash<QString, QVariant> &s);
    void createResponse();
private:
    bool setScriptName(const QUrl &url);

    QString scriptName;
    CgiResponse response;
};

#endif // CGIPLUGINHTTPREQUESTHANDLER_H
