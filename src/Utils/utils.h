#ifndef UTILS_H
#define UTILS_H
#include <QJsonObject>
#include <QByteArray>

class Utils
{
public:
    Utils();
    static QJsonObject convertPayloadToJson(const QByteArray& payload);
};

#endif    // UTILS_H
