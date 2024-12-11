#ifndef AUTONOMOUSSYSTEM_H
#define AUTONOMOUSSYSTEM_H
#include <QJsonObject>
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
    QList<Router *> routers;
    AutonomousSystem();
    AutonomousSystem(int asId, UT::TopologyType type);
    void build(QJsonObject config);

private:
    void setASGateaways(QJsonArray ASgateawayIds);
    void setUserGateaways(QJsonArray gateawayIds);
    void setDHCPServer(int dhcpId);
    void setBrokenRouters(QJsonArray gateawayIds);
    void setGateaways(QJsonArray gateaways);
    void connectToAS(QJsonArray ASs);
};

#endif // AUTONOMOUSSYSTEM_H
