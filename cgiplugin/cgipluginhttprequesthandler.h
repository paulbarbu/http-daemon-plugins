#ifndef CGIPLUGINHTTPREQUESTHANDLER_H
#define CGIPLUGINHTTPREQUESTHANDLER_H

#include "httprequesthandler.h"

class CgiPluginHTTPRequestHandler : public HTTPRequestHandler
{
public:
    explicit CgiPluginHTTPRequestHandler(const HTTPRequest &requestData, const QHash<QString, QVariant> &s);
    void createResponse();
};

#endif // CGIPLUGINHTTPREQUESTHANDLER_H
