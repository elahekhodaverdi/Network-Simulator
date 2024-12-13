#include "PC.h"

PC::PC(int id, MACAddress macAddress, QObject *parent)
    : Node(id, macAddress, parent)
{
}

PC::PC(int id, QObject *parent)
    : Node(id, parent)
{
}

void PC::sendPacket(QVector<QSharedPointer<PC>> selectedPCs) {
    if (!selectedPCs.contains(this))
        return;
    qDebug() << "sending packet from " << m_id;
}

void PC::setIP(IPv4Ptr_t ip)
{
    m_IP = ip;
    gateway->setRouterIP(ip->toString());
    connect(gateway.get(), &Port::packetReceived, this, &PC::receivePacket);
    connect(this, &PC::newPacket, gateway.get(), &Port::sendPacket);
}
