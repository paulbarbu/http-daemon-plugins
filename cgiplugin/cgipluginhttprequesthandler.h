#ifndef CGIPLUGINHTTPREQUESTHANDLER_H
#define CGIPLUGINHTTPREQUESTHANDLER_H

#include <QProcessEnvironment>

#include "httprequesthandler.h"
#include "cgiresponse.h"

class CgiPluginHTTPRequestHandler : public HTTPRequestHandler
{
public:
    explicit CgiPluginHTTPRequestHandler(const QHash<QString, QVariant> &s);
    void createResponse(const HTTPRequest &requestData);
private:
    void setScriptName();
    void setEnvironment(const HTTPRequest &requestData);

    QString scriptName;
    QStringList urlParts;
    CgiResponse response;
    int timeout;
    QProcessEnvironment env;
};

#endif // CGIPLUGINHTTPREQUESTHANDLER_H
