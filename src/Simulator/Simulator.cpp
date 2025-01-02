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

    // if (!commandFile.exists()) {
    //     qDebug() << "Error: Command file does not exist.";
    //     return;
    // }

    // if (!commandFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     qDebug() << "Error: Unable to open command file.";
    //     return;
    // }

    // commandStream.setDevice(&commandFile);
    // isFileOpen = true;

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
    if (commandFile.isOpen()) {
        commandFile.close();
        qDebug() << "Command file closed.";
    }
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
            qDebug() << "DHCP";
            Q_EMIT phaseChanged(UT::Phase::DHCP);
            break;
        case UT::Phase::IdentifyNeighbours:
            qDebug() << "Identify";
            Q_EMIT phaseChanged(UT::Phase::IdentifyNeighbours);
            break;
        case UT::Phase::Routing:
            qDebug() << "Routing";
            Q_EMIT phaseChanged(UT::Phase::Routing);
            break;
        case UT::Phase::Execution:
            qDebug() << "Execution";
            Q_EMIT phaseChanged(UT::Phase::Execution);
            break;
        case UT::Phase::Analysis:
            qDebug() << "Analysis";
            reset();
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
    // qDebug() << "packet sent" << packetsSent.size();
}

void Simulator::incNumOfPackets(int num)
{
    numOfPackets += num;
}

void Simulator::analysis()
{
    if (!isFileOpen) {
        qDebug() << "Error: Command file is not open.";
        return;
    }

    commandStream.seek(lastCommandFilePosition);

    while (!commandStream.atEnd()) {
        QString commandLine = commandStream.readLine().trimmed();

        if (commandLine.isEmpty()) {
            continue;
        }

        qDebug() << "Processing command:" << commandLine;

        if (commandLine == "Reset") {
            qDebug() << "Simulation reset. Exiting analysis phase.";
            lastCommandFilePosition = commandStream.pos();
            reset();
            return;
        } else if (commandLine == "Packet-loss") {
            calculatePacketLoss();
        } else if (commandLine == "Hop-count-avg") {
            calculateAverageHopCount();
        } else if (commandLine == "Waiting-cycles-stat") {
            calculateWaitingCyclesStats();
        } else if (commandLine.startsWith("Print-routing-table")) {
            QString routerId = commandLine.section(' ', 1, 1).trimmed();
            printRoutingTable(routerId);
        } else if (commandLine == "Used-routers") {
            listUsedRouters();
        } else if (commandLine == "Poor-routers") {
            listPoorRouters();
        } else if (commandLine == "Clean") {
            exitSimulation();
            return;
        } else if (commandLine == "Exit") {
            exitSimulation();
            return;
        } else {
            qDebug() << "Unknown command";
        }
    }

    lastCommandFilePosition = commandStream.pos();
    qDebug() << "Finished processing all commands in the file.";
}

void Simulator::reset()
{
    // packetsSent.clear();
    qDebug() << "Reseting the process";
    lastCommandFilePosition = 0;
    network.reset();
}

void Simulator::calculatePacketLoss()
{
    qDebug() << "We have" << (packetsSent.size() / numOfPackets) << "percentage packet loss.";
}

void Simulator::calculateAverageHopCount()
{
    double sum = 0;
    for (PacketPtr_t packet : packetsSent)
        sum += packet->path().size();
    qDebug() << "Average hop count is equal to:" << (sum / packetsSent.size());
}

void Simulator::calculateWaitingCyclesStats()
{
    if (packetsSent.isEmpty()) {
        qDebug() << "No packets available for statistics.";
        return;
    }

    qint64 totalWaitingCycles = 0;
    int minWaitingCycles = INT_MAX;
    int maxWaitingCycles = INT_MIN;

    for (PacketPtr_t packet : packetsSent) {
        int waitingCycles = packet->waitingCycles();
        totalWaitingCycles += waitingCycles;

        if (waitingCycles < minWaitingCycles) {
            minWaitingCycles = waitingCycles;
        }
        if (waitingCycles > maxWaitingCycles) {
            maxWaitingCycles = waitingCycles;
        }
    }

    double averageWaitingCycles = static_cast<double>(totalWaitingCycles) / packetsSent.size();

    qDebug() << "Total Waiting Cycles:" << totalWaitingCycles;
    qDebug() << "Minimum Waiting Cycles:" << minWaitingCycles;
    qDebug() << "Maximum Waiting Cycles:" << maxWaitingCycles;
    qDebug() << "Average Waiting Cycles:" << averageWaitingCycles;
}

void Simulator::printRoutingTable(const QString &routerId)
{
    qDebug() << "Printing routing table for router:" << routerId;
}

void Simulator::listUsedRouters()
{
    QSet<QString> usedRouters;

    for (const PacketPtr_t &packet : packetsSent) {
        if (packet) {
            const QList<QString> &path = packet->path();
            for (const QString &ip : path) {
                usedRouters.insert(ip);
            }
        }
    }

    qDebug() << "List of routers used during the simulation:";
    for (const QString &routerIp : usedRouters) {
        qDebug() << routerIp;
    }
}

void Simulator::listPoorRouters()
{
    QList<IPv4Ptr_t> allRouterPtrs = network.getAllRoutersIPs();
    QSet<QString> allRouters;
    for (const IPv4Ptr_t &ipPtr : allRouterPtrs) {
        if (ipPtr) {
            allRouters.insert(ipPtr->toString());
        }
    }

    QSet<QString> usedRouters;
    for (const PacketPtr_t &packet : packetsSent) {
        if (packet) {
            const QList<QString> &path = packet->path();
            for (const QString &ip : path) {
                usedRouters.insert(ip);
            }
        }
    }

    QSet<QString> poorRouters = allRouters - usedRouters;

    qDebug() << "List of poor routers (not used during the simulation):";
    for (const QString &routerIp : poorRouters) {
        qDebug() << routerIp;
    }
}

void Simulator::exitSimulation()
{
    qDebug() << "Exiting simulation.";
}
