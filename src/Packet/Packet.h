#ifndef PACKET_H
#define PACKET_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QString>
#include "globals.h"
// #include "IPHeader.h"
// #include "TCPHeader.h"
// #include "BGPHeader.h"

class Packet : public QObject
{
    Q_OBJECT
public:
    explicit Packet(QObject* parent = nullptr);
    ~Packet();

private:
    UT::PacketType packetType;
    QByteArray payload;
    uint32_t sequenceNumber;
    uint32_t waitingCycles;
    uint32_t totalCycles;
    QList<QString> path;

    std::optional<UT::PacketControlType> controlType;

    // IPHeader ipHeader;
    // std::optional<BGPHeader> bgpHeader;
    // std::optional<TCPHeader> tcpHeader;


};

#endif // PACKET_H
