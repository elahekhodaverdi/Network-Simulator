#include "Network.h"
#include <QDir>
#include "../Simulator/Simulator.h"

QList<PCPtr_t> Network::PCs;
QList<AutonomousSystemPtr_t> Network::autonomousSystems;

Network::Network()
{
    
}

Network::~Network() {}

int Network::numOfRouters()
{
    int num = 0;
    for (AutonomousSystemPtr_t as : autonomousSystems) {
        num += as->numOfRouters();
    }
    return num;
}

int Network::numOfBrokenRouters()
{
    int num = 0;
    for (AutonomousSystemPtr_t as : autonomousSystems) {
        num += as->numOfBrokenRouters();
    }
    return num;
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

void Network::reset()
{
    for (PCPtr_t pc : PCs) {
        pc.clear();
    }
    PCs.clear();
    for (AutonomousSystemPtr_t as : autonomousSystems) {
        as.clear();
    }
    autonomousSystems.clear();
}

RouterPtr_t Network::findRouterById(int id)
{
    for (const AutonomousSystemPtr_t& as : autonomousSystems) {
        RouterPtr_t router = as->findRouterById(id);
        if (router != nullptr) {
            return router;
        }
    }
    return nullptr;
}

void Network::cleanLogFiles()
{
    for (AutonomousSystemPtr_t as : autonomousSystems)
        as->cleanLogFiles();
}
