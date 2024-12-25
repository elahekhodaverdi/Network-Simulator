#include "DHCPServer.h"
#include <QJsonObject>
#include <QJsonDocument>

DHCPServer::DHCPServer(QString ipRange, QObject *parent) :
    QObject {parent}, m_ipRange(ipRange)
{

}

DHCPServer::~DHCPServer()
{
}

QString DHCPServer::getIP(int id){
    return m_ipRange.arg(id);
}

void DHCPServer::handleDiscoveryPacket(PacketPtr_t packet)
{
    int id = packet->payload().toInt();
    if (sentOffers.contains(id))
        return;
    sentOffers.append(id);
    QString ip = getIP(id);
    QJsonObject jsonObject;
    jsonObject["id"] = id;
    jsonObject["ip"] = ip;
    QJsonDocument jsonDoc(jsonObject);
    QByteArray payload = jsonDoc.toJson(QJsonDocument::Compact);

    PacketPtr_t offerPacket = PacketPtr_t::create(DataLinkHeader(), this);
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    offerPacket->setIPHeader(ipHeader);
    offerPacket->setPacketType(UT::PacketType::Control);
    offerPacket->setControlType(UT::PacketControlType::DHCPOffer);
    offerPacket->setPayload(payload);
    Q_EMIT newPacket(offerPacket, nullptr);
}

void DHCPServer::handleRequestPacket(PacketPtr_t packet){
    PacketPtr_t ackPacket = PacketPtr_t::create(DataLinkHeader(), this);
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setDestIp(packet->ipHeader()->sourceIp());
    ackPacket->setIPHeader(ipHeader);
    ackPacket->setPacketType(UT::PacketType::Control);
    ackPacket->setControlType(UT::PacketControlType::DHCPAcknowledge);
    Q_EMIT newPacket(ackPacket, nullptr);
}


