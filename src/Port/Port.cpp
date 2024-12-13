#include "Port.h"

Port::Port(QObject *parent) :
    QObject {parent}
{}

Port::~Port() {}

void
Port::sendPacket(const PacketPtr_t &data)
{
    ++m_numberOfPacketsSent;
}

void
Port::receivePacket(const PacketPtr_t &data)
{}

uint8_t Port::getPortNumber()
{
    return m_number;
}
QString Port::getRouterIP()
{
    return m_routerIP;
}
