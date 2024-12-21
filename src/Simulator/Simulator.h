#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../EventsCoordinator/EventsCoordinator.h"
#include "../Network/Network.h"
#include "../Network/SimulationConfig.h"
class Simulator
{
public:
    Simulator();
    ~Simulator();

    void run();

    static SimulationConfig simulationConfig;

public Q_SLOTS:
    void nextTick();

private:
    enum class Phase { Start, Identification, Execution, Analysis };

    // TODO: I think it should be QSharedPointer
    static EventsCoordinator *eventsCoordinator;

    Network network;
    int tick;
    int routerDone; //TODO: rename later
    Phase currentPhase;

    void start();
    void identification();
    void execution();
    void analysis();
};

#endif // SIMULATOR_H
