#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include "../Headers/DataLinkHeader.h"
#include "../Headers/TCPHeader.h"
#include "../Headers/IPHeader.h"
#include "globals.h"

class Packet : public QObject
{
    Q_OBJECT
public:
    explicit Packet(DataLinkHeader dataLinkHeader, QObject* parent = nullptr);
    void addToPath(QString ip);
    void incWaitingCycles();
    void incTotalCycles();

    void setPacketType(UT::PacketType packetType);
    void setPayload(QByteArray payload);
    void setSequenceNumber(uint32_t sequenceNumber);
    void setWaitingCycles(uint32_t waitingCycles);
    void setTotalCycles(uint32_t totalCycles);
    void setIPHeader(QSharedPointer<IPHeader> ipHeader);

    UT::PacketType packetType();
    QByteArray payload();
    uint32_t sequenceNumber();
    uint32_t waitingCycles();
    uint32_t totalCycles();
    QList<QString> path();
    QSharedPointer<IPHeader> ipHeader();

    ~Packet();

private:
    UT::PacketType  m_packetType;
    QByteArray m_payload;
    uint32_t m_sequenceNumber;
    uint32_t m_waitingCycles;
    uint32_t m_totalCycles;
    QList<QString> m_path;
    DataLinkHeader m_dataLinkHeader;

    QSharedPointer<IPHeader> m_ipHeader;
    std::optional<TCPHeader> m_tcpHeader;

    std::optional<UT::PacketControlType> m_controlType;


};

typedef QSharedPointer<Packet> PacketPtr_t;

#endif // PACKET_H
