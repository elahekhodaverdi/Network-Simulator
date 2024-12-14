#include "Node.h"
#include "../MACAddress/MACAddressGenerator.h"

Node::Node(int id, MACAddress macAddress, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_MACAddress(macAddress)
    , m_IP(IPv4Ptr_t::create())
{}

Node::Node(int id, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_IP(IPv4Ptr_t::create())
{
    m_MACAddress = MACAddressGenerator::getRandomMAC();
}

Node::~Node()
{
    if (!m_IP.isNull())
        m_IP.clear();
}

int Node::getId(){
    return m_id;
}
IPv4Ptr_t Node::getIP()
{
    return m_IP;
}

MACAddress Node::getMACAddress(){
    return m_MACAddress;
}
