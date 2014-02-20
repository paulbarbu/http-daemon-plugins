#include <QtGlobal>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    #include <QMimeDatabase>
#endif
#include <QDebug>

#include "filehttprequesthandler.h"

FileHTTPRequestHandler::FileHTTPRequestHandler(const QHash<QString, QVariant> &s) :
    HTTPRequestHandler(s), chunk_size(512*1024)
{
    unsigned int chunk_size_val;
    bool ok;
    chunk_size_val = settings["chunksize"].toUInt(&ok);

    if(ok){
        chunk_size = chunk_size_val*1024;
    }
}

void FileHTTPRequestHandler::createResponse(const HTTPRequest &requestData)
{
    response.clear();

    QString fullPath = settings["docroot"].toString() + requestData.url.path();
    QFileInfo fileInfo(fullPath);

    if(!fileInfo.exists()){
        response.setStatusCode(404);
        response.setReasonPhrase("Not Found");

        emit responseWritten(response);
        emit endOfWriting();

        return;
    }

    if(!fileInfo.isReadable()){
        qDebug() << "Not readable!";

        response.setStatusCode(403);
        response.setReasonPhrase("Forbidden");
        response.setBody("Permission denied\n");

        emit responseWritten(response);
        emit endOfWriting();

        return;
    }

    if(fileInfo.isDir()){
        serveDirContents(fullPath);
    }
    else{
        serveFileContents(fullPath);
    }
}

void FileHTTPRequestHandler::serveDirContents(const QString &path)
{
    QDir dir(path);
    QStringList dirList = dir.entryList();

    response.setStatusCode(200);
    response.setReasonPhrase("OK");
    response.setHeaderField("Content-Type", "text/plain");
    response.setBody(dirList.join("\n"));

    emit responseWritten(response);
    emit endOfWriting();
}

void FileHTTPRequestHandler::serveFileContents(const QString &path)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Cannot open " << path;

        response.setStatusCode(500);
        response.setReasonPhrase("Internal Server Error");

        emit responseWritten(response);
        emit endOfWriting();

        return;
    }

    response.setStatusCode(200);
    response.setReasonPhrase("OK");

    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QMimeDatabase db;
        QMimeType type = db.mimeTypeForFile(path);
        if(type.isValid()){
            qDebug() << "Mime type:" << type.name();
            response.setHeaderField("Content-Type", type.name());
        }
    #endif

    QByteArray content;
    content = file.read(chunk_size);

    while(!content.isEmpty()){
        response.appendBody(content);
        emit responseWritten(response);

        content = file.read(chunk_size);
    }

    emit endOfWriting();
}
