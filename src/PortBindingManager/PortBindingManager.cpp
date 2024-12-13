#include "PortBindingManager.h"
#include <QPointer>

PortBindingManager::PortBindingManager(QObject *parent) :
    QObject {parent}
{}

void
PortBindingManager::bind(const PortPtr_t &port1, const PortPtr_t &port2)
{
    connect(port1.get(), &Port::packetSent, port2.get(), &Port::receivePacket);
    connect(port2.get(), &Port::packetSent, port1.get(), &Port::receivePacket);
}

bool
PortBindingManager::unbind(const PortPtr_t &port1, const PortPtr_t &port2)
{
    disconnect(port1.get(), &Port::packetSent, port2.get(), &Port::receivePacket);
    disconnect(port2.get(), &Port::packetSent, port1.get(), &Port::receivePacket);
}
