#include "PortBindingManager.h"
#include <QPointer>

PortBindingManager::PortBindingManager(QObject *parent) :
    QObject {parent}
{}

void
PortBindingManager::bind(const PortPtr_t &port1, const PortPtr_t &port2)
{
    if (isBounded(port1) && bindings[port1].contains(port2)) {
        qWarning("Ports are already bound.");
        return;
    }

    bindings[port1].append(port2);
    bindings[port2].append(port1);
    connect(port1.get(), &Port::packetSent, port2.get(), &Port::receivePacket);
    connect(port2.get(), &Port::packetSent, port1.get(), &Port::receivePacket);
    Q_EMIT bindingChanged(port1->getRouterIP(),
                          port1->getPortNumber(),
                          port2->getRouterIP(),
                          port2->getPortNumber(),
                          true);
}

bool
PortBindingManager::unbind(const PortPtr_t &port1, const PortPtr_t &port2)
{
    if (!bindings.contains(port1) || !bindings[port1].contains(port2)) {
        qWarning("Ports are not currently bound.");
        return false;
    }

    bindings[port1].removeAll(port2);
    bindings[port2].removeAll(port1);

    disconnect(port1.get(), &Port::packetSent, port2.get(), &Port::receivePacket);
    disconnect(port2.get(), &Port::packetSent, port1.get(), &Port::receivePacket);

    Q_EMIT bindingChanged(port1->getRouterIP(),
                          port1->getPortNumber(),
                          port2->getRouterIP(),
                          port2->getPortNumber(),
                          false);
    return true;
}

bool PortBindingManager::isBounded(const PortPtr_t &port)
{
    return bindings.contains(port);
}
