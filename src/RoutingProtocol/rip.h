#ifndef RIP_H
#define RIP_H

#include "RoutingProtocol.h"

class RIP : public RoutingProtocol
{
public:
    explicit RIP(QObject *parent = nullptr);
    void initialize() override;
    void addNewNeighbor(const IPv4Ptr_t &neighborIP, PortPtr_t outPort) override;
    void processRoutingPacket(const PacketPtr_t &packet, PortPtr_t outPort) override;


private:
    QMap<IPv4Ptr_t, int> distanceVector;

    void updateDistanceVector(IPv4Ptr_t destIP, int metric, IPv4Ptr_t neighborIP, PortPtr_t outPort);
    void sendRIPPacket(PortPtr_t triggeringPort);
    QByteArray convertDistanceVectorToJson();
    QMap<IPv4Ptr_t, int> convertToDistanceVector(const QByteArray &jsonData);
};

#endif    // RIP_H
