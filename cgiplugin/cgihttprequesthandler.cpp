#include <QDebug>
#include <QProcess>
#include <QFileInfo>

#include "cgihttprequesthandler.h"
#include "cgiresponseparser.h"

CgiHTTPRequestHandler::CgiHTTPRequestHandler(const QHash<QString, QVariant> &s) :
    HTTPRequestHandler(s), pathInfo(""), timeout(5000)
{
    int timeout_val;
    bool ok;
    timeout_val = settings["timeout"].toInt(&ok);

    if(ok){
        timeout = timeout_val;
    }
}

void CgiHTTPRequestHandler::createResponse(const HTTPRequest &requestData)
{
    //TODO: on some setups the plugins blocks on process.waitForFinished
    //TODO: correctly parse CGI output

    // http://127.0.0.1:8282/cgi/info.php/asd
    // => SCRIPT_FILENAME cgi-bin-dir + /info.php/
    // => SCRIPT_NAME /cgi/info.php
    // => PATH_INFO /asd

    // http://127.0.0.1:8282/cgi/wp/index.php/asd
    // => SCRIPT_FILENAME cgi-bin-dir + /wp/index.php/
    // => SCRIPT_NAME /cgi/wp/index.php
    // => PATH_INFO /asd

    if(requestData.url.isEmpty()){
        response.setStatusCode(404);
        response.setReasonPhrase("Not Found");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    clear();
    int slashPos = requestData.url.path().indexOf('/', 1);
    urlParts << requestData.url.path().left(slashPos);
    urlParts << requestData.url.path().right(requestData.url.path().size() - slashPos);

    if(urlParts[1].endsWith('/'))
    {
        urlParts[1].truncate(urlParts[1].size()-1);
    }

    foreach(QString part, urlParts){
        qDebug()<<requestData.url.path() << "part:" << part;
    }

    qDebug() << settings;

    QFileInfo info(settings.value("cgi-dir", "").toString());

    if(!info.isReadable() || !info.isDir()){
        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");
        response.setBody("Invalid configuration");
        qDebug() << "Cannot read cgi-dir or not a directory:" << info.absoluteFilePath();

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    int extPos = urlParts[1].indexOf(".");

    if(-1 == extPos){
        response.setStatusCode(404);
        response.setReasonPhrase("Not Found");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    slashPos = urlParts[1].indexOf('/', extPos);

    QString extension(urlParts[1].mid(extPos+1, slashPos-extPos-1));

    qDebug() << "Extension:" << extension;
    if(!settings.contains(extension)){
        response.setStatusCode(404);
        response.setReasonPhrase("Not Found");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    pathInfo = urlParts[1].right(urlParts[1].size() - slashPos);

    setEnvironment(requestData);

    QProcess process;

    process.setProcessEnvironment(env);
    process.setWorkingDirectory(settings["cgi-dir"].toString());

    QString path = settings[extension].toString();

    if("*" == path){
        path = settings["cgi-dir"].toString() + urlParts[1];
    }

    qDebug() << "Starting process:" << path;
    process.start(path);

    if(!process.waitForStarted(timeout)){
        qDebug() << "Process hasn't started, error:" << process.errorString();
        qDebug() << "Process state: " << process.state();

        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    if(requestData.contentLength > 0){
        QByteArray input;

        if(!requestData.postData.isEmpty()){
            QHash<QString, QString>::const_iterator i;

            for(i=requestData.postData.constBegin(); i != requestData.postData.constEnd(); ++i){
                input += i.key() +  "=" + i.value() + "&";
            }

            //remove the last "&"
            input.remove(input.length() - 1, 1);
        }
        else{
            input = requestData.rawPostData;
        }

        qDebug() << "STDIN:" << input;

        process.write(input);
        process.waitForBytesWritten(timeout);
    }

    process.closeWriteChannel();

    if (!process.waitForFinished(timeout)){
        qDebug() << "Process hasn't finished, error:" << process.errorString();
        qDebug() << "Process state: " << process.state();
        process.close();

        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    QByteArray output(process.readAllStandardOutput());
    qDebug() << "CGI output:" << output;

    CgiResponseParser parser(output);
    if(!parser.parse()){
        qDebug() << "Couldn't parse the output of the CGI script";

        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    response = parser.getResponse();

    emit responseWritten(response);
    emit endOfWriting();
}

void CgiHTTPRequestHandler::clear()
{
    pathInfo.clear();
    response.clear();
    urlParts.clear();
    env.clear();
}

void CgiHTTPRequestHandler::setEnvironment(const HTTPRequest &requestData)
{
    env.insert("REDIRECT_STATUS", "200");

    //absolute path to the script
    env.insert("SCRIPT_FILENAME", settings["cgi-dir"].toString() + urlParts[1]);
    qDebug() << "SCRIPT_FILENAME" << settings["cgi-dir"].toString() + urlParts[1];

    env.insert("SCRIPT_NAME", requestData.url.path());
    qDebug() << "SCRIPT_NAME" << requestData.url.path();

    env.insert("PATH_INFO", pathInfo);
    qDebug() << "PATH_INFO" << pathInfo;

    env.insert("REQUEST_METHOD", requestData.method);
    qDebug() << "REQUEST_METHOD" << requestData.method;

    env.insert("REMOTE_ADDR", requestData.remoteAddress.toString());
    qDebug() << "REMOTE_ADDR" << requestData.remoteAddress.toString();

    env.insert("SERVER_PORT", QString::number(requestData.port));
    qDebug() << "SERVER_PORT" << QString::number(requestData.port);

    env.insert("SERVER_NAME", requestData.host.toString());
    qDebug() << "SERVER_NAME" << requestData.host.toString();

    env.insert("SERVER_PROTOCOL", requestData.protocol + "/" + QString::number(requestData.protocolVersion));
    qDebug() << "SERVER_PROTOCOL:" << QString(requestData.protocol + "/" + QString::number(requestData.protocolVersion));

    env.insert("GATEWAY_INTERFACE", "CGI/1.1");

    if(requestData.url.hasQuery()){
        #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
            env.insert("QUERY_STRING", requestData.url.query(QUrl::FullyEncoded));
            qDebug() << "QUERY_STRING" << requestData.url.query(QUrl::FullyEncoded);
        #else
            env.insert("QUERY_STRING", requestData.url.encodedQuery());
            qDebug() << "QUERY_STRING" << requestData.url.encodedQuery();
        #endif
    }
    else{
        env.insert("QUERY_STRING", "");
        qDebug() << "QUERY_STRING" << "";
    }

    if(requestData.contentLength > 0){
        env.insert("CONTENT_LENGTH", QString::number(requestData.contentLength));
        qDebug() << "CONTENT_LENGTH" << QString::number(requestData.contentLength);
    }


    if(!requestData.contentType.isEmpty()){
        env.insert("CONTENT_TYPE", requestData.contentType);
        qDebug() << "CONTENT_TYPE" << requestData.contentType;
    }
}
