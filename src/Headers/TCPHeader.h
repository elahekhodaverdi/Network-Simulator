#ifndef TCPHEADER_H
#define TCPHEADER_H

#include <QObject>

class TCPHeader : public QObject
{
    Q_OBJECT
public:
    explicit TCPHeader(QObject *parent = nullptr);

Q_SIGNALS:

private:
    qint16 sourcePort;
    qint16 destination_t;
    qint32 sequenceNumber;
    qint32 acknowledgementNumber;
    // qint8 flags;
    qint16 checksum;
    qint16 windowSize;
    qint16 urgentPointer;
    std::optional<QByteArray> optionalData;
};

#endif // TCPHEADER_H
