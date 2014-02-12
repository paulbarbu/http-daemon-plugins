#include <QProcess>
#include <QFileInfo>

#include "cgipluginhttprequesthandler.h"
#include "cgiresponseparser.h"

CgiPluginHTTPRequestHandler::CgiPluginHTTPRequestHandler(const HTTPRequest &requestData,
                                                         const QHash<QString, QVariant> &s) :
    HTTPRequestHandler(requestData, s), scriptName(""), timeout(5000)
{
    int timeout_val;
    bool ok;
    timeout_val = settings["timeout"].toInt(&ok);

    if(ok){
        timeout = timeout_val;
    }

    urlParts = requestData.url.path().split("/", QString::SkipEmptyParts);

    setScriptName();
}

void CgiPluginHTTPRequestHandler::createResponse()
{
    qDebug() << settings;

    QFileInfo info(settings.value("cgi-dir", "").toString());

    if(!info.isReadable() || !info.isDir()){
        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");
        response.setBody("Invalid configuration");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    if(scriptName.isEmpty()){
        response.setStatusCode(404);
        response.setReasonPhrase("Not Found");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    setEnvironment();

    QProcess process;

    process.setProcessEnvironment(env);
    process.setWorkingDirectory(settings["cgi-dir"].toString());

    int pos = urlParts[1].indexOf(".");

    if(-1 == pos){
        response.setStatusCode(404);
        response.setReasonPhrase("Not Found");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    QString extension(urlParts[1].right(urlParts[1].size() - pos -1));

    qDebug() << "Extension:" << extension;
    if(!settings.contains(extension)){
        response.setStatusCode(404);
        response.setReasonPhrase("Not Found");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    QString path = settings[extension].toString();

    if("*" == path){
        path = settings["cgi-dir"].toString() + "/" + urlParts[1];
    }

    qDebug() << "Starting process:" << path;
    process.start(path);

    //TODO: do it using the SIGNAL-SLOT mechanism
    if(!process.waitForStarted(timeout)){
        qDebug() << "Process hasn't started, error:" << process.errorString();
        qDebug() << "Process state: " << process.state();

        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    //TODO: test with a form
    if(requestData.contentLength > 0){
        QByteArray input;
        QHash<QString, QString>::const_iterator i;

        //TODO: implement a HTTPRequest.rawPost() method
        //TODO: test this and the HTTPParser with somethign that contains "=" and "&" as actual data
        for(i=requestData.postData.constBegin(); i != requestData.postData.end(); ++i){
            input += i.key() +  "=" + i.value() + "&";
        }

        //remove the last "&"
        input.remove(input.length() - 1, 1);
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

    //TODO: check the segfault happening if not setting the response and calling these
    emit responseWritten(response);
    emit endOfWriting();
}

void CgiPluginHTTPRequestHandler::setScriptName()
{
    if(2 <= urlParts.size()){
        scriptName = "/" + urlParts[0] + "/" + urlParts[1];
    }
}

void CgiPluginHTTPRequestHandler::setEnvironment()
{
    //{PHP specific
    env.insert("REDIRECT_STATUS", "200"); //TODO: maybe change this, http://php.net/manual/en/security.cgi-bin
    //absolute path to the script
    env.insert("SCRIPT_FILENAME", settings["cgi-dir"].toString() + "/" + urlParts[1]);
    qDebug() << "SCRIPT_FILENAME" << settings["cgi-dir"].toString() + "/" + urlParts[1];

    env.insert("SCRIPT_NAME", scriptName);
    qDebug() << "SCRIPT_NAME" << scriptName;

    env.insert("PATH_INFO", requestData.url.path().replace(0, scriptName.length(), ""));
    qDebug() << "PATH_INFO" << requestData.url.path().replace(0, scriptName.length(), "");

    env.insert("GATEWAY_INTERFACE", requestData.method);
    qDebug() << "GATEWAY_INTERFACE" << requestData.method;

    env.insert("REMOTE_ADDR", requestData.remoteAddress.toString());
    qDebug() << "REMOTE_ADDR" << requestData.remoteAddress.toString();

    env.insert("SERVER_PORT", QString::number(requestData.port));
    qDebug() << "SERVER_PORT" << QString::number(requestData.port);

    env.insert("SERVER_NAME", requestData.host.toString()); //TODO: check it
    qDebug() << "SERVER_NAME" << requestData.host.toString();

    env.insert("SERVER_PROTOCOL", requestData.protocol + "/" + QString::number(requestData.protocolVersion)); //TODO: check it
    qDebug() << "SERVER_PROTOCOL:" << QString(requestData.protocol + "/" + QString::number(requestData.protocolVersion));

    env.insert("REQUEST_METHOD", "CGI/1.1");

    if(requestData.url.hasQuery()){
        #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
            env.insert("QUERY_STRING", requestData.url.query(QUrl::FullyEncoded));
            qDebug() << "QUERY_STRING" << requestData.url.query(QUrl::FullyEncoded);
        #else
            //TODO: test this
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

    if(requestData.fields.contains("Content-Type")){
        env.insert("CONTENT_TYPE", requestData.fields["Content-Type"].toString());
        qDebug() << "CONTENT_TYPE" << requestData.fields["Content-Type"].toString();
    }
}
