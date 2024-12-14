#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QString>
#include "Network/Network.h"
#include "Utils/ConfigReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString projectDir = QString(PROJECT_DIR_PATH);
    QString configFilePath = QDir(projectDir).filePath("assets/config.json");

    ConfigReader::readNetworkConfig(configFilePath);

    // SimulationConfig
    qDebug() << "SimulationConfig:";
    qDebug() << "Duration (ms):" << Network::simulationConfig.simulationDurationMs;
    qDebug() << "Cycle Duration (ms):" << Network::simulationConfig.cycleDurationMs;
    qDebug() << "TTL:" << Network::simulationConfig.TTL;
    qDebug() << "Packets per Simulation:" << Network::simulationConfig.packetsPerSimulation;
    qDebug() << "Statistical Distribution:" << Network::simulationConfig.statisticalDistribution;
    qDebug() << "Router Buffer Size:" << Network::simulationConfig.routerBufferSize;
    qDebug() << "Router Port Count:" << Network::simulationConfig.routerPortCount;
    qDebug() << "Routing Protocol:" << Network::simulationConfig.routingProtocol;
    qDebug() << "Routing Table Update Interval:"
             << Network::simulationConfig.routingTableUpdateInterval;
    qDebug() << "Routing Per Port:" << Network::simulationConfig.routingPerPort;
    qDebug() << "Routing Table Size:" << Network::simulationConfig.routingTableSize;
    qDebug() << "Routing Packets per Port Cycle:"
             << Network::simulationConfig.routingPacketsPerPortCycle;

    // Autonomous Systems
    qDebug() << "\nAutonomous Systems:";
    for (const auto *asSystem : Network::autonomousSystems) {
        qDebug() << "AS ID:" << asSystem->id;
        QString topologyTypeStr;
        switch (asSystem->topologyType) {
        case UT::TopologyType::Mesh:
            topologyTypeStr = "Mesh";
            break;
        case UT::TopologyType::RingStar:
            topologyTypeStr = "RingStar";
            break;
        case UT::TopologyType::Torus:
            topologyTypeStr = "Torus";
            break;
        }

        qDebug() << "Topology Type:" << topologyTypeStr;
        qDebug() << "Node Count:" << asSystem->nodeCount;
        qDebug() << "AS Gateways Count:" << asSystem->asGateways.size();
        qDebug() << "User Gateways Count:" << asSystem->userGateways.size();
        qDebug() << "DHCP Server Set:" << (asSystem->dhcpServer ? "Yes" : "No");
        qDebug() << "Broken Routers Count:" << asSystem->brokenRouters.size();
        qDebug() << "Connections Count:" << asSystem->connections.size();
    }

    return 0;
}
