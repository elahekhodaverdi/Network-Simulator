#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "../Network/Network.h"

class ConfigReader
{
public:
    static void readNetworkConfig(const QString &pathToConfFile);

private:
    static QJsonObject readJson(const QString &filePath);
    static SimulationConfig parseSimulationConfig(const QJsonObject &config);
    static void parseAutonomousSystems(const QJsonArray &systemsArray);
    static UT::TopologyType stringToTopologyType(const QString &typeString);
    static QList<UT::TopologyType> parseTopologyTypes(const QString &topologyTypeString);
};

#endif // CONFIGREADER_H
