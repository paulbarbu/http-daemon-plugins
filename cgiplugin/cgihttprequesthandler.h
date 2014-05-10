#ifndef CGIPLUGINHTTPREQUESTHANDLER_H
#define CGIPLUGINHTTPREQUESTHANDLER_H

#include <QProcessEnvironment>

#include "httprequesthandler.h"

class CgiHTTPRequestHandler : public HTTPRequestHandler
{
public:
    explicit CgiHTTPRequestHandler(const QHash<QString, QVariant> &s);
    void createResponse(const HTTPRequest &requestData);
private:
    void clear();
    void setEnvironment(const HTTPRequest &requestData);

    QString pathInfo;
    QStringList urlParts;
    HTTPResponse response;
    int timeout;
    QProcessEnvironment env;
};

#endif // CGIPLUGINHTTPREQUESTHANDLER_H
