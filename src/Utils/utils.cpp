#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QDebug>


QJsonObject convertPayloadToJson(const QByteArray& payload)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(payload);

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        return jsonDoc.object();
    } else {
        qWarning() << "Invalid JSON payload:" << payload;
        return QJsonObject();
    }
}
