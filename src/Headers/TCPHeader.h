#ifndef TCPHEADER_H
#define TCPHEADER_H

#include <QObject>
#include <optional>
#include <QByteArray>
#include <cstdint>

class TCPHeader : public QObject
{
    Q_OBJECT
public:
    explicit TCPHeader(QObject *parent = nullptr);

    uint16_t calculateChecksum(const QByteArray &data, const QByteArray &sourceIP, const QByteArray &destIP);

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
    std::optional<QByteArray> m_optionalData;
};

#endif // TCPHEADER_H
