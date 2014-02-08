#include "loginhttprequesthandler.h"

LoginHTTPRequestHandler::LoginHTTPRequestHandler(const HTTPRequest &requestData) :
    HTTPRequestHandler(requestData)
{
}

void LoginHTTPRequestHandler::createResponse()
{
    HTTPResponse response;
    QString page = "\r\n<html><body>"
            "<form method=\"POST\">"
            "%1"
            "Username: <input type=\"text\" name=\"username\">"
            "Password: <input type=\"password\" name=\"pass\">"
            "<INPUT type=\"submit\" value=\"Auth\">"
            "</form></body></html>";

    if("GET" == requestData.method){
        response.setStatusCode(200);
        response.setReasonPhrase("OK");

        //TODO: maybe use a QNetworkCookie here
        if(requestData.fields.contains("Cookie") &&
                "loggedin=1" == requestData.fields["Cookie"].toString()){
            response.setBody("You're logged in!");
        }
        else{
            response.setBody(page.arg(""));
        }

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    if("POST" == requestData.method && !requestData.postData.isEmpty()){
        if(requestData.postData.contains("username") &&
                "Ion" == requestData.postData["username"] &&
                requestData.postData.contains("pass") &&
                "1234" == requestData.postData["pass"]){

            response.setStatusCode(200);
            response.setReasonPhrase("OK");

            //this could be somethign randomized in order to avoid replicating
            response.setHeaderField("Set-Cookie", "loggedin=1");
            response.setBody("You're logged in!\n");

            emit responseWritten(response);
            emit endOfWriting();
            return;
        }

        response.setStatusCode(200);
        response.setReasonPhrase("OK");
        response.setBody(page.arg("Login failed, try again!<br>"));

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }


    response.setStatusCode(400);
    response.setReasonPhrase("Bad request");

    emit responseWritten(response);
    emit endOfWriting();
}
