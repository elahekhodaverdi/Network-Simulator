#include "Network.h"
#include <QDir>
#include "../Simulator/Simulator.h"

QList<PCPtr_t> Network::PCs;
QList<AutonomousSystemPtr_t> Network::autonomousSystems;

Network::Network()
{
    
}

Network::~Network()
{
        
}
AutonomousSystemPtr_t Network::findASById(int id)
{
    AutonomousSystemPtr_t target = nullptr;
    for (AutonomousSystemPtr_t as : autonomousSystems) {
        if (as->getID() == id) {
            target = as;
            break;
        }
    }
    return target;
}

QList<IPv4Ptr_t> Network::getAllRoutersIPs()
{
    QList<IPv4Ptr_t> allRoutersIPs;
    for (AutonomousSystemPtr_t AS : autonomousSystems) {
        allRoutersIPs.append(AS->getAllRoutersIPs());
    }
    return allRoutersIPs;
}
