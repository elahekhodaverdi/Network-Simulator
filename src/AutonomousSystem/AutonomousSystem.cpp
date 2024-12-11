#include "AutonomousSystem.h"

AutonomousSystem::AutonomousSystem() {}

AutonomousSystem::AutonomousSystem(int asId,
                                   const QString &topoType,
                                   int nodes,
                                   const QList<int> &asGates,
                                   const QList<int> &userGates,
                                   int dhcp,
                                   const QList<int> &broken,
                                   const QList<QPair<int, QList<QPair<int, int>>>> &connects)
    : id(asId)
    , topologyType(topoType)
    , nodeCount(nodes)
    , asGateways(asGates)
    , userGateways(userGates)
    , dhcpServer(dhcp)
    , brokenRouters(broken)
    , connections(connects)
{}
