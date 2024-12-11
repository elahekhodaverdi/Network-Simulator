#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "../NetworkConfig/NetworkConfig.h"
#include "src/AutonomousSystem/AutonomousSystem.h"

class ConfigReader
{
public:
    static QJsonObject readJson(const QString &filePath);
    static NetworkConfig readNetworkConfig(const QString &pathToConfFile);

private:
    static SimulationConfig parseSimulationConfig(const QJsonObject &config);
    static QList<AutonomousSystem *> parseAutonomousSystems(const QJsonArray &systemsArray);
};

#endif // CONFIGREADER_H
