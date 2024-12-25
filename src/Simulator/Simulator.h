#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QSharedPointer>
#include "../EventsCoordinator/EventsCoordinator.h"
#include "../Network/Network.h"
#include "../Globals/Globals.h"
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
    //void neighborsIdentified();
    void executionIsDone();

Q_SIGNALS:
    void phaseChanged(UT::Phase nextPhase);

private:
    inline static Simulator *m_self = nullptr;
    QSharedPointer<EventsCoordinator> eventsCoordinator;

    Network network;
    int numOfRoutersDone;
    UT::Phase currentPhase;

    void goToNextPhase(UT::Phase nextPhase);
    void start();
    void startDHCP();
    void startIdentifyingNeighbors();
    void startRouting();
    void execution();
    void analysis();
};

#endif // SIMULATOR_H
