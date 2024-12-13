#include "PortBindingManager.h"
#include <QDebug>
#include <QPointer>
QMap<PortPtr_t, QList<PortPtr_t>> PortBindingManager::bindings;

PortBindingManager::PortBindingManager(QObject *parent)
    : QObject{parent}
{}

void PortBindingManager::printBindings()
{
    qDebug() << "==================== Port Bindings ====================";
    for (auto it = bindings.begin(); it != bindings.end(); ++it) {
        PortPtr_t keyPort = it.key();
        QList<PortPtr_t> boundPorts = it.value();

        if (keyPort) {
            qDebug() << "Port:" << keyPort->getPortNumber();
        } else {
            qDebug() << "Port: NULL";
        }

        qDebug() << "Bound to:";
        for (const PortPtr_t &boundPort : boundPorts) {
            if (boundPort) {
                qDebug() << "   -> Port:" << boundPort->getPortNumber();
            } else {
                qDebug() << "   -> Port: NULL";
            }
        }
    }
    qDebug() << "======================================================";
}

void PortBindingManager::bind(const PortPtr_t &port1, const PortPtr_t &port2)
{
    if (port1 == port2) {
        qWarning("Can't bind port to itself.");
        return;
    }
    if (isBounded(port1) && bindings[port1].contains(port2)) {
        qWarning("Ports are already bound.");
        return;
    }

    bindings[port1].append(port2);
    bindings[port2].append(port1);
    connect(port1.get(), &Port::packetSent, port2.get(), &Port::receivePacket);
    connect(port2.get(), &Port::packetSent, port1.get(), &Port::receivePacket);
}

bool PortBindingManager::unbind(const PortPtr_t &port1, const PortPtr_t &port2)
{
    if (!bindings.contains(port1) || !bindings[port1].contains(port2)) {
        qWarning("Ports are not currently bound.");
        return false;
    }

    bindings[port1].removeAll(port2);
    bindings[port2].removeAll(port1);

    disconnect(port1.get(), &Port::packetSent, port2.get(), &Port::receivePacket);
    disconnect(port2.get(), &Port::packetSent, port1.get(), &Port::receivePacket);

    return true;
}

bool PortBindingManager::isBounded(const PortPtr_t &port)
{
    return bindings.contains(port);
}
