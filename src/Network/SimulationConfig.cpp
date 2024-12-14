#include "SimulationConfig.h"
#include <QDebug>
#include <QRegularExpression>

int SimulationConfig::simulationDurationMs = 0;
int SimulationConfig::cycleDurationMs = 0;
int SimulationConfig::TTL = 0;
int SimulationConfig::packetsPerSimulation = 0;
QString SimulationConfig::statisticalDistribution = "";
int SimulationConfig::routerBufferSize = 0;
int SimulationConfig::routerPortCount = 0;
QString SimulationConfig::routingProtocol = "";
QString SimulationConfig::routingTableUpdateInterval = "";
bool SimulationConfig::routingPerPort = false;
int SimulationConfig::routingTableSize = 0;
int SimulationConfig::routingPacketsPerPortCycle = 0;

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
        } else if (unit == "ms" || unit.isEmpty()) {
            return value;
        }
    }

    qWarning() << "Invalid time format:" << timeString;
    return 0;
}
