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
    , currentPhase(UT::Phase::Idle)
{
    if (!eventsCoordinator) {
        eventsCoordinator.reset(EventsCoordinator::instance());
    }

    QObject::connect(this,
                     &Simulator::phaseChanged,
                     eventsCoordinator.get(),
                     &EventsCoordinator::changePhase);

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
    goToNextPhase(UT::Phase::Start);
}

void Simulator::goToNextPhase(UT::Phase nextPhase)
{
    if (nextPhase == currentPhase)
        return;

    currentPhase = nextPhase;
    numOfRoutersDone = 0;

    switch (currentPhase) {
        case UT::Phase::Start:
            start();
            break;
        case UT::Phase::DHCP:
            Q_EMIT phaseChanged(UT::Phase::DHCP);
            break;
        case UT::Phase::Analysis:
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
    eventsCoordinator->setDurationMs(simulationConfig.simulationDurationMs);
    eventsCoordinator->setIntervalMs(simulationConfig.cycleDurationMs);
    eventsCoordinator->setPcs(network.PCs);
    Q_EMIT phaseChanged(UT::Phase::Start);
    goToNextPhase(UT::Phase::DHCP);
}

void Simulator::startDHCP() {}

void Simulator::startIdentifyingNeighbors() {}

void Simulator::startRouting() {}

void Simulator::execution()
{

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
    if (currentPhase == UT::Phase::DHCP)
        Q_EMIT phaseChanged(UT::Phase::IdentifyNeighbors);

    if (currentPhase == UT::Phase::Routing)
        Q_EMIT phaseChanged(UT::Phase::Execution);
}

void Simulator::executionIsDone()
{
    Q_EMIT phaseChanged(UT::Phase::Analysis);
}
