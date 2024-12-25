#include "Router.h"
#include "../PortBindingManager/PortBindingManager.h"
#include "../Network/SimulationConfig.h"
#include "../RoutingProtocol/rip.h"
#include "../Simulator/Simulator.h"
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
    if (Simulator::simulationConfig.routingProtocol == "OSPF"){}
        //routingProtocol = new OSPF();
    else if (Simulator::simulationConfig.routingProtocol == "RIP")
        routingProtocol = new RIP();
    else
        routingProtocol = nullptr;

    connect(routingProtocol,
            &RoutingProtocol::NewOutgoingRoutingPacket,
            this,
            &Router::sendRoutingPacket);
}

void Router::handleNewTick(UT::Phase phase){
    packetsToSend.clear();
    if (m_currentPhase != phase){
        handlePhaseChange(phase);
    }
    for (int i = 0; i < buffer.size(); i++){
        buffer[i].first->incTotalCycles();
    }
    updateBuffer();
    sendPackets();
    for (int i = 0; i < buffer.size(); i++){
        buffer[i].first->incWaitingCycles();
    }
}

void Router::updateBuffer(){
    int bufferIndex = 0;
    while (packetsToSend.size() < numBoundPorts() && bufferIndex < buffer.size()){
        auto currentPacket = buffer[bufferIndex].first;
        auto packetInPort = buffer[bufferIndex].second;
        if (currentPacket->packetType() == UT::PacketType::Control){
            if (packetsToSend.size() > 0){
                bufferIndex++;
                continue;
            }
            broadcastPacket(currentPacket, packetInPort);
            buffer.removeAt(bufferIndex);
            continue;
        }
        auto sendPort = routingProtocol->findOutPort(currentPacket->ipHeader()->destIp());
        if (sendPort == nullptr || packetsToSend.contains(sendPort)){
            bufferIndex++;
            continue;
        }
        packetsToSend.insert(sendPort, currentPacket);
        buffer.removeAt(bufferIndex);
    }
}

void Router::handlePhaseChange(const UT::Phase nextPhase){
    m_currentPhase = nextPhase;
    if (m_currentPhase == UT::Phase::DHCP){
        if (isDHCPServer()){
            QString ip = dhcpServer->getIP(m_id);
            setIP(IPv4Ptr_t::create(ip));
        }
        else
            sendDiscoveryDHCP();
        return;
    }
}

void Router::setAsDHCPServer(QString ipRange)
{
    dhcpServer = QSharedPointer<DHCPServer>::create(ipRange);
    QObject::connect(dhcpServer.get(),
                     &DHCPServer::newPacket,
                     this,
                     &Router::addNewPacketTobBuffer);
}

void Router::addNewPacketTobBuffer(PacketPtr_t packet, PortPtr_t triggeringPort){
    if (m_IP)
        packet->ipHeader()->setSourceIp(m_IP);
    buffer.append(qMakePair(packet, triggeringPort));
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

int Router::numRemainingPorts() const
{
    int boundPorts = 0;
    for (const auto &port : ports) {
        if (PortBindingManager::isBounded(port)) {
            ++boundPorts;
        }
    }
    return maxPorts - boundPorts;
}

int Router::numBoundPorts() const
{
    return maxPorts - numRemainingPorts();
}

void Router::setIP(IPv4Ptr_t ip)
{
    m_IP = ip;
    for (const auto &port : ports) {
        port->setRouterIP(ip->toString());
    }
    qDebug() << "Router with ID: " << m_id << "set it IP: " << m_IP->toString();
}

void Router::printRoutingTable() const
{
    routingProtocol->printRoutingTable();
}

bool Router::isDHCPServer() const
{
    return (dhcpServer != nullptr);
}

void Router::receivePacket(const PacketPtr_t &data, uint8_t portNumber)
{
    if (broken)
        return;
    //qDebug() << "Packet Received from: " << portNumber << " Content:" << data->payload();
    data->ipHeader()->decTTL();
    if (data->packetType() == UT::PacketType::Control){
        handleControlPacket(data, portNumber);
    }
    else if (data->ipHeader()->ttl() > 0)
        buffer.append(qMakePair(data, ports[portNumber - 1]));
}

void Router::handleControlPacket(const PacketPtr_t &data, uint8_t portNumber){
    if (data->controlType() == UT::PacketControlType::DHCPDiscovery ){
        if (isDHCPServer()){
            dhcpServer->handleDiscoveryPacket(data);
        }
        else if (data->ipHeader()->ttl() > 0){
            buffer.append(qMakePair(data, ports[portNumber - 1]));
        }
    }
    else if (data->controlType() == UT::PacketControlType::DHCPOffer){
        handleOfferDHCP(data, ports[portNumber - 1]);
    }
    else if (data->controlType() == UT::PacketControlType::DHCPRequest){
        if (isDHCPServer()){
            dhcpServer->handleRequestPacket(data);
        }
        else if (data->ipHeader()->ttl() > 0){
            buffer.append(qMakePair(data, ports[portNumber - 1]));
        }
    }
    else if (data->controlType() == UT::PacketControlType::DHCPAcknowledge){
        handleAckDHCP(data, ports[portNumber - 1]);
    }
    // if (data->controlType() == UT::PacketControlType::Response){
    //     routingProtocol->addNewNeighbor(data->ipHeader()->sourceIp(), ports[portNumber - 1]);
    //     return;
    // }
    // if (data->controlType() == UT::PacketControlType::Request){
    //     sendResponsePacket(data, portNumber);
    //     return;
    // }
    // if(data->controlType() == UT::PacketControlType::RIP || data->controlType() == UT::PacketControlType::OSPF){
    //     routingProtocol->processRoutingPacket(data, ports[portNumber - 1]);
    //     return;
    // }
}

void Router::sendResponsePacket(const PacketPtr_t &requestPacket, uint8_t portNumber){
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader());
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setSourceIp(m_IP);
    ipHeader->setDestIp(requestPacket->ipHeader()->sourceIp());
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::Response);
    Q_EMIT newPacket(packet, portNumber);
}

void Router::broadcastPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort){
    for (const auto& port : ports){
        if (!PortBindingManager::isBounded(port))
            continue;
        if (port->isInterAsConnection())
            continue;
        if (triggeringPort != nullptr && port->getPortNumber() == triggeringPort->getPortNumber())
            continue;
        packetsToSend.insert(port, packet);
    }
}

void Router::sendPackets()
{
    //qDebug() << m_id << " " << packetsToSend.size();
    for (auto i = packetsToSend.cbegin(); i != packetsToSend.cend(); ++i){
        PacketPtr_t packet = i.value();
        PortPtr_t port = i.key();
        Q_EMIT newPacket(packet, port->getPortNumber());
    }
}

void Router::sendRoutingPacket(PacketPtr_t &packet, PortPtr_t triggeringPort){
    for (const auto& port : ports){
        if (!PortBindingManager::isBounded(port))
            continue;
        if (port != triggeringPort && port->getPortNumber() == triggeringPort->getPortNumber())
            continue;
        Q_EMIT newPacket(packet, port->getPortNumber());
    }
}

void Router::addPacketForBroadcast(const PacketPtr_t &packet, PortPtr_t triggeringPort){
    buffer.append(qMakePair(packet, triggeringPort));
}


