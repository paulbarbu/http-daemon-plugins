#ifndef LOGINHTTPREQUESTHANDLER_H
#define LOGINHTTPREQUESTHANDLER_H

#include "httprequesthandler.h"

class LoginHTTPRequestHandler : public HTTPRequestHandler
{
public:
    LoginHTTPRequestHandler(const HTTPRequest &requestData);
    void createResponse();
};

#endif // LOGINHTTPREQUESTHANDLER_H
