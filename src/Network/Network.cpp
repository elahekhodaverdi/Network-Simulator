#include "Network.h"
#include <QDir>
#include "../Utils/ConfigReader.h"
QList<PCPtr_t> Network::PCs;
SimulationConfig Network::simulationConfig;
QList<AutonomousSystemPtr_t> Network::autonomousSystems;
EventsCoordinator *Network::eventsCoordinator = nullptr;

Network::Network()
{
    eventsCoordinator = EventsCoordinator::instance();
}

Network::~Network()
{
    eventsCoordinator->release();
}
AutonomousSystemPtr_t Network::findASById(int id)
{
    AutonomousSystemPtr_t target = nullptr;
    for (AutonomousSystemPtr_t as : autonomousSystems) {
        if (as->getID() == id) {
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
