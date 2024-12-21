#include "Router.h"
#include "../PortBindingManager/PortBindingManager.h"
#include "../Network/SimulationConfig.h"
#include "../RoutingProtocol/rip.h"
//#include "../RoutingProtocol/OSPF.h"


Router::Router(int id, MACAddress macAddress, int portCount, int bufferSize, QObject *parent)
    : Node(id, macAddress, parent)
    , maxPorts(portCount)
    , maxBufferSize(bufferSize)
{
    connectPortsToSignals();
    initializeRoutingProtocol();
    moveToThread(this);
    this->start();
}

Router::~Router()
{
    for (int i = 0; i < ports.size(); ++i) {
        PortBindingManager::unbind(ports[i]);
        disconnect(ports[i].get(), &Port::packetReceived, this, &Router::receivePacket);
        disconnect(this, &Router::newPacket, ports[i].get(), &Port::sendPacket);
    }
    this->quit();
    this->wait();
}

void Router::connectPortsToSignals()
{
    for (int i = 0; i < maxPorts; ++i) {
        auto port = PortPtr_t::create(this);
        port->setPortNumber(i + 1);
        ports.append(port);
        connect(port.get(), &Port::packetReceived, this, &Router::receivePacket);
        connect(this, &Router::newPacket, port.get(), &Port::sendPacket);
    }
}

void Router::initializeRoutingProtocol()
{
    if (SimulationConfig::routingProtocol == "OSPF"){}
        //routingProtocol = new OSPF();
    else if (SimulationConfig::routingProtocol == "RIP")
        routingProtocol = new RIP();
    else
        routingProtocol = nullptr;

    connect(routingProtocol,
            &RoutingProtocol::NewOutgoingRoutingPacket,
            this,
            &Router::sendRoutingPacket);
}

void Router::setRouterAsDHCPServer()
{
    DHCPServer = true;
}

void Router::markAsBroken()
{
    broken = true;
}

bool Router::isBroken() const
{
    return broken;
}

QList<PortPtr_t> Router::getPorts() const
{
    return ports;
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
    routingProtocol->printRoutingTable();
}

bool Router::isDHCPServer() const
{
    return DHCPServer;
}

void Router::receivePacket(const PacketPtr_t &data, uint8_t portNumber)
{
    if (broken)
        return;
    if (data->packetType() == UT::PacketType::Control){
        handleControlPacket(data, portNumber);
        return;
    }
    buffer.append(data);
    qDebug() << "Packet Received from: " << portNumber << " Content:" << data->payload();
}

void Router::handleControlPacket(const PacketPtr_t &data, uint8_t portNumber){
    if (data->controlType() == UT::PacketControlType::Response){
        routingProtocol->addNewNeighbor(data->ipHeader()->sourceIp(), ports[portNumber - 1]);
        return;
    }
    if (data->controlType() == UT::PacketControlType::Request){
        sendResponsePacket(data, portNumber);
        return;
    }
    if(data->controlType() == UT::PacketControlType::RIP || data->controlType() == UT::PacketControlType::OSPF){
        routingProtocol->processRoutingPacket(data, ports[portNumber - 1]);
        return;
    }
}

void Router::sendResponsePacket(const PacketPtr_t &requestPacket, uint8_t portNumber){
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader(), this);
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setSourceIp(m_IP);
    ipHeader->setDestIp(requestPacket->ipHeader()->sourceIp());
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::Response);
    Q_EMIT newPacket(packet, portNumber);
}


QMap<PortPtr_t, PacketPtr_t> Router::findPacketsToSend(){
    uint8_t numBoundPorts = 0;
    for (auto& port : ports){
        if (PortBindingManager::isBounded(port))
            numBoundPorts++;
    }
    uint8_t numPacketsToSend = 0;
    int bufferIndex = 0;
    QMap<PortPtr_t, PacketPtr_t> portPacketsMap;
    while (numPacketsToSend != numBoundPorts && bufferIndex < buffer.size()){
        auto currentPacket = buffer[bufferIndex];
        auto sendPort = routingProtocol->findOutPort(currentPacket->ipHeader()->destIp());
        if (sendPort == nullptr || portPacketsMap.contains(sendPort)){
            bufferIndex++;
            continue;
        }
        portPacketsMap.insert(sendPort, currentPacket);
        buffer.removeAt(bufferIndex);
        numPacketsToSend++;
    }
    return portPacketsMap;

}

void Router::sendPacket(QVector<QSharedPointer<PC>> selectedPCs)
{
    if (buffer.isEmpty())
        return;
    if (broken)
        return;
    qDebug() << "Sending packets from Router:" << m_id;
    checkCurrentThread();
    for (auto packet : buffer)
        packet->incTotalCycles();
    auto portPacketsMap = findPacketsToSend();
    qDebug() << "map size" << portPacketsMap.size();
    for (auto i = portPacketsMap.cbegin(); i != portPacketsMap.cend(); ++i){
        PacketPtr_t packet = i.value();
        PortPtr_t port = i.key();
        Q_EMIT newPacket(packet, port->getPortNumber());
    }
    for (auto packet : buffer)
        packet->incWaitingCycles();
}

void Router::sendRoutingPacket(PacketPtr_t &packet){
    for (const auto& port : ports){
        if (PortBindingManager::isBounded(port))
            Q_EMIT newPacket(packet, port->getPortNumber());
    }
}


