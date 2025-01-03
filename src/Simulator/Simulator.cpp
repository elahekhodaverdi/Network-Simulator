#include "Simulator.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QString>
#include "../Topology/TopologyBuilder.h"
#include "../Utils/ConfigReader.h"
#include <iostream>

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
{
    if (!eventsCoordinator) {
        eventsCoordinator.reset(EventsCoordinator::instance());
    }

    connect(this,
            &Simulator::phaseChanged,
            eventsCoordinator.get(),
            &EventsCoordinator::changePhase);
    connect(eventsCoordinator.get(),
            &EventsCoordinator::executionIsDone,
            this,
            &Simulator::executionIsDone);
    connect(eventsCoordinator.get(),
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

void Simulator::routerIsDone()
{
    numOfRoutersDone++;
    if (currentPhase == UT::Phase::DHCP
        && numOfRoutersDone
               >= network.numOfRouters() + network.PCs.size() - network.autonomousSystems.size()) {
        numOfRoutersDone = 0;
        goToNextPhase(UT::Phase::IdentifyNeighbours);
    } else if (currentPhase == UT::Phase::Routing
               && numOfRoutersDone >= network.numOfRouters() - network.numOfBrokenRouters()) {
        numOfRoutersDone = 0;
        goToNextPhase(UT::Phase::Execution);
    }
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
}

void Simulator::incNumOfPackets(int num)
{
    numOfPackets += num;
}

void Simulator::analysis()
{
    std::string input;
    while (true) {
        qDebug() << "Enter command for analysis (or 'Exit' to end):";
        std::getline(std::cin, input);

        QString commandLine = QString::fromStdString(input).trimmed();

        if (commandLine.isEmpty()) {
            continue;
        }

        if (commandLine == "Exit") {
            exitSimulation();
            return;
        } else if (commandLine == "Reset") {
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
        } else if (commandLine == "Top-routers") {
            listTopRouters();
        } else {
            qDebug() << "Unknown command.";
        }
    }
}

void Simulator::reset()
{
    qDebug() << "Resetting simulation.";
    network.reset();
    numOfPackets = 0;
    TopologyBuilder::reset();
    EventsCoordinator::instance()->reset();
    packetsSent.clear();
    goToNextPhase(UT::Phase::Start);
}

void Simulator::calculatePacketLoss()
{
    if (numOfPackets == 0) {
        qDebug() << "No packets sent, cannot calculate packet loss.";
        return;
    }
    qDebug() << "Packet loss percentage:"
             << (100.0 * (numOfPackets - packetsSent.size()) / numOfPackets) << "%"
             << " Total packets:" << numOfPackets;
}

void Simulator::calculateAverageHopCount()
{
    if (packetsSent.isEmpty()) {
        qDebug() << "No packets sent, cannot calculate average hop count.";
        return;
    }

    double totalHops = 0;
    for (const PacketPtr_t &packet : packetsSent) {
        totalHops += packet->path().size();
    }

    qDebug() << "Average hop count:" << (totalHops / packetsSent.size());
}

void Simulator::calculateWaitingCyclesStats()
{
    if (packetsSent.isEmpty()) {
        qDebug() << "No packets available for statistics.";
        return;
    }

    qint64 totalWaitingCycles = 0;
    int minWaitingCycles = INT_MAX, maxWaitingCycles = INT_MIN;

    for (const PacketPtr_t &packet : packetsSent) {
        int waitingCycles = packet->waitingCycles();
        totalWaitingCycles += waitingCycles;

        minWaitingCycles = qMin(minWaitingCycles, waitingCycles);
        maxWaitingCycles = qMax(maxWaitingCycles, waitingCycles);
    }

    qDebug() << "Waiting Cycles Stats: Total:" << totalWaitingCycles << "Min:" << minWaitingCycles
             << "Max:" << maxWaitingCycles
             << "Avg:" << (static_cast<double>(totalWaitingCycles) / packetsSent.size());
}

void Simulator::printRoutingTable(const QString &routerId)
{
    qDebug() << "Routing table for router:" << routerId.toInt();
    RouterPtr_t router = network.findRouterById(routerId.toInt());
    router->printRoutingTable();
}

void Simulator::listUsedRouters()
{
    QSet<QString> usedRouters;
    for (const PacketPtr_t &packet : packetsSent) {
        for (const QString &ip : packet->path()) {
            usedRouters.insert(ip);
        }
    }

    qDebug() << "Routers used in simulation:" << usedRouters;
}

void Simulator::listPoorRouters()
{
    QSet<QString> allRouters, usedRouters;
    for (const IPv4Ptr_t &ip : network.getAllRoutersIPs()) {
        allRouters.insert(ip->toString());
    }

    for (const PacketPtr_t &packet : packetsSent) {
        for (const QString &ip : packet->path()) {
            usedRouters.insert(ip);
        }
    }

    QSet<QString> poorRouters = allRouters - usedRouters;
    qDebug() << "Routers not used during simulation:" << poorRouters;
}

void Simulator::listTopRouters()
{
    QMap<QString, int> routerUsage;

    for (const PacketPtr_t &packet : packetsSent) {
        if (packet) {
            for (const QString &ip : packet->path()) {
                routerUsage[ip]++;
            }
        }
    }

    QList<QPair<QString, int>> usageList;
    for (auto it = routerUsage.constBegin(); it != routerUsage.constEnd(); ++it) {
        usageList.append(qMakePair(it.key(), it.value()));
    }

    std::sort(usageList.begin(), usageList.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    qDebug() << "Top routers by packet deliveries:";
    for (int i = 0; i < qMin(4, usageList.size()); ++i) {
        qDebug() << "Router:" << usageList[i].first << "Packets:" << usageList[i].second;
    }
}

void Simulator::exitSimulation()
{
    qDebug() << "Exiting simulation...";
    EventsCoordinator::instance()->cleanup();
    QCoreApplication::quit();
}
