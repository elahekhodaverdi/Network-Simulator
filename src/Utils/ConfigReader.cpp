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

QList<AutonomousSystem> ConfigReader::parseAutonomousSystems(const QJsonArray &systemsArray)
{
    QList<AutonomousSystem> systems;

    return systems;
}

QList<QMap<QString, QVariant>> ConfigReader::convertASJsonArrayToQMap(const QJsonArray &jsonArray)
{
    QList<QMap<QString, QVariant>> result;

    for (const QJsonValue &jsonValue : jsonArray) {
        if (jsonValue.isObject()) {
            QMap<QString, QVariant> mappedJson = convertASJsonToQMap(jsonValue.toObject());
            result.append(mappedJson);
        }
    }

    return result;
}

QMap<QString, QVariant> ConfigReader::convertASJsonToQMap(const QJsonObject &jsonObj)
{
    QMap<QString, QVariant> map;

    for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it) {
        const QString &key = it.key();
        const QJsonValue &value = it.value();

        if (value.isArray()) {
            // Handle arrays (e.g., gateways, as_gateways)
            if (key == "gateways") {
                QVariantList gatewayList;
                for (const QJsonValue &gatewayValue : value.toArray()) {
                    QMap<QString, QVariant> gatewayMap;
                    if (gatewayValue.isObject()) {
                        QJsonObject gatewayObj = gatewayValue.toObject();
                        gatewayMap["node"] = gatewayObj["node"].toInt();
                        gatewayMap["users"] = gatewayObj["users"].toArray().toVariantList();
                    }
                    gatewayList.append(gatewayMap);
                }
                map[key] = gatewayList; // Use QVariantList instead of QList<QMap<...>>
            } else if (key == "connect_to_as") {
                QVariantList connectList;
                for (const QJsonValue &connectValue : value.toArray()) {
                    QMap<QString, QVariant> connectMap;
                    if (connectValue.isObject()) {
                        QJsonObject connectObj = connectValue.toObject();
                        connectMap["id"] = connectObj["id"].toInt();

                        QVariantList gatewayPairs;
                        QJsonArray gatewayPairsArray = connectObj["gateway_pairs"].toArray();
                        for (const QJsonValue &pairValue : gatewayPairsArray) {
                            QMap<QString, QVariant> pairMap;
                            if (pairValue.isObject()) {
                                QJsonObject pairObj = pairValue.toObject();
                                pairMap["gateway"] = pairObj["gateway"].toInt();
                                pairMap["connect_to"] = pairObj["connect_to"].toInt();
                            }
                            gatewayPairs.append(pairMap);
                        }
                        connectMap["gateway_pairs"] = gatewayPairs;
                    }
                    connectList.append(connectMap);
                }
                map[key] = connectList; // Use QVariantList instead of QList<QMap<...>>
            } else {
                map[key] = value.toArray().toVariantList();
            }
        } else if (value.isString()) {
            QString stringValue = value.toString();
            if (stringValue.contains("|")) {
                map[key] = stringValue.split("|");
            } else {
                map[key] = stringValue;
            }
        } else if (value.isDouble() || value.isBool()) {
            map[key] = value.toVariant();
        } else if (value.isNull()) {
            map[key] = QVariant(QVariant::Invalid);
        }
    }
    return map;
}
