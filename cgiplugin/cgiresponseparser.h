#ifndef CGIRESPONSEPARSER_H
#define CGIRESPONSEPARSER_H

#include <QByteArray>

#include "cgiresponse.h"

class CgiResponseParser
{
public:
    CgiResponseParser(const QByteArray &r) : rawResponse(r) {};
    bool parse();
    CgiResponse getResponse() const;
private:
    bool parseHeaders(const QByteArray &headers);
    bool parseStatus(const QString &statusLine);

    QByteArray rawResponse;
    CgiResponse response;
};

#endif // CGIRESPONSEPARSER_H
