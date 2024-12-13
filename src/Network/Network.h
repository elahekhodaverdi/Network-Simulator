#ifndef NETWORK_H
#define NETWORK_H

#include <QList>
#include <QString>
#include "../AutonomousSystem/AutonomousSystem.h"
#include "./SimulationConfig.h"

class Network
{
public:
    static QList<PCPtr_t> PCs;
    static SimulationConfig simulationConfig;
    static QList<AutonomousSystem *> autonomousSystems;
};

#endif // NETWORK_H
