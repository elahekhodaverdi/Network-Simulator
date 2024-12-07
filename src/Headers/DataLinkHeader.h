#ifndef DATALINKHEADER_H
#define DATALINKHEADER_H

#include "../MACAddress/MACAddress.h"
#include <QObject>

class DataLinkHeader : public QObject
{
    Q_OBJECT
public:
    explicit DataLinkHeader(QObject *parent = nullptr);

Q_SIGNALS:

private:
    MACAddress m_sourceMACAddress;
    MACAddress m_destinationMACAddress;
};

#endif // DATALINKHEADER_H
