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
    HTTPRequestHandler(s), chunkSize(512*1024)
{
    unsigned int chunk_size_val;
    bool ok;
    chunk_size_val = settings["chunksize"].toUInt(&ok);

    if(ok){
        chunkSize = chunk_size_val*1024;
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

    unsigned int fileSize = file.size();
    if(fileSize < chunkSize){
        chunkSize = fileSize;
    }

    uchar *data = file.map(0, chunkSize);

    if(0 == data){
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

    emit responseWritten(response); //send the headers

    emit rawDataWritten(data, chunkSize); //send the first part of the file

    if(chunkSize < fileSize){
        unsigned int pos = chunkSize;
        unsigned int notSent = fileSize-chunkSize;

        for(; pos<=notSent; pos+=chunkSize){
            data = file.map(pos, chunkSize);

            if(0 == data){
                qDebug() << "for, data=0";
                emit endOfWriting();
                return;
            }

            emit rawDataWritten(data, chunkSize);
        }

        int diff = fileSize % chunkSize;

        if(0 == diff){
            qDebug() << "diff=0, done";
            emit endOfWriting();
            return;
        }

        //what happens if diff is 0
        data = file.map(pos, diff);

        if(0 == data){
            qDebug() << "diff, data=0";
            emit endOfWriting();
            return;
        }

        emit rawDataWritten(data, diff);
    }

    emit endOfWriting();
}
