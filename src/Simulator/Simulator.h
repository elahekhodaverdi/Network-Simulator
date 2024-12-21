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

    explicit Simulator(QObject *parent = nullptr);

public:
    ~Simulator() override;

    static Simulator *instance(QObject *parent = nullptr);
    static void release();

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
    inline static Simulator *m_self = nullptr;
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
