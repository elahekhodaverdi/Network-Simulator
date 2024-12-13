#include "AutonomousSystem.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "../Network/Network.h"
#include "../PortBindingManager/PortBindingManager.h"
#include "../Topology/TopologyBuilder.h"

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
                Network::PCs.append(pc);
            }

            connections.append(QPair<RouterPtr_t, QList<PCPtr_t>>(router, pcs));
        } else {
            qWarning() << "Gateway with ID" << gatewayId << "not found in the list of routers.";
        }
    }
}

void AutonomousSystem::setConnectToAS(QJsonArray ASs)
{
    for (const QJsonValue& value : ASs) {
        QJsonObject connectionObject = value.toObject();
        int targetASId = connectionObject["id"].toInt();

        AutonomousSystem* targetAS = Network::findASById(targetASId);

        if (!targetAS) {
            qWarning() << "Target AS with ID" << targetASId << "not found.";
            continue;
        }

        QJsonArray gatewayPairs = connectionObject["gateway_pairs"].toArray();
        for (const QJsonValue& pairValue : gatewayPairs) {
            QJsonObject pairObject = pairValue.toObject();
            int sourceRouterId = pairObject["gateway"].toInt();
            int targetRouterId = pairObject["connect_to"].toInt();

            RouterPtr_t sourceRouter = findRouterById(sourceRouterId);
            RouterPtr_t targetRouter = targetAS->findRouterById(targetRouterId);

            if (sourceRouter && targetRouter) {
                PortPtr_t port1 = sourceRouter->getAnUnboundPort();
                PortPtr_t port2 = targetRouter->getAnUnboundPort();
                PortBindingManager::bind(port1, port2);
            } else {
                qWarning() << "Source Router with ID:" << sourceRouterId
                           << " or Target Router with ID:" << targetRouterId << " not found in AS";
            }
        }
    }
}
