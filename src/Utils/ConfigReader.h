#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "../Network/Network.h"
#include "src/AutonomousSystem/AutonomousSystem.h"

class ConfigReader
{
public:
    static QJsonObject readJson(const QString &filePath);
    static Network readNetwork(const QString &pathToConfFile);

private:
    static SimulationConfig parseSimulationConfig(const QJsonObject &config);
    static QList<AutonomousSystem *> parseAutonomousSystems(const QJsonArray &systemsArray);
    static UT::TopologyType stringToTopologyType(const QString &typeString);
    static QList<UT::TopologyType> parseTopologyTypes(const QString &topologyTypeString);
};

#endif // CONFIGREADER_H
