#include "Port.h"
#include <QDebug>
Port::Port(QObject *parent) :
    QObject {parent}
{}

void
Port::sendPacket(const PacketPtr_t &data, uint8_t port_number)
{
    if (port_number != m_number)
        return;

    Q_EMIT packetSent(data);
    ++m_numberOfPacketsSent;
}

void Port::receivePacket(const PacketPtr_t &data)
{
    Q_EMIT packetReceived(data, m_number);
}

uint8_t Port::getPortNumber()
{
    return m_number;
}
QString Port::getRouterIP()
{
    return m_routerIP;
}

void Port::setPortNumber(uint8_t number)
{
    m_number = number;
}

void Port::setRouterIP(QString routerIP)
{
    m_routerIP = routerIP;
}

void Port::setIsInterASConnection(){
    m_isInterASConnection = true;
}

bool Port::isInterAsConnection(){
    return m_isInterASConnection;
}
