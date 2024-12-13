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
    uint8_t setPortNumber(uint8_t number);
    QString setRouterIP(QString routerIP);
Q_SIGNALS:
    void packetSent(const PacketPtr_t &data);
    void packetReceived(const PacketPtr_t &data);

public Q_SLOTS:
    void sendPacket(const PacketPtr_t &data);
    void receivePacket(const PacketPtr_t &data);

private:
    uint8_t  m_number;
    uint64_t m_numberOfPacketsSent;
    QString  m_routerIP;
};

typedef QSharedPointer<Port> PortPtr_t;

#endif    // PORT_H
