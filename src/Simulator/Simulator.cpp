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

    QObject::connect(this, &Simulator::phaseChanged, this, &Simulator::nextPhase);
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
    Q_EMIT phaseChanged(Phase::Start);
}

void Simulator::nextPhase(Phase nextPhase)
{
    if (nextPhase == currentPhase)
        return;

    currentPhase = nextPhase;

    switch (currentPhase) {
    case Phase::Start:
        start();
        Q_EMIT phaseChanged(Phase::Identification);
        break;
    case Phase::Identification:
        identification();
        break;
    case Phase::Execution:
        execution();
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
}

void Simulator::identification()
{
    // TODO: Start transferring control packets
    // Example: eventsCoordinator->startIdentification();
}

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

void Simulator::aRouterIsDone()
{
    numOfRoutersDone++;
    if (numOfRoutersDone >= network.numOfRouters()) {
        Q_EMIT phaseChanged(Phase::Execution);
    }
}

void Simulator::executionIsDone()
{
    Q_EMIT phaseChanged(Phase::Analysis);
}
