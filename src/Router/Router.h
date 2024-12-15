#ifndef ROUTER_H
#define ROUTER_H

#include "../Globals/Globals.h"
#include "../Node/Node.h"
#include "../Packet/Packet.h"
#include "../Port/Port.h"
#include "../PC/PC.h"

class Router : public Node
{
    Q_OBJECT

public:
    explicit Router(int id,
                    MACAddress macAddress,
                    int portCount = 6,
                    int bufferSize = 6,
                    QObject *parent = nullptr);
    ~Router();
    void setRouterAsDHCPServer();
    void setRouterBroken();
    bool routerIsBroken() const;

    PortPtr_t getAnUnboundPort() const;
    int remainingPorts() const;
    QList<PortPtr_t> getPorts() { return ports; }
    void setIP(IPv4Ptr_t ip) override;
    void printRoutingTable() const;
    bool isDHCPServer() const;
    void addRoutingTableEntry(IPv4Ptr_t destination,
                              int subnetMask,
                              IPv4Ptr_t nextHop,
                              PortPtr_t outPort,
                              int metric,
                              UT::RoutingProtocol protocol);

public Q_SLOTS:
    void sendPacket(QVector<QSharedPointer<PC>> selectedPCs);
    void receivePacket(const PacketPtr_t &data, uint8_t port_number) override;

private:
    struct RoutingTableEntry
    {
        IPv4Ptr_t destination;
        int subnetMask;
        IPv4Ptr_t nextHop;
        PortPtr_t outPort;
        int metric;
        UT::RoutingProtocol protocol;
    };

    QList<PortPtr_t> ports;
    UT::IPVersion ipVersion = UT::IPVersion::IPv4;
    QList<PacketPtr_t> buffer;
    bool DHCPServer = false;
    bool broken = false;
    QList<RoutingTableEntry> routingTable;
    PortPtr_t findSendPort(IPv4Ptr_t destIP);
    QMap<PortPtr_t, PacketPtr_t> findPacketsToSend();

    int maxPorts;
    int maxBufferSize;
};

typedef QSharedPointer<Router> RouterPtr_t;

#endif // ROUTER_H
