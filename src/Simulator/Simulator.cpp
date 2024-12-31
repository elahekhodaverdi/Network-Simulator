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
    QObject::connect(eventsCoordinator.get(),
                     &EventsCoordinator::neighboursDetectionIsDone,
                     this,
                     &Simulator::neighboursIdentified);
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
        case UT::Phase::IdentifyNeighbours:
            Q_EMIT phaseChanged(UT::Phase::IdentifyNeighbours);
            break;
        case UT::Phase::Routing:
            Q_EMIT phaseChanged(UT::Phase::Routing);
            break;
        case UT::Phase::Execution:
            Q_EMIT phaseChanged(UT::Phase::Execution);
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

void Simulator::analysis()
{
    // TODO: Perform analysis on the simulation results
}

void Simulator::routerIsDone()
{
    numOfRoutersDone++;

    if (currentPhase == UT::Phase::Routing && numOfRoutersDone == 20)
        qDebug() << "done" << numOfRoutersDone;
    if (currentPhase == UT::Phase::DHCP
        && (numOfRoutersDone
            >= (network.numOfRouters() + network.PCs.size() - network.autonomousSystems.size())))
        goToNextPhase(UT::Phase::IdentifyNeighbours);

    else if (currentPhase == UT::Phase::Routing
             && numOfRoutersDone >= (network.numOfRouters() - network.numOfBrokenRouters()))
        goToNextPhase(UT::Phase::Execution);
    else
        return;
    numOfRoutersDone = 0;
}

void Simulator::neighboursIdentified()
{
    goToNextPhase(UT::Phase::Routing);
}

void Simulator::executionIsDone()
{
    goToNextPhase(UT::Phase::Analysis);
}

void Simulator::storeSentPacket(const PacketPtr_t &packet)
{
    packetsSent.append(packet);
    qDebug() << "packet sent" << packetsSent.size();
}

void Simulator::incNumOfPackets(int num)
{
    numOfPackets += num;
}
