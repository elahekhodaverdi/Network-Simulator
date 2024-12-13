#ifndef NETWORK_H
#define NETWORK_H

#include <QList>
#include <QString>
#include "../AutonomousSystem/AutonomousSystem.h"
#include "./SimulationConfig.h"

class NetworkConfig
{
public:
    SimulationConfig simulationConfig;
    QList<AutonomousSystem *> autonomousSystems;
};

#endif // NETWORK_H
