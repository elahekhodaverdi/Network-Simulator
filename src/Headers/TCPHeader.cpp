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
