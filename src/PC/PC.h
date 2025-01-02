#ifndef PC_H
#define PC_H

#include <QSharedPointer>
#include <QVector>
#include "../Node/Node.h"
#include "../Packet/Packet.h"
#include "../Port/Port.h"

class PC : public Node
{
    Q_OBJECT

public:
    explicit PC(int id, MACAddress macAddress, QObject *parent = nullptr);
    explicit PC(int id, QObject *parent = nullptr);
    ~PC();
    void setIP(IPv4Ptr_t ip) override;
    PortPtr_t gateway();
public Q_SLOTS:
    void handleNewTick(const UT::Phase phase) override;
    void setShouldSendPacket(QVector<QSharedPointer<PC>> selectedPCs);
    void receivePacket(const PacketPtr_t &data, uint8_t port_number) override;
Q_SIGNALS:
    void packetsSent(int num);

private:
    bool shouldSendNewPacket{false};
    PortPtr_t m_gateway;

    PacketPtr_t createNewPacket();
    QSharedPointer<PC> chooseRandomPC();
    void setupGateway();
    void sendNewPacket();
    void handlePhaseChange(const UT::Phase nextPhase);
    void handleControlPacket(const PacketPtr_t &data);
    void addPacketForBroadcast(const PacketPtr_t &packet, PortPtr_t triggeringPort) override;
    void broadcastPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort);
};

typedef QSharedPointer<PC> PCPtr_t;

#endif // PC_H
