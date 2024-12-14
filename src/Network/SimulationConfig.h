#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

#include <QString>

class SimulationConfig
{
public:
    static int simulationDurationMs;
    static int cycleDurationMs;
    static int TTL;
    static int packetsPerSimulation;
    static QString statisticalDistribution;
    static int routerBufferSize;
    static int routerPortCount;
    static QString routingProtocol;
    static int routingTableUpdateInterval;
    static bool routingPerPort;
    static int routingTableSize;
    static int routingPacketsPerPortCycle;

    SimulationConfig();

    SimulationConfig(const QString &simDuration,
                     const QString &cycleDuration,
                     int ttl,
                     int packets,
                     const QString &statDist,
                     int bufferSize,
                     int portCount,
                     const QString &routeProtocol,
                     int routeUpdateInterval,
                     bool routePerPort,
                     int routeTableSize,
                     int packetsPerCycle);

private:
    int convertTimeToMs(const QString &timeString);
};

#endif // SIMULATIONCONFIG_H
