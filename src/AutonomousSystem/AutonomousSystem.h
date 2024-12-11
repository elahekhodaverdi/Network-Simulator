#ifndef AUTONOMOUSSYSTEM_H
#define AUTONOMOUSSYSTEM_H

#include <QList>
#include <QString>

#include "../Globals/Globals.h"
#include "../PC/PC.h"
#include "../Router/Router.h"
class AutonomousSystem
{
public:
    int id;
    UT::TopologyType topologyType;
    int nodeCount;
    QList<Router *> asGateways;
    QList<Router *> userGateways;
    Router *dhcpServer;
    QList<Router *> brokenRouters;
    QList<QPair<Router *, QList<PC *>>> connections;
    AutonomousSystem();
    AutonomousSystem(int asId, UT::TopologyType type);
    void build(QJsonObject config);
};

#endif // AUTONOMOUSSYSTEM_H
