#include "PC.h"
#include "../Network/Network.h"
#include <QRandomGenerator>


PC::PC(int id, MACAddress macAddress, QObject *parent) :
    Node(id, macAddress, parent), m_gateway(PortPtr_t::create(this))
{
}

PC::PC(int id, QObject *parent)
    : Node(id, parent)
    , m_gateway(PortPtr_t::create(this))
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


PCPtr_t PC::chooseRandomPC()
{
    PCPtr_t randomPC;
    do {
        int randomIndex = QRandomGenerator::global()->bounded(Network::PCs.size());
        randomPC = Network::PCs.at(randomIndex);
    } while (randomPC.data() == this);

    return randomPC;
}

PacketPtr_t PC::createNewPacket()
{
    PCPtr_t destinationPC = chooseRandomPC();

    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader(this->getMACAddress(), destinationPC->getMACAddress()), this);

    if (m_IP && destinationPC->m_IP) {
        QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
        ipHeader->setSourceIp(m_IP);
        ipHeader->setDestIp(destinationPC->getIP());
        packet->setIPHeader(ipHeader);
    } else {
        qDebug() << "Source or destination IP not set.";
        return nullptr;
    }

    QByteArray payload = QByteArray("Hello from PC ") + QByteArray::number(m_id);
    packet->setPayload(payload);
    uint16_t checksum = packet->ipHeader()->calculateHeaderChecksum(payload);
    packet->ipHeader()->setHeaderChecksum(checksum);

    packet->setSequenceNumber(QRandomGenerator::global()->generate());
    packet->setPacketType(UT::PacketType::Data);

    qDebug() << "Packet created from PC" << m_id << "to PC" << destinationPC->getId();
    return packet;
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
