#ifndef NODE_H
#define NODE_H

#include <QDebug>
#include <QThread>
#include "../Packet/Packet.h"
#include "../Port/Port.h"
#include "../IP/IP.h"
#include "../MACAddress/MACAddress.h"

class Node : public QThread
{
    Q_OBJECT

public:
    explicit Node(int id, MACAddress macAddress, QObject *parent = nullptr);
    explicit Node(int id, QObject *parent = nullptr);
    ~Node();

    int getId();
    IPv4Ptr_t getIP();
    MACAddress getMACAddress();
    virtual void setIP(IPv4Ptr_t ip) = 0;

Q_SIGNALS:
    void newPacket(const PacketPtr_t &data, uint8_t port_number);
    void dhcpIsDone();
    void newPacketSent(int num);

public Q_SLOTS:
    virtual void handleNewTick(const UT::Phase phase) = 0;
    virtual void receivePacket(const PacketPtr_t &data, uint8_t port_number) = 0;

protected:
    int m_id;
    MACAddress m_MACAddress;
    IPv4Ptr_t m_IP;
    UT::Phase m_currentPhase;
    bool m_dhcpIsDone{false};

    void setDHCPDone();
    void checkCurrentThread();

    void sendDiscoveryDHCP();
    void sendRequestDHCP();
    void handleOfferDHCP(const PacketPtr_t &packet, PortPtr_t triggeringPort);
    void handleAckDHCP(const PacketPtr_t &packet, PortPtr_t triggeringPort);

    virtual void sendRequestPacket() {}
    void handleRequestPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort);
    void sendResponsePacket(const PacketPtr_t &requestPacket, uint8_t portNumber);
    virtual void handleResponsePacket(const PacketPtr_t &packet, PortPtr_t triggeringPort) {}

    virtual void addPacketForBroadcast(const PacketPtr_t &packet, PortPtr_t triggeringPort) = 0;
};

#endif    // NODE_H
