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
    explicit RoutingProtocol(QString routerIP, QObject *parent = nullptr);
    void printRoutingTable() const;
    PortPtr_t findOutPort(IPv4Ptr_t destIP);
    //virtual ~RoutingProtocol() = default;
    virtual void initialize() = 0;
    virtual void processRoutingPacket(const PacketPtr_t &packet, PortPtr_t outPort) = 0;
    virtual void addNewNeighbor(const IPv4Ptr_t &neighborIP, PortPtr_t outPort) = 0;

Q_SIGNALS:
    void NewOutgoingRoutingPacket(PacketPtr_t &packet, PortPtr_t triggeringPort);

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

    void updateRoutingTable(RoutingTableEntry newEntry);
    QList<RoutingTableEntry> routingTable;
    IPv4Ptr_t m_routerIP;
    QList<IPv4Ptr_t> neighbors;
};

#endif // ROUTINGPROTOCOL_H
