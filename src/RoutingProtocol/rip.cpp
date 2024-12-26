#include "rip.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "../Utils/utils.h"

RIP::RIP(QObject *parent) :
    RoutingProtocol {parent}
{}

void RIP::addNewNeighbor(const IPv4Ptr_t &neighborIP, PortPtr_t outPort)
{
    updateDistanceVector(neighborIP, 1, neighborIP, outPort);
}

void RIP::initialize(){

}

void RIP::startRouting(){
    m_routingStarted = true;
    sendRIPPacket();
}

void RIP::updateDistanceVector(IPv4Ptr_t destIP, int metric, IPv4Ptr_t neighborIP, PortPtr_t inPort)
{
    int currentMetric = distanceVector.value(destIP, INF);
    if (currentMetric <= metric + 1)
        return;
    distanceVector[destIP] = metric + 1;
    RoutingTableEntry newEntry{
        destIP,
        IPv4Ptr_t::create("255.255.255.0"),
        neighborIP,
        inPort,
        metric + 1,
        UT::RoutingProtocol::RIP
    };
    updateRoutingTable(newEntry);
    if (m_routingStarted)
        sendRIPPacket(inPort);
}

void RIP::processRoutingPacket(const PacketPtr_t &packet, PortPtr_t inPort){
    QMap<IPv4Ptr_t, int> neighborDistanceVector = convertToDistanceVector(packet->payload());
    IPv4Ptr_t neighborIP = packet->ipHeader()->sourceIp();
    for (auto it = neighborDistanceVector.cbegin(); it != neighborDistanceVector.cend(); ++it)
        updateDistanceVector(it.key(), it.value(), neighborIP, inPort);
}

void RIP::sendRIPPacket(PortPtr_t triggeringPort){
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader(), this);
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setSourceIp(m_routerIP);
    ipHeader->setTTL(1);
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::RIP);
    packet->setPayload(convertDistanceVectorToJson());
    Q_EMIT newRoutingPacket(packet, triggeringPort);
}

QByteArray RIP::convertDistanceVectorToJson()
{
    QJsonObject jsonObject;

    for (auto it = distanceVector.cbegin(); it != distanceVector.cend(); ++it)
    {
        const IPv4Ptr_t &ipPtr = it.key();
        if (ipPtr)
        {
            QString ipString = ipPtr->toString();
            jsonObject[ipString] = it.value();
        }
    }

    QJsonDocument jsonDoc(jsonObject);

    return jsonDoc.toJson(QJsonDocument::Compact);
}

QMap<IPv4Ptr_t, int> RIP::convertToDistanceVector(const QByteArray &jsonData)
{
    QMap<IPv4Ptr_t, int> neighborDistanceVector;

    QJsonObject jsonObject = Utils::convertPayloadToJson(jsonData);

    for (auto it = jsonObject.constBegin(); it != jsonObject.constEnd(); ++it)
    {
        QString ipString = it.key();
        int distance = it.value().toInt();

        IPv4Ptr_t ipPtr = QSharedPointer<IPv4_t>::create(ipString);

        neighborDistanceVector.insert(ipPtr, distance);
    }

    return neighborDistanceVector;
}
