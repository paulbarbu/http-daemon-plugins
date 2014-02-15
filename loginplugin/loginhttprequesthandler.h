#ifndef LOGINHTTPREQUESTHANDLER_H
#define LOGINHTTPREQUESTHANDLER_H

#include "httprequesthandler.h"

class LoginHTTPRequestHandler : public HTTPRequestHandler
{
public:
    LoginHTTPRequestHandler();
    void createResponse(const HTTPRequest &r);
};

#endif // LOGINHTTPREQUESTHANDLER_H
