#ifndef NODE_H
#define NODE_H

#include <QDebug>
#include <QThread>
#include "../IP/IP.h"
#include "../MACAddress/MACAddress.h"

class Node : public QThread
{
    Q_OBJECT

public:
    explicit Node(int id, MACAddress macAddress, QObject *parent = nullptr);
    void setIPAddress(QString ip);

protected:
    int m_id;
    MACAddress m_MACAddress;
    IPv4_t m_IP;
};

#endif    // NODE_H
