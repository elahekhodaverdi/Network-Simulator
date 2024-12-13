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
void Node::setIPAddress(QString ip)
{
    m_IP = IP<UT::IPVersion::IPv4>::IP(ip);
}

int Node::getId(){
    return m_id;
}
