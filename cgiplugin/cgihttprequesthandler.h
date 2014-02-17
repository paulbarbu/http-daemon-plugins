#ifndef CGIPLUGINHTTPREQUESTHANDLER_H
#define CGIPLUGINHTTPREQUESTHANDLER_H

#include <QProcessEnvironment>

#include "httprequesthandler.h"
#include "cgiresponse.h"

class CgiHTTPRequestHandler : public HTTPRequestHandler
{
public:
    explicit CgiHTTPRequestHandler(const QHash<QString, QVariant> &s);
    void createResponse(const HTTPRequest &requestData);
private:
    void clear();
    void setScriptName();
    void setEnvironment(const HTTPRequest &requestData);

    QString scriptName;
    QStringList urlParts;
    CgiResponse response;
    int timeout;
    QProcessEnvironment env;
};

#endif // CGIPLUGINHTTPREQUESTHANDLER_H
