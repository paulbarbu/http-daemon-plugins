#include <QUrl>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QUrlQuery>
#endif

#include "squarehttprequesthandler.h"

SquareHTTPRequestHandler::SquareHTTPRequestHandler() : HTTPRequestHandler()
{
}

void SquareHTTPRequestHandler::createResponse(const HTTPRequest &r)
{
    HTTPResponse response;
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QUrlQuery url_query(r.url);
    #else
        QUrl url_query = r.url;
    #endif

    if("GET" != r.method || !url_query.hasQueryItem("a")){
        response.setStatusCode(400);
        response.setReasonPhrase("Bad Request");
        response.setBody("Try again with valid data!\n");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    QString numToSquare = url_query.queryItemValue("a");

    response.setStatusCode(200);
    response.setReasonPhrase("OK");

    bool ok;
    double n = numToSquare.toDouble(&ok);

    if(!ok){
        response.setBody("a must be a number!\n");
    }
    else{
        response.setBody(numToSquare + "^2 = " + QString::number(n*n) + "\n");
    }

    emit responseWritten(response);
    emit endOfWriting();
}
