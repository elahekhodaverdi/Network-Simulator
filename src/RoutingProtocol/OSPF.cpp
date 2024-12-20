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
        if (routerIP == m_routerIP)
            shortestPathTree[routerIP] = 0;

        else
            shortestPathTree[routerIP] = std::numeric_limits<int>::max();
    }
}

void OSPF::addNeighbor(IPv4Ptr_t neighborIP)
{
    if (!neighbors.contains(neighborIP)) {
        neighbors.append(neighborIP);
        shortestPathTree[neighborIP] = 1;
    }
}

void OSPF::advertiseLinkState()
{
    LSA lsa;
    lsa.neighborIPs = neighbors;
    lsa.routerIP = m_routerIP;
    sendRoutingInformation(lsa.toByteArray());
}

void OSPF::sendRoutingInformation(const QByteArray &lsa)
{
    Q_EMIT sendPacketToNeighbors(lsa);
}

void OSPF::processReceivedRoutingInformation(const QByteArray &lsaData)
{
    LSA lsa = LSA::fromByteArray(lsaData);

    if (!LSDB.contains(lsa.routerIP))
        sendRoutingInformation(lsa.toByteArray());
    LSDB[lsa.routerIP] = lsa;
    calculateSPF();
}

void OSPF::calculateSPF()
{
    QMap<IPv4Ptr_t, int> distance;
    QMap<IPv4Ptr_t, IPv4Ptr_t> previousHop;

    for (const auto &routerIP : Network::getAllRoutersIPs()) {
        distance[routerIP] = std::numeric_limits<int>::max();
    }
    distance[m_routerIP] = 0;

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
