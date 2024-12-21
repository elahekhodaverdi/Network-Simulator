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
    static QList<IPv4Ptr_t> getAllRoutersIPs();

public:
    static QList<PCPtr_t> PCs;
    static QList<AutonomousSystemPtr_t> autonomousSystems;
};

#endif // NETWORK_H
