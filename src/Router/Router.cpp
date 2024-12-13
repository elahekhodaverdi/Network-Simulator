#include "Router.h"
#include "../PortBindingManager/PortBindingManager.h"
Router::Router(int id, MACAddress macAddress, QObject *parent)
    : Node(id, macAddress, parent)
{
    for (int i = 0; i < maxPorts; ++i) {
        PortPtr_t port = PortPtr_t::create(this);
        port->setPortNumber(i + 1);
        ports.append(port);
    }
}

void Router::setRouterAsDHCPServer()
{
    DHCPServer = true;
}

void Router::setRouterBroken()
{
    broken = true;
}

bool Router::routerIsBroken() const
{
    return broken;
}

PortPtr_t Router::getAnUnboundPort() const
{
    for (const auto &port : ports) {
        if (!PortBindingManager::isBounded(port)) {
            return port;
        }
    }
    return nullptr;
}

int Router::remainingPorts() const
{
    int boundPorts = 0;
    for (const auto &port : ports) {
        if (PortBindingManager::isBounded(port)) {
            ++boundPorts;
        }
    }
    return maxPorts - boundPorts;
}

void Router::setIP(IPv4Ptr_t ip)
{
    m_IP = ip;
    for (const auto &port : ports) {
        port->setRouterIP(ip->toString());
    }
}
