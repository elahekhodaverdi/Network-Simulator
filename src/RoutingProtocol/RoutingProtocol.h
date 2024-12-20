#ifndef ROUTINGPROTOCOL_H
#define ROUTINGPROTOCOL_H

#include <QObject>
#include "../Globals/Globals.h"
#include "../IP/IP.h"
class RoutingProtocol : public QObject
{
    Q_OBJECT
public:
    explicit RoutingProtocol(QObject *parent = nullptr);
    explicit RoutingProtocol(QString routerIP, QObject *parent = nullptr);
    virtual ~RoutingProtocol() = default;
    virtual void initialize() = 0;
    virtual void processReceivedRoutingInformation(const QByteArray &lsaData) = 0;

    virtual void addNeighbor(IPv4Ptr_t neighborIP);
Q_SIGNALS:
    void updateRoutingTable(QMap<IPv4Ptr_t, std::pair<int, IPv4Ptr_t>> routingTable,
                            UT::RoutingProtocol protocol);
    void sendPacketToNeighbors(const QByteArray &data);

protected:
    virtual void sendRoutingInformation(const QByteArray &data) = 0;
    IPv4Ptr_t m_routerIP;
    QList<IPv4Ptr_t> neighbors;
};

#endif // ROUTINGPROTOCOL_H
