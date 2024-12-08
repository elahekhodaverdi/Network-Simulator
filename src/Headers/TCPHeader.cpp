#include "TCPHeader.h"
#include <QByteArray>
#include <algorithm>
#include <cstdint>

TCPHeader::TCPHeader(QObject *parent)
    : QObject{parent}, m_checksum(0)
{}

void TCPHeader::setSourcePort(uint16_t sourcePort)
{
    m_sourcePort = sourcePort;
}

void TCPHeader::setDestPort(uint16_t destPort)
{
    m_destPort = destPort;
}

void TCPHeader::setSequenceNumber(uint32_t sequenceNumber)
{
    m_sequenceNumber = sequenceNumber;
}

void TCPHeader::setAcknowledgementNumber(uint32_t acknowledgementNumber)
{
    m_acknowledgementNumber = acknowledgementNumber;
}

void TCPHeader::setFlags(uint8_t flags)
{
    m_flags = flags;
}

void TCPHeader::setDataOffset(uint8_t dataOffset)
{
    m_dataOffset = dataOffset;
}

void TCPHeader::setWindowSize(uint16_t windowSize)
{
    m_windowSize = windowSize;
}
void TCPHeader::setUrgentPointer(uint16_t urgentPointer)
{
    m_urgentPointer = urgentPointer;
}

uint16_t TCPHeader::sourcePort() const
{
    return m_sourcePort;
}

uint16_t TCPHeader::destPort() const
{
    return m_destPort;
}

uint32_t TCPHeader::sequenceNumber() const
{
    return m_sequenceNumber;
}

uint32_t TCPHeader::acknowledgementNumber() const
{
    return m_acknowledgementNumber;
}

uint8_t TCPHeader::flags() const
{
    return m_flags;
}

uint8_t TCPHeader::dataOffset() const
{
    return m_dataOffset;
}

uint16_t TCPHeader::checksum() const
{
    return m_checksum;
}

uint16_t TCPHeader::windowSize() const
{
    return m_windowSize;
}

uint16_t TCPHeader::urgentPointer() const
{
    return m_urgentPointer;
}

uint16_t onesComplementSum(const QByteArray &data)
{
    uint32_t sum = 0;
    for (int i = 0; i < data.size(); i += 2) {
        uint16_t word = 0;
        word = (static_cast<uint8_t>(data[i]) << 8);
        if (i + 1 < data.size()) {
            word |= static_cast<uint8_t>(data[i + 1]);
        }
        sum += word;
        while (sum >> 16) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
    }
    return ~sum & 0xFFFF;
}

uint16_t TCPHeader::calculateChecksum(const QByteArray &data,
                                      const QByteArray &sourceIP,
                                      const QByteArray &destIP)
{
    QByteArray pseudoHeader;
    pseudoHeader.append(sourceIP);
    pseudoHeader.append(destIP);
    pseudoHeader.append(static_cast<uint8_t>(0));
    pseudoHeader.append(static_cast<uint8_t>(6));
    pseudoHeader.append(reinterpret_cast<const char *>(&m_dataOffset), sizeof(m_dataOffset));

    QByteArray combinedData = pseudoHeader;
    combinedData.append(reinterpret_cast<const char *>(&m_sourcePort), sizeof(m_sourcePort));
    combinedData.append(reinterpret_cast<const char *>(&m_destPort), sizeof(m_destPort));
    combinedData.append(reinterpret_cast<const char *>(&m_sequenceNumber), sizeof(m_sequenceNumber));
    combinedData.append(reinterpret_cast<const char *>(&m_acknowledgementNumber),
                        sizeof(m_acknowledgementNumber));
    combinedData.append(reinterpret_cast<const char *>(&m_flags), sizeof(m_flags));
    combinedData.append(reinterpret_cast<const char *>(&m_dataOffset), sizeof(m_dataOffset));
    combinedData.append(reinterpret_cast<const char *>(&m_windowSize), sizeof(m_windowSize));
    combinedData.append(reinterpret_cast<const char *>(&m_checksum), sizeof(m_checksum));
    combinedData.append(reinterpret_cast<const char *>(&m_urgentPointer), sizeof(m_urgentPointer));

    combinedData.append(data);

    return onesComplementSum(combinedData);
}
