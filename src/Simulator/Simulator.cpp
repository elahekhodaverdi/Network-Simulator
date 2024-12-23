#include "Simulator.h"
#include <QDir>
#include "../Utils/ConfigReader.h"

SimulationConfig Simulator::simulationConfig;

Simulator *Simulator::instance(QObject *parent)
{
    if (!m_self) {
        m_self = new Simulator(parent);
    }
    return m_self;
}

void Simulator::release()
{
    delete m_self;
    m_self = nullptr;
}

Simulator::Simulator(QObject *parent)
    : QObject(parent)
    , numOfRoutersDone(0)
    , currentPhase(Phase::Idle)
{
    if (!eventsCoordinator) {
        eventsCoordinator.reset(EventsCoordinator::instance());
    }

    QObject::connect(this, &Simulator::phaseChanged, eventsCoordinator.get(), &EventsCoordinator::]);
    QObject::connect(eventsCoordinator.get(),
                     &EventsCoordinator::executionIsDone,
                     this,
                     &Simulator::executionIsDone);
}

Simulator::~Simulator()
{
    eventsCoordinator.clear();
}

void Simulator::run()
{
    goToNextPhase(Phase::Start);
}

void Simulator::goToNextPhase(Phase nextPhase)
{
    if (nextPhase == currentPhase)
        return;

    currentPhase = nextPhase;
    numOfRoutersDone = 0;

    Q_EMIT phaseChanged(currentPhase);
    switch (currentPhase) {
    case Phase::Start:
        start();
        break;
    case Phase::Analysis:
        analysis();
        break;
    default:
        break;
    }
}

void Simulator::start()
{
    QString projectDir = QString(PROJECT_DIR_PATH);
    QString configFilePath = QDir(projectDir).filePath("assets/config.json");
    ConfigReader::readNetworkConfig(configFilePath);
    goToNextPhase(Phase::DHCP);
}

void Simulator::startDHCP() {}

void Simulator::startIdentifyingNeighbors() {}

void Simulator::startRouting() {}

void Simulator::execution()
{
    eventsCoordinator->startSimulation(simulationConfig.cycleDurationMs,
                                       simulationConfig.simulationDurationMs,
                                       network.PCs);
}

void Simulator::analysis()
{
    // TODO: Perform analysis on the simulation results
}

void Simulator::routerIsDone()
{
    numOfRoutersDone++;
    if (numOfRoutersDone < network.numOfRouters())
        return;
    numOfRoutersDone = 0;
    if (currentPhase == Phase::DHCP)
        Q_EMIT phaseChanged(Phase::IdentifyNeighbors);

    if (currentPhase == Phase::Routing)
        Q_EMIT phaseChanged(Phase::Execution);
}

void Simulator::executionIsDone()
{
    Q_EMIT phaseChanged(Phase::Analysis);
}
