#ifndef SQUAREHTTPREQUESTHANDLER_H
#define SQUAREHTTPREQUESTHANDLER_H

#include "httprequesthandler.h"

class SquareHTTPRequestHandler : public HTTPRequestHandler
{
public:
    SquareHTTPRequestHandler(const HTTPRequest &r);
    void createResponse();
};

#endif // SQUAREHTTPREQUESTHANDLER_H
