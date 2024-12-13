#include "AutonomousSystem.h"
#include "../Topology/TopologyBuilder.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

AutonomousSystem::AutonomousSystem() {}

AutonomousSystem::AutonomousSystem(int asId, UT::TopologyType type)
    : id(asId)
    , topologyType(type)
{}
void AutonomousSystem::build(QJsonObject config)
{
    nodeCount = config["node_count"].toInt();
    routers = TopologyBuilder::buildTopology(nodeCount, topologyType);

    auto asGatewayIds = config["as_gateways"].toArray();
    setASGateaways(asGatewayIds);

    auto userGatewayIds = config["user_gateways"].toArray();
    setUserGateaways(userGatewayIds);

    auto brokenRoutersIds = config["broken_routers"].toArray();
    setBrokenRouters(brokenRoutersIds);

    auto gateways = config["gateways"].toArray();
    setGateways(gateways);

    auto connectToAs = config["connect_to_as"].toArray();
    setConnectToAS(connectToAs);

    int dhcpId = config["dhcp_server"].toInt();
    setDHCPServer(dhcpId);
}
