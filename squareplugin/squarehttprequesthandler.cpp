#include "squarehttprequesthandler.h"

SquareHTTPRequestHandler::SquareHTTPRequestHandler(const HTTPRequest &r) :
    HTTPRequestHandler(r)
{
}

void SquareHTTPRequestHandler::createResponse()
{
    HTTPResponse response;
    if("GET" != requestData.method || !requestData.url.hasQueryItem("a")){
        response.setStatusCode(400);
        response.setReasonPhrase("Bad Request");
        response.setBody("Try again with valid data!\n");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    QString numToSquare = requestData.url.queryItemValue("a");

    response.setStatusCode(200);
    response.setReasonPhrase("OK");

    bool ok;
    double n = numToSquare.toDouble(&ok);

    if(!ok){
        response.setBody("a-ul trebuie sa fie numar!\n");
    }
    else{
        response.setBody(numToSquare + "^2 = " + QString::number(n*n) + "\n");
    }

    emit responseWritten(response);
    emit endOfWriting();
}
