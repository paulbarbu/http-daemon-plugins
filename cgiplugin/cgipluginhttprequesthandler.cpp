#include "cgipluginhttprequesthandler.h"

CgiPluginHTTPRequestHandler::CgiPluginHTTPRequestHandler(const HTTPRequest &requestData,
                                                         const QHash<QString, QVariant> &s) :
    HTTPRequestHandler(requestData, s)
{

}

void CgiPluginHTTPRequestHandler::createResponse()
{
    HTTPResponse response;
    response.setBody("foobar cgiplugin");

    //TODO: implement
    //TODO: check not to be empty
    qDebug() << settings;


    emit responseWritten(response);
    emit endOfWriting();
}
