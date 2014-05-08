#include <QHash>
#include <QString>
#include <QVariant>

#include "fileplugin.h"
#include "filehttprequesthandler.h"

FilePlugin::FilePlugin(QObject *parent) : QObject(parent)
{
}

HTTPRequestHandler *FilePlugin::getHTTPRequestHandler(const QHash<QString, QVariant> &s) const
{
    return new FileHTTPRequestHandler(s);
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    Q_EXPORT_PLUGIN2(fileplugin, FilePlugin)
#endif
