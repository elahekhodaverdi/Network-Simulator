#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

#include <QString>

class SimulationConfig
{
public:
    int simulationDurationMs;
    int cycleDurationMs;
    int TTL;
    int packetsPerSimulation;
    QString statisticalDistribution;
    int routerBufferSize;
    int routerPortCount;
    QString routingProtocol;
    QString routingTableUpdateInterval;
    bool routingPerPort;
    int routingTableSize;
    int routingPacketsPerPortCycle;

    SimulationConfig();

    SimulationConfig(const QString &simDuration,
                     const QString &cycleDuration,
                     int ttl,
                     int packets,
                     const QString &statDist,
                     int bufferSize,
                     int portCount,
                     const QString &routeProtocol,
                     const QString &routeUpdateInterval,
                     bool routePerPort,
                     int routeTableSize,
                     int packetsPerCycle);

private:
    int convertTimeToMs(const QString &timeString);
};

#endif // SIMULATIONCONFIG_H
