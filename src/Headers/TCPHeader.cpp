#include "TCPHeader.h"
#include <QByteArray>
#include <algorithm>
#include <cstdint>

TCPHeader::TCPHeader(QObject *parent)
    : QObject{parent}, m_checksum(0)
{}

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

uint16_t TCPHeader::calculateChecksum(const QByteArray &data, const QByteArray &sourceIP, const QByteArray &destIP)
{
    QByteArray pseudoHeader;
    pseudoHeader.append(sourceIP);  // Source IP address (4 bytes)
    pseudoHeader.append(destIP);    // Destination IP address (4 bytes)
    pseudoHeader.append(static_cast<uint8_t>(0)); // Padding byte (0)
    pseudoHeader.append(static_cast<uint8_t>(6)); // Protocol (TCP = 6)
    pseudoHeader.append(reinterpret_cast<const char*>(&m_dataOffset), sizeof(m_dataOffset)); // Length of the TCP header

    QByteArray combinedData = pseudoHeader;
    combinedData.append(reinterpret_cast<const char*>(&m_sourcePort), sizeof(m_sourcePort));
    combinedData.append(reinterpret_cast<const char*>(&m_destPort), sizeof(m_destPort));
    combinedData.append(reinterpret_cast<const char*>(&m_sequenceNumber), sizeof(m_sequenceNumber));
    combinedData.append(reinterpret_cast<const char*>(&m_acknowledgementNumber), sizeof(m_acknowledgementNumber));
    combinedData.append(reinterpret_cast<const char*>(&m_flags), sizeof(m_flags));
    combinedData.append(reinterpret_cast<const char*>(&m_dataOffset), sizeof(m_dataOffset));
    combinedData.append(reinterpret_cast<const char*>(&m_windowSize), sizeof(m_windowSize));
    combinedData.append(reinterpret_cast<const char*>(&m_checksum), sizeof(m_checksum));
    combinedData.append(reinterpret_cast<const char*>(&m_urgentPointer), sizeof(m_urgentPointer));

    if (m_optionalData.has_value()) {
        combinedData.append(m_optionalData.value());
    }

    combinedData.append(data);

    return onesComplementSum(combinedData);
}
