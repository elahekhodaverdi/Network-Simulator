#include "Router.h"

Router::Router(int id, MACAddress macAddress, QObject *parent)
    : Node(id, macAddress, parent)
{
}

void Router::setRouterAsDHCPServer()
{
    DHCPServer = true;
}
void Router::setRouterBroken()
{
    broken = true;
}
bool Router::routerIsBroken()
{
    return broken;
}

void Router::addPort(const PortPtr_t &port)
{
    ports.append(port);
}
