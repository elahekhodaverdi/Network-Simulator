#ifndef PORT_H
#define PORT_H

#include "../Packet/Packet.h"

#include <QObject>

class Port : public QObject
{
    Q_OBJECT

public:
    explicit Port(QObject *parent = nullptr);
    ~Port() override;
    uint8_t getPortNumber();
    QString getRouterIP();
    void setPortNumber(uint8_t number);
    void setRouterIP(QString routerIP);
Q_SIGNALS:
    void packetSent(const PacketPtr_t &data);
    void packetReceived(const PacketPtr_t &data, uint8_t port_number);

public Q_SLOTS:
    void sendPacket(const PacketPtr_t &data, uint8_t port_number);
    void receivePacket(const PacketPtr_t &data, uint8_t port_number);

private:
    uint8_t  m_number;
    uint64_t m_numberOfPacketsSent;
    QString  m_routerIP;
};

typedef QSharedPointer<Port> PortPtr_t;

#endif    // PORT_H
