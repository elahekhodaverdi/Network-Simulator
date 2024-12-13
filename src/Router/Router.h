#ifndef ROUTER_H
#define ROUTER_H

#include "../Globals/Globals.h"
#include "../Node/Node.h"
#include "../Packet/Packet.h"
#include "../Port/Port.h"

class Router : public Node
{
    Q_OBJECT

public:
    explicit Router(int id, MACAddress macAddress, QObject *parent = nullptr);

    void setRouterAsDHCPServer();
    void setRouterBroken();
    bool routerIsBroken() const;

    PortPtr_t getAnUnboundPort() const;
    int remainingPorts() const;

    void setIP(IPv4Ptr_t ip) override;
    void printRoutingTable() const;

public Q_SLOTS:
    void receivePacket(const PacketPtr_t &data, uint8_t port_number) override;

private:
    struct RoutingTableEntry
    {
        IPv4_t destination;
        IPv4_t nextHop;
        PortPtr_t outPort;
    };

    QList<PortPtr_t> ports;
    UT::IPVersion ipVersion = UT::IPVersion::IPv4;
    QList<PacketPtr_t> buffer;
    bool DHCPServer = false;
    bool broken = false;
    QList<RoutingTableEntry> routingTable;

    const int maxPorts = 5;

};

typedef QSharedPointer<Router> RouterPtr_t;

#endif // ROUTER_H
