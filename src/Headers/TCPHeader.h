#ifndef TCPHEADER_H
#define TCPHEADER_H

#include <QObject>

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
    void setOptionalData(const std::optional<std::vector<uint8_t>> &optionalData);

Q_SIGNALS:

private:
    uint16_t m_sourcePort;
    uint16_t m_destPort;
    uint32_t m_sequenceNumber;
    uint32_t m_acknowledgementNumber;
    uint8_t  m_flags;
    uint8_t  m_dataOffset;
    uint16_t m_checksum;
    uint16_t m_windowSize;
    uint16_t m_urgentPointer;
    std::optional<std::vector<uint8_t>> m_optionalData;
};

#endif // TCPHEADER_H
