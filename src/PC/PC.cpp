#include "PC.h"

PC::PC(int id, MACAddress macAddress, QObject *parent)
    : Node(id, macAddress, parent)
{
}

PC::PC(int id, QObject *parent)
    : Node(id, parent)
{
}

void PC::sendPacket(QVector<QSharedPointer<PC>> selectedPCs) {
    if (!selectedPCs.contains(this))
        return;
    qDebug() << "sending packet from " << m_id;
}

void PC::receivePacket(const PacketPtr_t &data, uint8_t port_number)
{
    qDebug() << "Packet received in PC: " << m_id << " Content: " << data->payload();
}

void PC::setIP(IPv4Ptr_t ip)
{
    m_IP = ip;
    m_gateway->setRouterIP(ip->toString());
    connect(m_gateway.get(), &Port::packetReceived, this, &PC::receivePacket);
    connect(this, &PC::newPacket, m_gateway.get(), &Port::sendPacket);
}

PortPtr_t PC::gateway(){
    return m_gateway;
}
