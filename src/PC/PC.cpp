#include "PC.h"

PC::PC(int id, MACAddress macAddress, QObject *parent) :
    Node(id, macAddress, parent), m_gateway(PortPtr_t::create(this))
{
}

PC::PC(int id, QObject *parent)
    : Node(id, parent)
{
}

void PC::sendPacket(QVector<QSharedPointer<PC>> selectedPCs)
{
    if (!selectedPCs.contains(this))
        return;
    qDebug() << "Sending packet from PC:" << m_id;
    PacketPtr_t packet = createNewPacket();
    Q_EMIT newPacket(packet, m_gateway->getPortNumber());
}

PacketPtr_t PC::createNewPacket(){
    //TODO
    return nullptr;
}

void PC::receivePacket(const PacketPtr_t &data, uint8_t port_number)
{
    qDebug() << "here id";
    qDebug() << "Packet received in PC: " << m_id << " Content: " << data->payload();
}

void PC::setIP(IPv4Ptr_t ip)
{
    m_IP = ip;
    if (m_gateway)
        m_gateway->setRouterIP(ip->toString());
}

PortPtr_t PC::gateway(){
    return m_gateway;
}

void PC::setGateway(PortPtr_t gateway)
{
    m_gateway = gateway;
    connect(m_gateway.get(), &Port::packetReceived, this, &PC::receivePacket);
    connect(this, &PC::newPacket, m_gateway.get(), &Port::sendPacket);
    m_gateway->setRouterIP(m_IP->toString());
}
