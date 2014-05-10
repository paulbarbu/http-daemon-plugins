#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0 ,0)
    #include <QtConcurrent/QtConcurrentFilter>
#else
    #include <QtConcurrentFilter>
#endif

#include "loginhttprequesthandler.h"

LoginHTTPRequestHandler::LoginHTTPRequestHandler() : HTTPRequestHandler()
{
}

void LoginHTTPRequestHandler::createResponse(const HTTPRequest &r)
{
    HTTPResponse response;
    QString page = "\r\n<html><body>"
            "<form method=\"POST\">"
            "%1"
            "Username: <input type=\"text\" name=\"username\">"
            "Password: <input type=\"password\" name=\"pass\">"
            "<INPUT type=\"submit\" value=\"Auth\">"
            "</form></body></html>";

    if("GET" == r.method){
        response.setStatusCode(200);
        response.setReasonPhrase("OK");

        QList<QNetworkCookie> cookieList = QtConcurrent::blockingFiltered(r.cookieJar,
            [] (const QNetworkCookie &cookie) -> bool {
                return cookie.name() == "loggedin" &&  cookie.value() == "1";
            }
        );

        if(1 == cookieList.size()){
            response.setBody("You're logged in!");
        }
        else{
            response.setBody(page.arg(""));
        }

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    if("POST" == r.method && !r.postData.isEmpty()){
        if(r.postData.contains("username") &&
                "Ion" == r.postData["username"] &&
                r.postData.contains("pass") &&
                "1234" == r.postData["pass"]){

            response.setStatusCode(200);
            response.setReasonPhrase("OK");

            //TODO: this could be something randomized in order to avoid replicating
            QNetworkCookie cookie("loggedin", "1");
            cookie.setHttpOnly(true);
            response.setCookie(cookie);

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
