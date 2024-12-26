#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include "../Globals/Globals.h"
#include "utils.h"

Utils::Utils() {}


QJsonObject Utils::convertPayloadToJson(const QByteArray& payload)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(payload);

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        return jsonDoc.object();
    } else {
        qWarning() << "Invalid JSON payload:" << payload;
        return QJsonObject();
    }
}
