#include "Node.h"
#include "../MACAddress/MACAddressGenerator.h"

Node::Node(int id, MACAddress macAddress, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_MACAddress(macAddress)
{}


Node::Node(int id, QObject *parent)
    : QThread(parent)
    , m_id(id)
{
    m_MACAddress = MACAddressGenerator::getRandomMAC();
}

int Node::getId(){
    return m_id;
}
IPv4Ptr_t Node::getIP()
{
    return m_IP;
}
