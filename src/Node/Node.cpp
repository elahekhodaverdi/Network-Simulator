#include "Node.h"

Node::Node(int id, MACAddress macAddress, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_MACAddress(macAddress)
{}

void Node::setIPAddress(QString ip)
{
    m_IP = IP(IP);
}
