#ifndef CGIRESPONSE_H
#define CGIRESPONSE_H

#include <QHash>

#include "httpresponse.h"

class CgiResponse : public HTTPResponse
{
public:
    explicit CgiResponse() : HTTPResponse() {}
    void setCgiField(const QString &key, const QString &value);

    QByteArray get() const;
    QByteArray getPartial();

    void clear();
private:
    QHash<QString, QString> cgiFields;
};

#endif // CGIRESPONSE_H
