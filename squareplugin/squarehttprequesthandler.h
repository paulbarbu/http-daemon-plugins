#ifndef SQUAREHTTPREQUESTHANDLER_H
#define SQUAREHTTPREQUESTHANDLER_H

#include "httprequesthandler.h"

class SquareHTTPRequestHandler : public HTTPRequestHandler
{
public:
    SquareHTTPRequestHandler();
    void createResponse(const HTTPRequest &r);
};

#endif // SQUAREHTTPREQUESTHANDLER_H
