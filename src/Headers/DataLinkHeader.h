#ifndef DATALINKHEADER_H
#define DATALINKHEADER_H

#include <QObject>
#include "/MACAddress.h"
class DataLinkHeader : public QObject
{
    Q_OBJECT
public:
    explicit DataLinkHeader(QObject *parent = nullptr);

Q_SIGNALS:

private:
    MACAddress m_sourceMACAddress;
};

#endif // DATALINKHEADER_H
