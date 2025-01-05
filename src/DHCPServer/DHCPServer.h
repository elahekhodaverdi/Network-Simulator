#ifndef DHCPSERVER_H
#define DHCPSERVER_H

#include <QObject>
#include <QString>
#include "../Packet/Packet.h"
#include "../Port/Port.h"

class DHCPServer : public QObject
{
    Q_OBJECT

public:
    explicit DHCPServer(int m_asID, QObject *parent = nullptr);
    ~DHCPServer() override;

    void handleDiscoveryPacket(PacketPtr_t packet);
    void handleRequestPacket(PacketPtr_t packet);
    QString getIP(int id);

Q_SIGNALS:
    void newPacket(PacketPtr_t packet, PortPtr_t triggeringPort);
private:
    QList<int> sentOffers;
    QList<QString> sentAcks;
    QString m_ipRange;
    int m_asID;
};

#endif      // DHCPSERVER_H
