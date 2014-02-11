#include <QStringList>
#include <QDebug>

#include "cgiresponseparser.h"

bool CgiResponseParser::parse()
{
    if(rawResponse.isEmpty()){
        return false;
    }

    QString headBodySep("\n\n");
    int pos = rawResponse.indexOf(headBodySep);

    if(-1 == pos){
        headBodySep = "\r\n\r\n";
        pos = rawResponse.indexOf(headBodySep);

        if(-1 == pos){
            qDebug() << "No separator between header and body";
            return false;
        }
    }

    response.setStatusCode(200);
    response.setReasonPhrase("OK");

    QByteArray headers = rawResponse.left(pos);
    if(!parseHeaders(headers)){
        qDebug() << "Cannot parse the header";
        return false;
    }

    response.setBody(rawResponse.right(rawResponse.size() - pos - headBodySep.size()));

    return true;
}

CgiResponse CgiResponseParser::getResponse() const
{
    return response;
}

bool CgiResponseParser::parseHeaders(const QByteArray &headers)
{
    QString data = headers;

    QStringList headerLines = data.replace("\r", "").split("\n", QString::SkipEmptyParts);

    if(headerLines.isEmpty()){
        qDebug() <<  "The CGI response should contain at least one header field";
        return false;
    }

    foreach(QString line, headerLines){
        int pos = line.indexOf(":");
        if(-1 == pos){
            qDebug() << "No ':' separator found on header line: " << line;
            return false;
        }

        QString key = line.left(pos).trimmed();
        QString value = line.right(line.size() - pos - 1).trimmed();

        if(key.isEmpty() || value.isEmpty()){
            continue;
        }

        //TODO: inspect more closely section 6 of http://www.faqs.org/rfcs/rfc3875.html
        if("Status" == key){
            if(!parseStatus(value)){
                qDebug() << "Failed to parse CGI status line";
                return false;
            }
        }
        else if("Location" == key){
            //TODO: implement
            response.setCgiField(key, value);
        }
        else{
            response.setHeaderField(key, value);
        }
    }

    return true;
}

bool CgiResponseParser::parseStatus(const QString &statusLine)
{
    int pos = statusLine.indexOf(" ");

    if(-1 == pos){
        qDebug() << "No status code and reason phrase separator";
        return false;
    }

    QString statusCode(statusLine.left(pos));

    bool ok;
    int s = statusCode.toInt(&ok);

    if(!ok || !response.isValidStatusCode(s)){
        qDebug() << "Invalid status code";
        return false;
    }

    response.setStatusCode(s);
    response.setReasonPhrase(statusLine.right(statusLine.size() - pos - 1));

    return true;
}
