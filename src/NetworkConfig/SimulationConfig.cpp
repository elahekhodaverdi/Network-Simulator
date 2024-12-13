#include "SimulationConfig.h"
#include <QDebug>
#include <QRegularExpression>

SimulationConfig::SimulationConfig()
    : simulationDurationMs(0)
    , cycleDurationMs(0)
    , TTL(0)
    , packetsPerSimulation(0)
    , statisticalDistribution("")
    , routerBufferSize(0)
    , routerPortCount(0)
    , routingProtocol("")
    , routingTableUpdateInterval("")
    , routingPerPort(false)
    , routingTableSize(0)
    , routingPacketsPerPortCycle(0)
{}

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
{
    simulationDurationMs = convertTimeToMs(simDuration);
    cycleDurationMs = convertTimeToMs(cycleDuration);
    TTL = ttl;
    packetsPerSimulation = packets;
    statisticalDistribution = statDist;
    routerBufferSize = bufferSize;
    routerPortCount = portCount;
    routingProtocol = routeProtocol;
    routingTableUpdateInterval = routeUpdateInterval;
    routingPerPort = routePerPort;
    routingTableSize = routeTableSize;
    routingPacketsPerPortCycle = packetsPerCycle;
}

int SimulationConfig::convertTimeToMs(const QString &timeString)
{
    QRegularExpression regex("(\\d+)(ms|s)?");
    QRegularExpressionMatch match = regex.match(timeString);

    if (match.hasMatch()) {
        int value = match.captured(1).toInt();
        QString unit = match.captured(2);

        if (unit == "s") {
            return value * 1000;
        } else if (unit == "us") {
            return value / 1000;
        } else if (unit == "ms" || unit.isEmpty()) {
            return value;
        }
    }

    qWarning() << "Invalid time format:" << timeString;
    return 0; // default to 0 if the format is incorrect
}
