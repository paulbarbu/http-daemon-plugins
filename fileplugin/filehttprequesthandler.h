#ifndef FILEHTTPREQUESTHANDLER_H
#define FILEHTTPREQUESTHANDLER_H

#include "httprequesthandler.h"

class FileHTTPRequestHandler : public HTTPRequestHandler
{
public:
    FileHTTPRequestHandler(const QHash<QString, QVariant> &s);
    void createResponse(const HTTPRequest &requestData);
protected:
    void serveDirContents(const QString &path);
    void serveFileContents(const QString &path);

private:
    HTTPResponse response;
};

#endif // FILEHTTPREQUESTHANDLER_H
