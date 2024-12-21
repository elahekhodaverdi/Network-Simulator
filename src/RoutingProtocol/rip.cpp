#include "rip.h"
#include <QJsonObject>
#include <QJsonDocument>

RIP::RIP(QObject *parent) :
    RoutingProtocol {parent}
{}

void RIP::addNewNeighbor(const IPv4Ptr_t &neighborIP, PortPtr_t outPort)
{
    updateDistanceVector(neighborIP, 1, neighborIP, outPort);
}

void RIP::initialize(){

}

void RIP::updateDistanceVector(IPv4Ptr_t destIP, int metric, IPv4Ptr_t neighborIP, PortPtr_t outPort)
{
    int currentMetric = distanceVector.value(destIP, INF);
    if (currentMetric <= metric + 1)
        return;
    distanceVector[destIP] = metric + 1;
    RoutingTableEntry newEntry{
        destIP,
        IPv4Ptr_t::create("255.255.255.0"),
        neighborIP,
        outPort,
        metric + 1,
        UT::RoutingProtocol::RIP
    };
    updateRoutingTable(newEntry);
    sendRIPPacket(outPort);
}

void RIP::processRoutingPacket(const PacketPtr_t &packet, PortPtr_t outPort){
    QMap<IPv4Ptr_t, int> neighborDistanceVector = convertToDistanceVector(packet->payload());
    IPv4Ptr_t neighborIP = packet->ipHeader()->sourceIp();
    for (auto it = neighborDistanceVector.cbegin(); it != neighborDistanceVector.cend(); ++it)
        updateDistanceVector(it.key(), it.value(), neighborIP, outPort);
}

void RIP::sendRIPPacket(PortPtr_t triggeringPort){
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader(), this);
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setTTL(1);
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::RIP);
    packet->setPayload(convertDistanceVectorToJson());
    Q_EMIT NewOutgoingRoutingPacket(packet, triggeringPort);
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

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isObject())
    {
        qWarning() << "Invalid JSON: Expected a JSON object.";
        return neighborDistanceVector;
    }

    QJsonObject jsonObject = jsonDoc.object();

    for (auto it = jsonObject.constBegin(); it != jsonObject.constEnd(); ++it)
    {
        QString ipString = it.key();
        int distance = it.value().toInt();

        IPv4Ptr_t ipPtr = QSharedPointer<IPv4_t>::create(ipString);

        neighborDistanceVector.insert(ipPtr, distance);
    }

    return neighborDistanceVector;
}
