#ifndef CGIRESPONSEPARSER_H
#define CGIRESPONSEPARSER_H

#include <QByteArray>
#include "httpresponse.h"

class CgiResponseParser
{
public:
    CgiResponseParser(const QByteArray &r) : rawResponse(r) {}
    bool parse();
    HTTPResponse getResponse() const;
private:
    bool parseHeaders(const QByteArray &headers);
    bool parseStatus(const QString &statusLine);

    QByteArray rawResponse;
    HTTPResponse response;
};

#endif // CGIRESPONSEPARSER_H
