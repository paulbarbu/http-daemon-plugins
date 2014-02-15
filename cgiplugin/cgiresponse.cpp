#include <QMultiHash>

#include "cgiresponse.h"

void CgiResponse::setCgiField(const QString &key, const QString &value)
{
    cgiFields.insert(key, value);
}

QByteArray CgiResponse::get() const
{
    QByteArray statusCode = getStatusCode();
    QByteArray reasonPhrase = getReasonPhrase();
    QMultiHash<QByteArray, QByteArray> fields = getFields();
    QByteArray body = getBody();

    if("" == statusCode || "" == reasonPhrase){
        return "";
    }

    QByteArray r = "HTTP/1.0 " + statusCode + " " + reasonPhrase + "\r\n";

    QMultiHash<QByteArray, QByteArray>::const_iterator i;
    for(i = fields.constBegin(); i != fields.constEnd(); ++i){
        r += i.key() + ": " + i.value() + "\r\n";
    }

    QHash<QString, QString>::const_iterator j;
    for(j = cgiFields.constBegin(); j != cgiFields.constEnd(); ++j){
        r += i.key() + ": " + i.value() + "\r\n";
    }

    r += "\r\n" + body;

    return r;
}

QByteArray CgiResponse::getPartial()
{
    return get();
}

void CgiResponse::clear()
{
    HTTPResponse::clear();
    cgiFields.clear();
}
