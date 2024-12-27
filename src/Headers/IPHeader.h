#ifndef IPHEADER_H
#define IPHEADER_H

#include <QObject>
#include <cstdint>
#include <QByteArray>
#include "../IP/IP.h"

class IPHeader : public QObject
{
    Q_OBJECT
public:
    explicit IPHeader(QObject *parent = nullptr);
    IPHeader(const IPHeader &other);

    void setVersionHeaderLength(uint8_t versionHeaderLength);
    void setTypeOfService(uint8_t typeOfService);
    void setTotalLength(uint16_t totalLength);
    void setIdentification(uint16_t identification);
    void setFlagsFragmentOffset(uint16_t flagsFragmentOffset);
    void setTTL(uint8_t ttl);
    void setProtocol(uint8_t protocol);
    void setHeaderChecksum(uint16_t headerChecksum);
    void setSourceIp(const IPv4Ptr_t &sourceIp);
    void setDestIp(const IPv4Ptr_t &destIp);
    void decTTL();

    uint8_t versionHeaderLength() const;
    uint8_t typeOfService() const;
    uint16_t totalLength() const;
    uint16_t identification() const;
    uint16_t flagsFragmentOffset() const;
    uint8_t ttl() const;
    uint8_t protocol() const;
    uint16_t headerChecksum() const;
    IPv4Ptr_t sourceIp() const;
    IPv4Ptr_t destIp() const;

    uint16_t calculateHeaderChecksum(const QByteArray &data);

Q_SIGNALS:

private:
    uint8_t m_versionHeaderLength;
    uint8_t m_typeOfService;
    uint16_t m_totalLength;
    uint16_t m_identification;
    uint16_t m_flagsFragmentOffset;
    uint8_t m_ttl;
    uint8_t m_protocol;
    uint16_t m_headerChecksum;
    IPv4Ptr_t m_sourceIp;
    IPv4Ptr_t m_destIp;
};

#endif    // IPHEADER_H
