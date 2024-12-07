#ifndef TCPHEADER_H
#define TCPHEADER_H

#include <QObject>
#include <cstdint>

class TCPHeader : public QObject
{
    Q_OBJECT
public:
    explicit TCPHeader(QObject *parent = nullptr);

    void setSourcePort(uint16_t sourcePort);
    void setDestPort(uint16_t destPort);
    void setSequenceNumber(uint32_t sequenceNumber);
    void setAcknowledgementNumber(uint32_t acknowledgementNumber);
    void setFlags(uint8_t flags);
    void setDataOffset(uint8_t dataOffset);
    void setWindowSize(uint16_t windowSize);
    void setUrgentPointer(uint16_t urgentPointer);

    uint16_t sourcePort() const;
    uint16_t destPort() const;
    uint32_t sequenceNumber() const;
    uint32_t acknowledgementNumber() const;
    uint8_t flags() const;
    uint8_t dataOffset() const;
    uint16_t checksum() const;
    uint16_t windowSize() const;
    uint16_t urgentPointer() const;

Q_SIGNALS:

private:
    uint16_t m_sourcePort;
    uint16_t m_destPort;
    uint32_t m_sequenceNumber;
    uint32_t m_acknowledgementNumber;
    uint8_t m_flags;
    uint8_t m_dataOffset;
    uint16_t m_checksum;
    uint16_t m_windowSize;
    uint16_t m_urgentPointer;
};

#endif // TCPHEADER_H
