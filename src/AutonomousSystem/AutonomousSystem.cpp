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
    routers = TopologyBuilder::buildTopology(nodeCount, topologyType, id);

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


RouterPtr_t AutonomousSystem::findRouterById(int id)
{
    for (auto& router : routers) {
        if (router->getId() == id) {
            return router;
        }
    }
    return nullptr;
}

void AutonomousSystem::setDHCPServer(int dhcpId)
{
    RouterPtr_t router = findRouterById(dhcpId);
    if (router) {
        dhcpServer = router;
    } else {
        qWarning() << "DHCP Server with ID" << dhcpId << "not found in the list of routers.";
    }
}

void AutonomousSystem::setASGateaways(QJsonArray ASgatewayIds)
{
    asGateways.clear();
    for (const QJsonValue& value : ASgatewayIds) {
        int gatewayId = value.toInt();
        RouterPtr_t router = findRouterById(gatewayId);

        if (router) {
            asGateways.append(router);
        } else {
            qWarning() << "AS Gateway with ID" << gatewayId << "not found in the list of routers.";
        }
    }
}


void AutonomousSystem::setUserGateaways(QJsonArray gatewayIds)
{
    userGateways.clear();

    for (const QJsonValue& value : gatewayIds) {
        int gatewayId = value.toInt();
        RouterPtr_t router = findRouterById(gatewayId);

        if (router) {
            userGateways.append(router);
        } else {
            qWarning() << "User Gateway with ID" << gatewayId << "not found in the list of routers.";
        }
    }
}


void AutonomousSystem::setBrokenRouters(QJsonArray brokenRouterIds)
{
    brokenRouters.clear();

    for (const QJsonValue& value : brokenRouterIds) {
        int routerId = value.toInt();
        RouterPtr_t router = findRouterById(routerId);

        if (router) {
            brokenRouters.append(router);
        } else {
            qWarning() << "Broken Router with ID" << routerId << "not found in the list of routers.";
        }
    }
}

void AutonomousSystem::setGateways(QJsonArray gateways)
{
    for (const QJsonValue& value : gateways) {
        QJsonObject gatewayObject = value.toObject();
        int gatewayId = gatewayObject["node"].toInt();
        RouterPtr_t router = findRouterById(gatewayId);

        if (router) {
            QJsonArray userIds = gatewayObject["users"].toArray();
            QList<PCPtr_t> pcs;
            for (const QJsonValue& userValue : userIds) {
                int userId = userValue.toInt();
                PCPtr_t pc = QSharedPointer<PC>::create(userId);

                QString ipString = QString("192.168.%100.%2")
                                     .arg(id)
                                     .arg(userId);

                IPv4Ptr_t ip = QSharedPointer<IPv4_t>::create(ipString);

                pc->setIP(ip);
                pcs.append(pc);
            }

            connections.append(QPair<RouterPtr_t, QList<PCPtr_t>>(router, pcs));
        } else {
            qWarning() << "Gateway with ID" << gatewayId << "not found in the list of routers.";
        }
    }
}


















