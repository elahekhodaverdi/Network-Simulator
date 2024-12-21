#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QSharedPointer>
#include "../EventsCoordinator/EventsCoordinator.h"
#include "../Network/Network.h"
#include "../Network/SimulationConfig.h"

class Simulator : public QObject
{
    Q_OBJECT

public:
    Simulator();
    ~Simulator();

    enum class Phase { Idle, Start, Identification, Execution, Analysis };

    void run();

    static SimulationConfig simulationConfig;

public Q_SLOTS:
    void nextPhase(Simulator::Phase nextPhase);
    void aRouterIsDone();
    void executionIsDone();

Q_SIGNALS:
    void phaseChanged(Simulator::Phase nextPhase);

private:
    QSharedPointer<EventsCoordinator> eventsCoordinator;

    Network network;
    int numOfRoutersDone;
    Phase currentPhase;

    void start();
    void identification();
    void execution();
    void analysis();
};

#endif // SIMULATOR_H
