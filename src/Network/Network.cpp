#include "Network.h"
#include "../Utils/ConfigReader.h"

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
    ConfigReader::readNetworkConfig("E:/elahe/git/CN_CHomeworks_3/assets/config.json");
    eventsCoordinator->startSimulation(SimulationConfig::cycleDurationMs,
                                       SimulationConfig::simulationDurationMs,
                                       PCs);
}
