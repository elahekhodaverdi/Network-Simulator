#include "OSPF.h"
#include <QJsonDocument>
#include <QJsonObject>

OSPF::OSPF(QObject *parent)
    : RoutingProtocol{parent}
{}

void OSPF::startRouting()
{
    m_routingStarted = true;
    sendLSAPacket();
}

void OSPF::addNewNeighbour(const IPv4Ptr_t &neighbourIP, PortPtr_t outPort)
{
    neighbourPorts[neighbourIP] = outPort;

    if (lsDatabase.contains(m_routerIP)) {
        LinkStateAdvertisement newLSA{m_routerIP, {{neighbourIP, 1}}};
        lsDatabase[m_routerIP] = newLSA;
    } else {
        lsDatabase[m_routerIP].linkMetrics[neighbourIP] = 1;
    }
    RoutingTableEntry newEntry{neighbourIP,
                               IPv4Ptr_t::create("255.255.255.0"),
                               neighbourIP,
                               outPort,
                               1,
                               UT::RoutingProtocol::OSPF};
    updateRoutingTable(newEntry);
}

void OSPF::sendLSAPacket()
{
    LinkStateAdvertisement &lsa = lsDatabase[m_routerIP];

    QByteArray payload = lsa.toByteArray();

    for (auto it = neighbourPorts.cbegin(); it != neighbourPorts.cend(); ++it) {
        PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader());
        QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
        ipHeader->setSourceIp(m_routerIP);
        ipHeader->setDestIp(it.key());
        ipHeader->setTTL(10);
        packet->setIPHeader(ipHeader);
        packet->setPacketType(UT::PacketType::Control);
        packet->setControlType(UT::PacketControlType::OSPF);
        packet->setPayload(payload);

        Q_EMIT newRoutingPacket(packet, nullptr);
    }
}
void OSPF::processRoutingPacket(const PacketPtr_t &packet, PortPtr_t inPort)
{
    LinkStateAdvertisement lsa = LinkStateAdvertisement::fromByteArray(packet->payload());
    processLSA(lsa);
    Q_EMIT newRoutingPacket(packet, inPort);
}

void OSPF::processLSA(const LinkStateAdvertisement &lsa)
{
    if (!lsDatabase.contains(lsa.originRouter)
        || lsDatabase[lsa.originRouter].linkMetrics != lsa.linkMetrics) {
        lsDatabase[lsa.originRouter] = lsa;
        recomputeRoutingTable();
    }
}
void OSPF::recomputeRoutingTable()
{
    QSet<IPv4Ptr_t> visited;
    QMap<IPv4Ptr_t, int> shortestPaths;
    QMap<IPv4Ptr_t, IPv4Ptr_t> nextHops;

    QQueue<IPv4Ptr_t> toVisit;

    shortestPaths[m_routerIP] = 0;
    toVisit.enqueue(m_routerIP);

    while (!toVisit.isEmpty()) {
        IPv4Ptr_t currentRouter = toVisit.dequeue();
        int currentMetric = shortestPaths[currentRouter];

        const LinkStateAdvertisement &lsa = lsDatabase[currentRouter];
        for (auto it = lsa.linkMetrics.cbegin(); it != lsa.linkMetrics.cend(); ++it) {
            IPv4Ptr_t neighbour = it.key();
            int linkMetric = it.value();

            int newMetric = currentMetric + linkMetric;

            if (!shortestPaths.contains(neighbour) || newMetric < shortestPaths[neighbour]) {
                shortestPaths[neighbour] = newMetric;
                nextHops[neighbour] = (nextHops.contains(currentRouter) ? nextHops[currentRouter]
                                                                        : neighbour);
                toVisit.enqueue(neighbour);
            }
        }
    }

    for (auto it = shortestPaths.cbegin(); it != shortestPaths.cend(); ++it) {
        IPv4Ptr_t destIP = it.key();
        int newMetric = it.value();

        if (destIP == m_routerIP)
            continue;

        PortPtr_t outPort = neighbourPorts.value(nextHops[destIP], nullptr);

        auto entryIt = std::find_if(routingTable.begin(),
                                    routingTable.end(),
                                    [&destIP](const RoutingTableEntry &entry) {
                                        return *entry.destination == *destIP;
                                    });

        if (entryIt == routingTable.end()) {
            RoutingTableEntry newEntry{destIP,
                                       IPv4Ptr_t::create("255.255.255.0"),
                                       nextHops[destIP],
                                       outPort,
                                       newMetric,
                                       UT::RoutingProtocol::OSPF};
            updateRoutingTable(newEntry);
        } else if (entryIt->metric > newMetric) {
            entryIt->nextHop = nextHops[destIP];
            entryIt->outPort = outPort;
            entryIt->metric = newMetric;
        }
    }
}
