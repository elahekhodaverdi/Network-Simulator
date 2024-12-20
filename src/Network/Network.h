#ifndef NETWORK_H
#define NETWORK_H

#include <QList>
#include <QString>
#include "../AutonomousSystem/AutonomousSystem.h"
#include "../EventsCoordinator/EventsCoordinator.h"
#include "../IP/IP.h"
#include "./SimulationConfig.h"
class Network
{
public:
    Network();
    ~Network();
    static AutonomousSystemPtr_t findASById(int id);
    static void run();
    static QList<IPv4Ptr_t> getAllRoutersIPs();

public:
    static QList<PCPtr_t> PCs;
    static SimulationConfig simulationConfig;
    static QList<AutonomousSystemPtr_t> autonomousSystems;
    static EventsCoordinator *eventsCoordinator;
};

#endif // NETWORK_H
