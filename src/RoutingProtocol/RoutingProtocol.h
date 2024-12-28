#ifndef ROUTINGPROTOCOL_H
#define ROUTINGPROTOCOL_H

#include <QObject>
#include "../Globals/Globals.h"
#include "../IP/IP.h"
#include "../Port/Port.h"
#include "../Packet/Packet.h"

class RoutingProtocol : public QObject
{
    Q_OBJECT
public:
    explicit RoutingProtocol(QObject *parent = nullptr);
    void printRoutingTable() const;
    void handleNewTick(UT::Phase phase);
    virtual void setRouterIP(IPv4Ptr_t routerIP);
    virtual void startRouting() = 0;
    PortPtr_t findOutPort(IPv4Ptr_t destIP);
    virtual void addNewNeighbour(const IPv4Ptr_t &neighbourIP, PortPtr_t inPort) = 0;
    virtual void processRoutingPacket(const PacketPtr_t &packet, PortPtr_t inPort) = 0;

Q_SIGNALS:
    void newRoutingPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort);
    void noUpdateAtRoutingTable();

protected:
    struct RoutingTableEntry
    {
        IPv4Ptr_t destination;
        IPv4Ptr_t subnetMask;
        IPv4Ptr_t nextHop;
        PortPtr_t outPort;
        int metric;
        UT::RoutingProtocol protocol;
    };

    bool m_routingStarted{false};
    void updateRoutingTable(RoutingTableEntry newEntry);
    QList<RoutingTableEntry> routingTable;
    IPv4Ptr_t m_routerIP;
    bool m_routingTableUpdatedFromLastTick{false};
    bool routingIsDone{false};
    int m_ticksFromLastUpdate{0};
};

#endif // ROUTINGPROTOCOL_H
