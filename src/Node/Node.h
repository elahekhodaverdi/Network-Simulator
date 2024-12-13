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
    explicit Node(int id, QObject *parent = nullptr);
    int getId();
    virtual void setIP(IPv4Ptr_t ip) = 0;

protected:
    int m_id;
    MACAddress m_MACAddress;
    IPv4Ptr_t m_IP;
};

#endif    // NODE_H
