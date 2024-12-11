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

NetworkConfig ConfigReader::readNetworkConfig(const QString &pathToConfFile)
{
    QJsonObject actualObj = readJson(pathToConfFile);

    QJsonObject generalSettings = actualObj;
    generalSettings.remove("Autonomous_systems");
    QJsonArray autonomousSystems = actualObj.value("Autonomous_systems").toArray();

    NetworkConfig networkConfig;
    networkConfig.simulationConfig = parseSimulationConfig(generalSettings);
    networkConfig.autonomousSystems = parseAutonomousSystems(autonomousSystems);

    return networkConfig;
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

QList<AutonomousSystem *> ConfigReader::parseAutonomousSystems(const QJsonArray &systemsArray)
{
    QList<AutonomousSystem *> systems;
    for (const QJsonValue &jsonValue : systemsArray) {
        if (jsonValue.isObject()) {
            QJsonObject ASsystem = jsonValue.toObject();
            AutonomousSystem *system = new AutonomousSystem(ASsystem["id"].toInt(),
                                                            UT::TopologyType::Mesh);
            system->build(ASsystem);
            systems.append(system);
        }
    }
    return systems;
}
