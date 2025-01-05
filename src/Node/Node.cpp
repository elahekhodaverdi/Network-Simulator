#include "Node.h"
#include "../MACAddress/MACAddressGenerator.h"
#include "../Utils/utils.cpp"
#include "../Utils/utils.h"

Node::Node(int id, MACAddress macAddress, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_MACAddress(macAddress)
    , m_IP(nullptr)
{}

Node::Node(int id, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_IP(nullptr)
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
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader());
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    packet->setIPHeader(ipHeader);
    packet->dataLinkHeader().setSourceMACAddress(m_MACAddress);
    qDebug() << "It's me, Im the problem its me"
             << packet->dataLinkHeader().sourceMACAddress().toString() << m_MACAddress.toString();
    ipHeader->setTTL(5);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::DHCPDiscovery);
    packet->setPayload(QByteArray::number(m_id));
    addPacketForBroadcast(packet, nullptr);
}

void Node::sendRequestDHCP(){
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader());
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    packet->dataLinkHeader().setSourceMACAddress(m_MACAddress);
    ipHeader->setSourceIp(m_IP);
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::DHCPRequest);
    addPacketForBroadcast(packet, nullptr);
}

void Node::handleOfferDHCP(const PacketPtr_t &packet, PortPtr_t triggeringPort){
    QJsonObject payloadJson = Utils::convertPayloadToJson(packet->payload());
    int id = payloadJson["id"].toInt();
    if (id != m_id){
        if (packet->ipHeader()->ttl() > 0)
            addPacketForBroadcast(packet, triggeringPort);
        return;
    }
    if (m_IP != nullptr)
        return;
    QString ip = payloadJson["ip"].toString();
    setIP(IPv4Ptr_t::create(ip));
    sendRequestDHCP();
}

void Node::handleAckDHCP(const PacketPtr_t &packet, PortPtr_t triggeringPort){
    if (m_IP == nullptr || *packet->ipHeader()->destIp() != *m_IP){
        if (packet->ipHeader()->ttl() > 0)
            addPacketForBroadcast(packet, triggeringPort);
        return;
    }
    setDHCPDone();
}

void Node::setDHCPDone(){
    if (m_dhcpIsDone)
        return;
    m_dhcpIsDone = true;
    Q_EMIT dhcpIsDone();
}

void Node::handleRequestPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort)
{
    sendResponsePacket(packet, triggeringPort->getPortNumber());
}

void Node::sendResponsePacket(const PacketPtr_t &requestPacket, uint8_t portNumber)
{
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader());
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setTTL(5);
    ipHeader->setSourceIp(m_IP);
    ipHeader->setDestIp(requestPacket->ipHeader()->sourceIp());
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::Response);
    Q_EMIT newPacket(packet, portNumber);
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
