#ifndef NETWORK_H
#define NETWORK_H

#include <QList>
#include <QString>
#include "../AutonomousSystem/AutonomousSystem.h"
#include "../IP/IP.h"
class Network
{
public:
    Network();
    ~Network();
    static AutonomousSystemPtr_t findASById(int id);
    static RouterPtr_t findRouterById(int id);
    static QList<IPv4Ptr_t> getAllRoutersIPs();
    static int numOfRouters();
    static int numOfBrokenRouters();
    static void reset();
    static void cleanLogFiles();

public:
    static QList<PCPtr_t> PCs;
    static QList<AutonomousSystemPtr_t> autonomousSystems;
};

#endif // NETWORK_H
