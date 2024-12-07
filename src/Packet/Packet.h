#ifndef PACKET_H
#define PACKET_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QString>
#include "globals.h"
#include "../Headers/DataLinkHeader.h"
#include "../Headers/TCPHeader.h"
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
    UT::PacketType  m_packetType;
    QByteArray m_payload;
    uint32_t m_sequenceNumber;
    uint32_t m_waitingCycles;
    uint32_t m_totalCycles;
    QList<QString> m_path;

    std::optional<UT::PacketControlType> m_controlType;

    DataLinkHeader m_dataLinkHeader;
    // IPHeader m_ipHeader;
    // std::optional<BGPHeader> m_bgpHeader;
    std::optional<TCPHeader> m_tcpHeader;



};

#endif // PACKET_H
