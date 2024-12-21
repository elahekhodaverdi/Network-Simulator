#ifndef ROUTER_H
#define ROUTER_H

#include "../Globals/Globals.h"
#include "../IP/IP.h"
#include "../Node/Node.h"
#include "../PC/PC.h"
#include "../Packet/Packet.h"
#include "../Port/Port.h"
#include "../RoutingProtocol/OSPF.h"

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

    void markAsBroken();
    bool isBroken() const;
    bool isDHCPServer() const;
    int remainingPorts() const;
    void printRoutingTable() const;
    PortPtr_t getAnUnboundPort() const;
    QList<PortPtr_t> getPorts() const;
    void setIP(IPv4Ptr_t ip) override;
    void setRouterAsDHCPServer();

public Q_SLOTS:
    void sendPacket(QVector<QSharedPointer<PC>> selectedPCs);
    void receivePacket(const PacketPtr_t &data, uint8_t portNumber) override;
    void sendRoutingPacket(const QByteArray &data);
    void updateRoutingTableFromProtocol(QMap<IPv4Ptr_t, std::pair<int, IPv4Ptr_t>> routingTable,
                                        UT::RoutingProtocol protocol);

private:
    struct RoutingTableEntry
    {
        IPv4Ptr_t destination;
        IPv4Ptr_t subnetMask;
        IPv4Ptr_t nextHop;
        PortPtr_t outPort;
        int metric;
        UT::RoutingProtocol protocol;
    };

    int maxPorts;
    int maxBufferSize;
    bool broken = false;
    bool DHCPServer = false;
    QList<PortPtr_t> ports;
    QList<PacketPtr_t> buffer;
    QMap<IPv4Ptr_t, int> distanceVector;
    QList<RoutingTableEntry> routingTable;
    RoutingProtocol *routingProtocol;
    UT::IPVersion ipVersion = UT::IPVersion::IPv4;

    PortPtr_t findSendPort(IPv4Ptr_t destIP);
    QMap<PortPtr_t, PacketPtr_t> findPacketsToSend();
    void connectPortsToSignals();
    void initializeRoutingProtocol();
    void updateRoutingTable(RoutingTableEntry newEntry);
    void addNewNeighbor(const IPv4Ptr_t &neighborIP, uint8_t portNumber);
    void handleControlPacket(const PacketPtr_t &data, uint8_t portNumber);
    void sendResponsePacket(const PacketPtr_t &requestPacket, uint8_t portNumber);
    void updateDistanceVector(IPv4Ptr_t destIP, int metric, IPv4Ptr_t neighborIP, uint8_t portNumber);
};

typedef QSharedPointer<Router> RouterPtr_t;

#endif // ROUTER_H
