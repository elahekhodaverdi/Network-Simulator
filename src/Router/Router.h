#ifndef ROUTER_H
#define ROUTER_H

#include "../Globals/Globals.h"
#include "../IP/IP.h"
#include "../Node/Node.h"
#include "../PC/PC.h"
#include "../Packet/Packet.h"
#include "../Port/Port.h"
#include "../RoutingProtocol/RoutingProtocol.h"

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
    void sendRoutingPacket(PacketPtr_t &packet, PortPtr_t triggeringPort);

Q_SIGNALS:
    void routingProtocolIsDone();

private:
    int maxPorts;
    int maxBufferSize;
    bool broken = false;
    bool DHCPServer = false;
    QList<PortPtr_t> ports;
    QList<PacketPtr_t> buffer;
    RoutingProtocol *routingProtocol;
    UT::IPVersion ipVersion = UT::IPVersion::IPv4;

    QMap<PortPtr_t, PacketPtr_t> findPacketsToSend();
    void connectPortsToSignals();
    void initializeRoutingProtocol();
    void handleControlPacket(const PacketPtr_t &data, uint8_t portNumber);
    void sendResponsePacket(const PacketPtr_t &requestPacket, uint8_t portNumber);
};

typedef QSharedPointer<Router> RouterPtr_t;

#endif // ROUTER_H
