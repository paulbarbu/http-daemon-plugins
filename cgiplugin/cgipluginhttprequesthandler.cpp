#include <QProcess>
#include <QProcessEnvironment>
#include <QFileInfo>

#include "cgipluginhttprequesthandler.h"

CgiPluginHTTPRequestHandler::CgiPluginHTTPRequestHandler(const HTTPRequest &requestData,
                                                         const QHash<QString, QVariant> &s) :
    HTTPRequestHandler(requestData, s)
{

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

    if(!setScriptName(requestData.url)){
        response.setStatusCode(404);
        response.setReasonPhrase("Not Found");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    QProcessEnvironment env;

    //TODO: duplicate with the dispacher code and the setScriptName method
    QStringList urlParts = requestData.url.path().split("/", QString::SkipEmptyParts);

    //PHP specific
    env.insert("REDIRECT_STATUS", "200"); //TODO: maybe change this, http://php.net/manual/en/security.cgi-bin
    //absolute path to the script, PHP specific
    env.insert("SCRIPT_FILENAME", settings["cgi-dir"].toString() + "/" + urlParts[1]);
    qDebug() << "SCRIPT_FILENAME" << settings["cgi-dir"].toString() + "/" + urlParts[1];
    env.insert("SCRIPT_NAME", scriptName);
    qDebug() << "SCRIPT_NAME" << scriptName;

    env.insert("PATH_INFO", requestData.url.path().replace(0, scriptName.length(), ""));
    qDebug() << "PATH_INFO" << requestData.url.path().replace(0, scriptName.length(), "");

    env.insert("REQUEST_METHOD", "CGI/1.1");
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

    if(requestData.fields.contains("Content-Type")){
        env.insert("CONTENT_TYPE", requestData.fields["Content-Type"].toString());
        qDebug() << "CONTENT_TYPE" << requestData.fields["Content-Type"].toString();
    }

    //TODO: here I could implement a configurable timeout, now using the default 3000 ms
    QProcess process;

    process.setProcessEnvironment(env);
    process.setWorkingDirectory(settings["cgi-dir"].toString());

    //invoke other - configurable - things too (PERL)
    process.start("php-cgi");

    //TODO: do it using the SIGNAL-SLOT mechanism
    if(!process.waitForStarted()){
        qDebug() << "Process hasn't started, error:" << process.errorString();
        qDebug() << "Process state: " << process.state();

        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");
        response.setBody("Invalid configuration");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    //TODO: refactor this!

    //TODO: set stdin and test (with a form)
    if(requestData.contentLength > 0){
        QByteArray input;
        QHash<QString, QString>::const_iterator i;

        for(i=requestData.postData.constBegin(); i != requestData.postData.end(); ++i){
            input += i.key() +  "=" + i.value() + "&";
        }

        input.remove(input.length() - 1, 1);
        qDebug() << "STDIN:" << input;

        process.write(input);
    }

    process.closeWriteChannel();

    if (!process.waitForFinished()){
        qDebug() << "Process hasn't finished, error:" << process.errorString();
        qDebug() << "Process state: " << process.state();

        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");
        response.setBody("Invalid configuration");

        emit responseWritten(response);
        emit endOfWriting();
        return;
    }

    QByteArray output(process.readAllStandardOutput());
    qDebug() << "CGI output:" << output;

    //TODO: proper parsing
    response.setStatusCode(200);
    response.setReasonPhrase("OK");
    response.setHeaderField("Content-Type", "text/html");

    response.setBody(output);

    // TODO: see: http://127.0.0.1:8282/cgi/inexisten_path

    //TODO: check the segfault happening if not setting the response and calling these
    emit responseWritten(response);
    emit endOfWriting();
}

bool CgiPluginHTTPRequestHandler::setScriptName(const QUrl &url)
{
    QStringList urlParts = url.path().split("/", QString::SkipEmptyParts);

    if(2 > urlParts.size()){
        return false;
    }

    scriptName = "/" + urlParts[0] + "/" + urlParts[1];
    return true;
}
