#ifndef AUTONOMOUSSYSTEM_H
#define AUTONOMOUSSYSTEM_H
#include <QJsonObject>
#include <QList>
#include <QSharedPointer>
#include <QString>
#include "../Globals/Globals.h"
#include "../PC/PC.h"
#include "../Router/Router.h"
class AutonomousSystem
{
public:
    AutonomousSystem();
    AutonomousSystem(int asId, UT::TopologyType type);
    void build(QJsonObject config);
    int getID() const;
    QList<IPv4Ptr_t> getAllRoutersIPs();
    int numOfRouters();
    int numOfBrokenRouters();

private:
    int id;
    UT::TopologyType topologyType;
    int nodeCount;
    QList<RouterPtr_t> asGateways;
    QList<RouterPtr_t> userGateways;
    RouterPtr_t dhcpServer;
    QList<RouterPtr_t> brokenRouters;
    QList<QPair<RouterPtr_t, QList<PCPtr_t>>> connections;
    QList<RouterPtr_t> routers;

private:
    RouterPtr_t findRouterById(int id);
    void setASGateaways(QJsonArray ASgateawayIds);
    void setUserGateaways(QJsonArray gateawayIds);
    void setDHCPServer(int dhcpId);
    void setBrokenRouters(QJsonArray gateawayIds);
    void setGateways(QJsonArray gateways);
    void setConnectToAS(QJsonArray ASs);
    void connectRouterSignalsToSimulator();
    void connectPCsSignalsToSimulator(PCPtr_t pc);
};

typedef QSharedPointer<AutonomousSystem> AutonomousSystemPtr_t;
#endif // AUTONOMOUSSYSTEM_H
