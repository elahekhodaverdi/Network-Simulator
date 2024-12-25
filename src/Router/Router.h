#ifndef ROUTER_H
#define ROUTER_H

#include "../Globals/Globals.h"
#include "../IP/IP.h"
#include "../Node/Node.h"
#include "../Packet/Packet.h"
#include "../Port/Port.h"
#include "../RoutingProtocol/RoutingProtocol.h"
#include "../DHCPServer/DHCPServer.h"
#include <QString>

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
    int numRemainingPorts() const;
    void printRoutingTable() const;
    PortPtr_t getAnUnboundPort() const;
    QList<PortPtr_t> getPorts() const;
    void setIP(IPv4Ptr_t ip) override;
    void setAsDHCPServer(QString ipRange);

public Q_SLOTS:
    void handleNewTick(const UT::Phase phase) override;
    void receivePacket(const PacketPtr_t &data, uint8_t portNumber) override;
    void sendRoutingPacket(PacketPtr_t &packet, PortPtr_t triggeringPort);
    void addNewPacketTobBuffer(PacketPtr_t packet, PortPtr_t triggeringPort);

Q_SIGNALS:
    void routingProtocolIsDone();

private:
    int maxPorts;
    int maxBufferSize;
    bool broken = false;
    QSharedPointer<DHCPServer> dhcpServer = nullptr;
    QList<PortPtr_t> ports;
    QList<QPair<PacketPtr_t, PortPtr_t>> buffer;
    RoutingProtocol *routingProtocol;
    UT::IPVersion ipVersion = UT::IPVersion::IPv4;
    QMap<PortPtr_t, PacketPtr_t> packetsToSend;

    int numBoundPorts() const;
    void connectPortsToSignals();
    void initializeRoutingProtocol();
    void updateBuffer();
    void sendPackets();
    void handleControlPacket(const PacketPtr_t &data, uint8_t portNumber);
    void sendResponsePacket(const PacketPtr_t &requestPacket, uint8_t portNumber);
    void handlePhaseChange(const UT::Phase nextPhase);
    void broadcastPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort) override;
};

typedef QSharedPointer<Router> RouterPtr_t;

#endif // ROUTER_H
