#include "Network.h"
#include <QDir>
#include "../Utils/ConfigReader.h"
QList<PCPtr_t> Network::PCs;
SimulationConfig Network::simulationConfig;
QList<AutonomousSystem *> Network::autonomousSystems;
EventsCoordinator *Network::eventsCoordinator = nullptr;

Network::Network()
{
    eventsCoordinator = EventsCoordinator::instance();
}

Network::~Network()
{
    eventsCoordinator->release();
}
AutonomousSystem *Network::findASById(int id)
{
    AutonomousSystem *target = nullptr;
    for (AutonomousSystem *as : autonomousSystems) {
        if (as->id == id) {
            target = as;
            break;
        }
    }
    return target;
}

void Network::run()
{
    QString projectDir = QString(PROJECT_DIR_PATH);
    QString configFilePath = QDir(projectDir).filePath("assets/config.json");
    ConfigReader::readNetworkConfig(configFilePath);

    eventsCoordinator->startSimulation(SimulationConfig::cycleDurationMs,
                                       SimulationConfig::simulationDurationMs,
                                       PCs);
}
