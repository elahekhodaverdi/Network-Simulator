#include "Router.h"
#include "../Network/SimulationConfig.h"
#include "../PortBindingManager/PortBindingManager.h"
#include "../RoutingProtocol/OSPF.h"
#include "../RoutingProtocol/rip.h"
#include "../Simulator/Simulator.h"

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
    if (Simulator::simulationConfig.routingProtocol == "OSPF")
        routingProtocol = new OSPF();
    else if (Simulator::simulationConfig.routingProtocol == "RIP")
        routingProtocol = new RIP();
    else
        routingProtocol = new RIP();
    routingProtocol->moveToThread(this->thread());
    connect(routingProtocol,
            &RoutingProtocol::newRoutingPacket,
            this,
            &Router::addPacketTobBuffer);
    connect(routingProtocol, &RoutingProtocol::noUpdateAtRoutingTable, this, &Router::routingIsDone);
}

void Router::handleNewTick(UT::Phase phase){
    packetsToSend.clear();

    if (m_currentPhase != phase) {
        handlePhaseChange(phase);
    }
    routingProtocol->handleNewTick(phase);
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
    buffer.clear();
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
    if (broken)
        return;
    if (m_currentPhase == UT::Phase::IdentifyNeighbours) {
        sendRequestPacket();
    }
    if (m_currentPhase == UT::Phase::Routing) {
        routingProtocol->startRouting();
    }
}

void Router::setAsDHCPServer(QString ipRange)
{
    dhcpServer = QSharedPointer<DHCPServer>::create(ipRange);
    QObject::connect(dhcpServer.get(),
                     &DHCPServer::newPacket,
                     this,
                     &Router::addPacketTobBuffer);
}

void Router::addPacketTobBuffer(PacketPtr_t packet, PortPtr_t triggeringPort){
    if (buffer.size() + 1 > maxBufferSize){
        qDebug() << maxBufferSize;
        if (packet->packetType() == UT::PacketType::Control){
            buffer.removeLast();
            buffer.push_front(qMakePair(packet, triggeringPort));
        }
        return;
    }
    // if (buffer.size() > 1000)
    // //qDebug() << buffer.size();
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
    routingProtocol->setRouterIP(ip);
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
    if (data->ipHeader()->ttl() <= 0)
        return;
    if (broken && !data->isDHCPPacket())
        return;

    data->ipHeader()->decTTL();
    if (m_IP)
        data->addToPath(m_IP->toString());
    if (data->packetType() == UT::PacketType::Control) {
        Q_EMIT packetReceived(data);
        handleControlPacket(data, portNumber);
    } else if (!broken)
        addPacketTobBuffer(data, ports[portNumber - 1]);
}

void Router::handleControlPacket(const PacketPtr_t &data, uint8_t portNumber){
    if (data->controlType() == UT::PacketControlType::DHCPDiscovery ){
        if (isDHCPServer()){
            dhcpServer->handleDiscoveryPacket(data);
        } else if (data->payload().toInt() != m_id) {
            addPacketTobBuffer(data, ports[portNumber - 1]);
        }
    }
    else if (data->controlType() == UT::PacketControlType::DHCPOffer){
        handleOfferDHCP(data, ports[portNumber - 1]);
    }
    else if (data->controlType() == UT::PacketControlType::DHCPRequest){
        if (isDHCPServer()){
            dhcpServer->handleRequestPacket(data);
        } else {
            if (m_IP == nullptr || *m_IP != *data->ipHeader()->sourceIp())
                addPacketTobBuffer(data, ports[portNumber - 1]);
        }
    }
    else if (data->controlType() == UT::PacketControlType::DHCPAcknowledge){
        handleAckDHCP(data, ports[portNumber - 1]);
    } else if (data->controlType() == UT::PacketControlType::Request) {
        handleRequestPacket(data, ports[portNumber - 1]);
    } else if (data->controlType() == UT::PacketControlType::Response) {
        handleResponsePacket(data, ports[portNumber - 1]);
    } else if (data->controlType() == UT::PacketControlType::RIP
               || data->controlType() == UT::PacketControlType::OSPF) {
        routingProtocol->processRoutingPacket(data, ports[portNumber - 1]);
    }
}

void Router::broadcastPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort)
{
    for (const auto& port : ports){
        if (!PortBindingManager::isBounded(port))
            continue;
        if (port->isInterAsConnection() && packet->isDHCPPacket())
            continue;
        if (triggeringPort != nullptr && port->getPortNumber() == triggeringPort->getPortNumber())
            continue;
        PacketPtr_t copiedPacket = PacketPtr_t::create(*packet);
        packetsToSend.insert(port, copiedPacket);
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

void Router::sendRequestPacket()
{
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader());
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setSourceIp(m_IP);
    packet->setIPHeader(ipHeader);
    packet->setPacketType(UT::PacketType::Control);
    packet->setControlType(UT::PacketControlType::Request);
    addPacketForBroadcast(packet, nullptr);
}

void Router::handleResponsePacket(const PacketPtr_t &packet, PortPtr_t triggeringPort)
{
    routingProtocol->addNewNeighbour(packet->ipHeader()->sourceIp(), triggeringPort);
}

void Router::addPacketForBroadcast(const PacketPtr_t &packet, PortPtr_t triggeringPort)
{
    addPacketTobBuffer(packet, triggeringPort);
}

void Router::routingIsDone()
{
    // qDebug() << "emit protocol" << m_id;
    Q_EMIT routingProtocolIsDone();
}
