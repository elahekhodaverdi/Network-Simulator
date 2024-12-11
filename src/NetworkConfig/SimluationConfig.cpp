#include "SimluationConfig.h"

SimulationConfig::SimulationConfig() {}

SimulationConfig::SimulationConfig(const QString &simDuration,
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
                                   int packetsPerCycle)
    : simulationDuration(simDuration)
    , cycleDuration(cycleDuration)
    , TTL(ttl)
    , packetsPerSimulation(packets)
    , statisticalDistribution(statDist)
    , routerBufferSize(bufferSize)
    , routerPortCount(portCount)
    , routingProtocol(routeProtocol)
    , routingTableUpdateInterval(routeUpdateInterval)
    , routingPerPort(routePerPort)
    , routingTableSize(routeTableSize)
    , routingPacketsPerPortCycle(packetsPerCycle)
{}
