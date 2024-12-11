#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QList>
#include <QString>
#include "../AutonomousSystem/AutonomousSystem.h"
#include "./SimluationConfig.h"

class NetworkConfig
{
public:
    SimulationConfig simulationConfig;
    QList<AutonomousSystem> autonomousSystems;
};

#endif // NETWORKCONFIG_H
