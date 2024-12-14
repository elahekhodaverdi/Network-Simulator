#include "IPHeader.h"
#include "../Network/SimulationConfig.h"

IPHeader::IPHeader(QObject *parent)
    : QObject(parent),
    m_versionHeaderLength(0),
    m_typeOfService(0),
    m_totalLength(0),
    m_identification(0),
    m_flagsFragmentOffset(0),
    m_ttl(SimulationConfig::TTL),
    m_protocol(0),
    m_headerChecksum(0)
{
}

void IPHeader::setVersionHeaderLength(uint8_t versionHeaderLength) {
    m_versionHeaderLength = versionHeaderLength;
}

void IPHeader::setTypeOfService(uint8_t typeOfService) {
    m_typeOfService = typeOfService;
}

void IPHeader::setTotalLength(uint16_t totalLength) {
    m_totalLength = totalLength;
}

void IPHeader::setIdentification(uint16_t identification) {
    m_identification = identification;
}

void IPHeader::setFlagsFragmentOffset(uint16_t flagsFragmentOffset) {
    m_flagsFragmentOffset = flagsFragmentOffset;
}

void IPHeader::setTTL(uint8_t ttl) {
    m_ttl = ttl;
}

void IPHeader::setProtocol(uint8_t protocol) {
    m_protocol = protocol;
}

void IPHeader::setHeaderChecksum(uint16_t headerChecksum) {
    m_headerChecksum = headerChecksum;
}

void IPHeader::setSourceIp(const IPv4Ptr_t &sourceIp)
{
    m_sourceIp = sourceIp;
}

void IPHeader::decTTL(){
    m_ttl--;
}

void IPHeader::setDestIp(const IPv4Ptr_t &destIp)
{
    m_destIp = destIp;
}

uint8_t IPHeader::versionHeaderLength() const {
    return m_versionHeaderLength;
}

uint8_t IPHeader::typeOfService() const {
    return m_typeOfService;
}

uint16_t IPHeader::totalLength() const {
    return m_totalLength;
}

uint16_t IPHeader::identification() const {
    return m_identification;
}

uint16_t IPHeader::flagsFragmentOffset() const {
    return m_flagsFragmentOffset;
}

uint8_t IPHeader::ttl() const {
    return m_ttl;
}

uint8_t IPHeader::protocol() const {
    return m_protocol;
}

uint16_t IPHeader::headerChecksum() const {
    return m_headerChecksum;
}

IPv4Ptr_t IPHeader::sourceIp() const {
    return m_sourceIp;
}

IPv4Ptr_t IPHeader::destIp() const {
    return m_destIp;
}

uint16_t IPHeader::calculateHeaderChecksum(const QByteArray &data) {
    uint32_t sum = 0;
    const uint16_t *dataPtr = reinterpret_cast<const uint16_t *>(data.constData());
    for (int i = 0; i < data.size() / 2; ++i) {
        sum += *dataPtr++;
    }
    if (data.size() % 2) {
        sum += static_cast<uint8_t>(data[data.size() - 1]);
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return static_cast<uint16_t>(~sum);
}
