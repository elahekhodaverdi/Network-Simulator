#ifndef NETWORK_H
#define NETWORK_H

#include <QList>
#include <QString>
#include "../AutonomousSystem/AutonomousSystem.h"
#include "../EventsCoordinator/EventsCoordinator.h"
#include "./SimulationConfig.h"
class Network
{
public:
    Network();
    static AutonomousSystem *findASById(int id);
    static void run();

public:
    static QList<PCPtr_t> PCs;
    static SimulationConfig simulationConfig;
    static QList<AutonomousSystem *> autonomousSystems;
    static EventsCoordinator *eventsCoordinator;
};

#endif // NETWORK_H
