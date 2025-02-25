#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QDebug>
#include <QFile>
#include <QObject>
#include <QSet>
#include <QSharedPointer>
#include <QTextStream>
#include <QVector>
#include "../EventsCoordinator/EventsCoordinator.h"
#include "../Globals/Globals.h"
#include "../Network/Network.h"
#include "../Network/SimulationConfig.h"

class Simulator : public QObject
{
    Q_OBJECT

public:
    explicit Simulator(QObject *parent = nullptr);
    ~Simulator() override;

    static Simulator *instance(QObject *parent = nullptr);
    static void release();

    void run();

    static SimulationConfig simulationConfig;

public Q_SLOTS:
    void routerIsDone();
    void neighboursIdentified();
    void executionIsDone();
    void incNumOfPackets(int num);
    void storeSentPacket(const PacketPtr_t &packet);

Q_SIGNALS:
    void phaseChanged(UT::Phase nextPhase);

private:
    inline static Simulator *m_self = nullptr;
    QSharedPointer<EventsCoordinator> eventsCoordinator;

    Network network;
    int numOfRoutersDone = 0;
    UT::Phase currentPhase = UT::Phase::Idle;
    QVector<PacketPtr_t> packetsSent;
    int numOfPackets = 0;

    void goToNextPhase(UT::Phase nextPhase);
    void start();
    void analysis();
    void reset();
    void calculatePacketLoss();
    void calculateAverageHopCount();
    void calculateWaitingCyclesStats();
    void printRoutingTable(const QString &routerId);
    void listUsedRouters();
    void listPoorRouters();
    void listTopRouters();
    void exitSimulation();
    void listTopPCs();
    void cleanLogFiles();
};

#endif // SIMULATOR_H
