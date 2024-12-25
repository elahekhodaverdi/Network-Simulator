#include "Node.h"
#include "../MACAddress/MACAddressGenerator.h"

Node::Node(int id, MACAddress macAddress, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_MACAddress(macAddress)
    , m_IP(IPv4Ptr_t::create())
{}

Node::Node(int id, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_IP(IPv4Ptr_t::create())
{
    m_MACAddress = MACAddressGenerator::getRandomMAC();
}

Node::~Node()
{
    if (!m_IP.isNull())
        m_IP.clear();
}

void Node::checkCurrentThread() {
    QThread* currentThread = QThread::currentThread();

    if (currentThread == this) {
        qDebug() << "This code is running in the object's thread:" << currentThread << "object ID: " << m_id;
    } else {
        qDebug() << "This code is running in a different thread.";
        qDebug() << "Current thread:" << currentThread << ", Object's thread:" << this;
    }
}

void Node::sendDiscoveryDHCP(){
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader(), this);
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::DHCPDiscovery);
    packet->setPayload(QByteArray::number(m_id));
    broadcastPacket(packet, nullptr);
}

int Node::getId(){
    return m_id;
}
IPv4Ptr_t Node::getIP()
{
    return m_IP;
}

MACAddress Node::getMACAddress(){
    return m_MACAddress;
}
