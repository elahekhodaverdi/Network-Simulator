#include "Router.h"
#include "../PortBindingManager/PortBindingManager.h"
#include "../Network/SimulationConfig.h"

#define INF std::numeric_limits<double>::infinity()

Router::Router(int id, MACAddress macAddress, int portCount, int bufferSize, QObject *parent)
    : Node(id, macAddress, parent)
    , maxPorts(portCount)
    , maxBufferSize(bufferSize)
{
    for (int i = 0; i < maxPorts; ++i) {
        PortPtr_t port = PortPtr_t::create(this);
        port->setPortNumber(i + 1);
        ports.append(port);
        connect(port.get(), &Port::packetReceived, this, &Router::receivePacket);
        connect(this, &Router::newPacket, port.get(), &Port::sendPacket);
    }
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
    out << "---------------------------------------------------------------\n";
    out << QString("%1\t%2\t%3\t%4\t%5\n")
               .arg("Prtcol", -6)
               .arg("Destination IP", -20)
               .arg("Next Hop IP", -20)
               .arg("Out Port", -10)
               .arg("Metric", -6);
    out << "---------------------------------------------------------------\n";

    for (const RoutingTableEntry &entry : routingTable) {
        out << QString("%1\t%2\t%3\t%4\t%5\n")
                   .arg((entry.protocol == UT::RoutingProtocol::OSPF ? "O" : "R"), -6)
                   .arg(entry.destination->toString() + "\\" + entry.subnetMask->toString(),
                        -20)
                   .arg(entry.nextHop->toString(), -20)
                   .arg((entry.outPort ? QString::number(entry.outPort->getPortNumber()) : "N/A"),
                        -10)
                   .arg(entry.metric, -6);
    }

    out << "---------------------------------------------------------------\n";
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
        addNewNeighbor(data->ipHeader()->sourceIp(), portNumber);
        return;
    }
    if (data->controlType() == UT::PacketControlType::Request){
        sendResponsePacket(data, portNumber);
        return;
    }
}

void Router::addNewNeighbor(const IPv4Ptr_t& neighborIP, uint8_t portNumber){
    if (SimulationConfig::routingProtocol == "RIP"){
        updateDistanceVector(neighborIP, 1, neighborIP, portNumber);
        return;
    }
    if (SimulationConfig::routingProtocol == "OSPF"){
        // TODO
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
        auto sendPort = findSendPort(currentPacket->ipHeader()->destIp());
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

PortPtr_t Router::findSendPort(IPv4Ptr_t destIP) {
    for (const RoutingTableEntry &entry : routingTable) {
        if (*entry.destination == *destIP) {
            return entry.outPort;
        }
    }

    //qDebug() << "No route found for destination IP: " << destIP->toString();
    return nullptr;
}


void Router::updateDistanceVector(IPv4Ptr_t destIP, int metric, IPv4Ptr_t neighborIP, uint8_t portNumber){
    int currentMetric = distanceVector.value(destIP, INF);
    if (currentMetric <= metric + 1)
        return;
    distanceVector[destIP] = metric + 1;
    RoutingTableEntry newEntry{
        destIP,
        IPv4Ptr_t::create("255.255.255.0"),
        neighborIP,
        ports[portNumber - 1],
        metric + 1,
        UT::RoutingProtocol::OSPF
    };
    updateRoutingTable(newEntry);
}

void Router::updateRoutingTable(RoutingTableEntry newEntry){
    for (auto &entry : routingTable){
        if (*entry.destination == *newEntry.destination){
            entry = newEntry;
        }
    }
    routingTable.append(newEntry);
}
