#ifndef CGIPLUGINHTTPREQUESTHANDLER_H
#define CGIPLUGINHTTPREQUESTHANDLER_H

#include <QProcessEnvironment>

#include "httprequesthandler.h"
#include "cgiresponse.h"

class CgiPluginHTTPRequestHandler : public HTTPRequestHandler
{
public:
    explicit CgiPluginHTTPRequestHandler(const HTTPRequest &requestData, const QHash<QString, QVariant> &s);
    void createResponse();
private:
    void setScriptName();
    void setEnvironment();

    QString scriptName;
    QStringList urlParts;
    CgiResponse response;
    int timeout;
    QProcessEnvironment env;
};

#endif // CGIPLUGINHTTPREQUESTHANDLER_H
