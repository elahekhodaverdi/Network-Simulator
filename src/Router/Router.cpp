#include "Router.h"
#include "../PortBindingManager/PortBindingManager.h"
Router::Router(int id, MACAddress macAddress, QObject *parent)
    : Node(id, macAddress, parent)
{
    for (int i = 0; i < maxPorts; ++i) {
        PortPtr_t port = PortPtr_t::create(this);
        port->setPortNumber(i + 1);
        ports.append(port);
        connect(port.get(), &Port::packetReceived, this, &Router::receivePacket);
        connect(this, &Router::newPacket, port.get(), &Port::sendPacket);
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

void Router::printRoutingTable() const
{
    QTextStream out(stdout);

    out << "Routing Table for Router ID: " << m_id << "\n";
    out << "-------------------------------------------\n";
    out << "Destination IP\tNext Hop IP\tOutgoing Port\n";
    out << "-------------------------------------------\n";

    for (const RoutingTableEntry &entry : routingTable) {
        out << entry.destination->toString() << "\t" << entry.nextHop->toString() << "\t"
            << (entry.outPort ? QString::number(entry.outPort->getPortNumber()) : "N/A") << "\n";
    }

    out << "-------------------------------------------\n";
}

bool Router::isDHCPServer() const
{
    return DHCPServer;
}

void Router::receivePacket(const PacketPtr_t &data, uint8_t port_number)
{
    buffer.append(data);
    qDebug() << "Packet Received from: " << port_number << " Content:" << data->payload();
}

void Router::addRoutingTableEntry(IPv4Ptr_t destination, IPv4Ptr_t nextHop, PortPtr_t outPort)
{
    RoutingTableEntry entry{destination, nextHop, outPort};
    routingTable.append(entry);
}

void Router::sendPacket(QVector<QSharedPointer<PC>> selectedPCs)
{

    qDebug() << "Sending packet from Router:" << m_id;
    if (buffer.isEmpty())
        return;
    PacketPtr_t topPacket = buffer.first();
    buffer.pop_front();
    uint8_t sendPortNumber = findSendPort(topPacket->ipHeader()->destIp());
    Q_EMIT newPacket(topPacket, sendPortNumber);
}

uint8_t Router::findSendPort(IPv4Ptr_t destIP){
    return 0;
    //TODO
}
