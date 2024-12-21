#include "OSPF.h"
#include "../Network/Network.h"
#include <limits>
#include <queue>
#include <tuple>

OSPF::OSPF(QObject *parent)
    : RoutingProtocol{parent}
{}

OSPF::OSPF(QString routerIP, QObject *parent)
    : RoutingProtocol{routerIP, parent}
{}

void OSPF::initialize()
{
    for (IPv4Ptr_t routerIP : Network::getAllRoutersIPs()) {
        shortestPathTree[routerIP] = (routerIP == m_routerIP) ? 0 : INF;
    }
}

void OSPF::addNewNeighbor(const IPv4Ptr_t &neighborIP, PortPtr_t outPort)
{
    if (!neighbors.contains(neighborIP)) {
        neighbors.append(neighborIP);
        shortestPathTree[neighborIP] = 1;
    }
}

LSA OSPF::createOwnLSA()
{
    LSA lsa;
    lsa.routerIP = m_routerIP;
    lsa.neighborIPs = neighbors;
    return lsa;
}

void OSPF::sendOwnLSAPacket(const QByteArray &lsaData)
{
    LSA lsa = createOwnLSA();
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader(), this);
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setSourceIp(m_routerIP);
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::OSPF);
    packet->setPayload(lsa.toByteArray());
    Q_EMIT NewOutgoingRoutingPacket(packet, nullptr);
}

void OSPF::processRoutingPacket(const PacketPtr_t &packet, PortPtr_t outPort)
{
    QByteArray lsaData = packet->payload();

    LSA lsa = LSA::fromByteArray(lsaData);

    if (!LSDB.contains(lsa.routerIP)) {
        Q_EMIT NewOutgoingRoutingPacket(packet, outPort);
    }

    LSDB[lsa.routerIP] = lsa;

    calculateSPF();
}

void OSPF::calculateSPF()
{
    QMap<IPv4Ptr_t, int> distance;
    QMap<IPv4Ptr_t, IPv4Ptr_t> previousHop;

    for (const auto &routerIP : shortestPathTree.keys()) {
        distance[routerIP] = shortestPathTree[routerIP];
    }

    auto compare = [](const std::pair<int, IPv4Ptr_t> &a, const std::pair<int, IPv4Ptr_t> &b) {
        return a.first > b.first;
    };

    std::priority_queue<std::pair<int, IPv4Ptr_t>, QList<std::pair<int, IPv4Ptr_t>>, decltype(compare)>
        pq(compare);

    pq.push({0, m_routerIP});

    while (!pq.empty()) {
        auto [currentCost, currentRouter] = pq.top();
        pq.pop();

        if (currentCost > distance[currentRouter])
            continue;

        if (LSDB.contains(currentRouter)) {
            const auto &lsa = LSDB[currentRouter];

            for (const auto &neighborIP : lsa.neighborIPs) {
                int newCost = currentCost + 1;
                if (newCost < distance[neighborIP]) {
                    distance[neighborIP] = newCost;
                    previousHop[neighborIP] = currentRouter;
                    pq.push({newCost, neighborIP});
                }
            }
        }
    }

    shortestPathTree = distance;
    buildRoutingTable(previousHop);
}

void OSPF::buildRoutingTable(const QMap<IPv4Ptr_t, IPv4Ptr_t> &previousHop)
{
    QMap<IPv4Ptr_t, std::pair<int, IPv4Ptr_t>> routingTable;

    for (const auto &destination : shortestPathTree.keys()) {
        if (destination == m_routerIP)
            continue;

        int cost = shortestPathTree[destination];
        IPv4Ptr_t nextHop = destination;

        while (previousHop.contains(nextHop) && previousHop[nextHop] != m_routerIP) {
            nextHop = previousHop[nextHop];
        }

        routingTable[destination] = {cost, nextHop};
    }

    Q_EMIT updateRoutingTable(routingTable, UT::RoutingProtocol::OSPF);
}
