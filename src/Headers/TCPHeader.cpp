#include "TCPHeader.h"

TCPHeader::TCPHeader(QObject *parent)
    : QObject{parent}
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
void TCPHeader::setOptionalData(const std::optional<std::vector<uint8_t>> &optionalData)
{
    m_optionalData = optionalData;
}
