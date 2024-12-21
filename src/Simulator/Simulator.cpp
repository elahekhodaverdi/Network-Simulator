#include "Simulator.h"
#include <QDir>
#include "../Utils/ConfigReader.h"
Simulator::Simulator()
{
    currentPhase = Phase::Start;
    tick = 0;
    eventsCoordinator = EventsCoordinator::instance();
    QObject::connect(eventsCoordinator, &EventsCoordinator::nextTick, this, &Simulator::nextTick);
}

void Simulator::nextTick()
{
    switch (currentPhase) {
    case Simulator::Phase::Start:

        break;
    case Simulator::Phase::Identification:

        break;
    case Simulator::Phase::Execution:

        break;
    case Simulator::Phase::Analysis:

        break;
    default:
        break;
    }
    tick++;
}
Simulator::~Simulator()
{
    eventsCoordinator->release();
}
void Simulator::start()
{
    QString projectDir = QString(PROJECT_DIR_PATH);
    QString configFilePath = QDir(projectDir).filePath("assets/config.json");
    ConfigReader::readNetworkConfig(configFilePath);
}
void Simulator::identification()
{
    // TODO: start trasnferrring control packet
    // eventsCoordinator->startSimulation
}
void Simulator::execution()
{
    eventsCoordinator->startSimulation(simulationConfig.cycleDurationMs,
                                       simulationConfig.simulationDurationMs,
                                       network.PCs);
}
void Simulator::analysis()
{
    // TODO: reading input commands
}
