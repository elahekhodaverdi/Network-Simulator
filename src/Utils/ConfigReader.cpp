#include "ConfigReader.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QStringList>
#include <QVariant>

QJsonObject ConfigReader::readJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open the file.");
        return QJsonObject();
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        qWarning("Invalid JSON format.");
        return QJsonObject();
    }

    return doc.object();
}

Network ConfigReader::readNetwork(const QString &pathToConfFile)
{
    QJsonObject actualObj = readJson(pathToConfFile);

    QJsonObject generalSettings = actualObj;
    generalSettings.remove("Autonomous_systems");
    QJsonArray autonomousSystems = actualObj.value("Autonomous_systems").toArray();

    Network network;
    network.simulationConfig = parseSimulationConfig(generalSettings);
    parseAutonomousSystems(autonomousSystems);

    return network;
}

SimulationConfig ConfigReader::parseSimulationConfig(const QJsonObject &config)
{
    return SimulationConfig(config["simulation_duration"].toString(),
                            config["cycle_duration"].toString(),
                            config["TTL"].toInt(),
                            config["packets_per_simulation"].toInt(),
                            config["statistical_distribution"].toString(),
                            config["router_buffer_size"].toInt(),
                            config["router_port_count"].toInt(),
                            config["routing_protocol"].toString(),
                            config["routing_table_update_interval"].toString(),
                            config["routing_per_port"].toBool(),
                            config["routing_table_size"].toInt(),
                            config["routing_packets_per_port_cycle"].toInt());
}

void ConfigReader::parseAutonomousSystems(const QJsonArray &systemsArray)
{
    for (const QJsonValue &jsonValue : systemsArray) {
        if (jsonValue.isObject()) {
            QJsonObject ASsystem = jsonValue.toObject();

            QString topologyTypeString = ASsystem["topology_type"].toString();
            QList<UT::TopologyType> topologyTypes = parseTopologyTypes(topologyTypeString);

            UT::TopologyType primaryTopologyType = !topologyTypes.isEmpty()
                                                       ? topologyTypes.first()
                                                       : UT::TopologyType::Mesh;

            AutonomousSystem *system = new AutonomousSystem(ASsystem["id"].toInt(),
                                                            primaryTopologyType);
            system->build(ASsystem);
            Network::autonomousSystems.append(system);
        }
    }
}

QList<UT::TopologyType> ConfigReader::parseTopologyTypes(const QString &topologyTypeString)
{
    QList<UT::TopologyType> topologyTypes;

    QStringList topologyList = topologyTypeString.split('|', Qt::SkipEmptyParts);
    for (QString typeString : topologyList) {
        topologyTypes.append(stringToTopologyType(typeString));
    }
    return topologyTypes;
}

UT::TopologyType ConfigReader::stringToTopologyType(const QString &typeString)
{
    if (typeString == "Mesh") {
        return UT::TopologyType::Mesh;
    } else if (typeString == "RingStar") {
        return UT::TopologyType::RingStar;
    } else if (typeString == "Torus") {
        return UT::TopologyType::Torus;
    }
    return UT::TopologyType::Mesh;
}
