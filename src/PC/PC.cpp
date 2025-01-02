#include "PC.h"
#include <QRandomGenerator>
#include "../Network/Network.h"
#include "../PortBindingManager/PortBindingManager.h"

PC::PC(int id, MACAddress macAddress, QObject *parent) :
    Node(id, macAddress, parent), m_gateway(PortPtr_t::create(this))
{
    moveToThread(this);
    this->start();
    setupGateway();
}

PC::PC(int id, QObject *parent)
    : Node(id, parent)
    , m_gateway(PortPtr_t::create(this))
{
    setupGateway();
    moveToThread(this);
    this->start();
}

PC::~PC()
{
    qDebug() << "PC dest";
    PortBindingManager::unbind(m_gateway);
    if (m_gateway && !m_gateway.isNull()) {
        disconnect(m_gateway.get(), &Port::packetReceived, this, &PC::receivePacket);
        disconnect(this, &PC::newPacket, m_gateway.get(), &Port::sendPacket);
        m_gateway.clear();
    }
    this->quit();
    this->wait();
}

void PC::handleNewTick(UT::Phase phase){
    if (phase != m_currentPhase){
        handlePhaseChange(phase);
    }
    if (shouldSendNewPacket)
        sendNewPacket();
}

void PC::setShouldSendPacket(QVector<QSharedPointer<PC>> selectedPCs)
{
    if (selectedPCs.contains(this)){
        shouldSendNewPacket = true;
        return;
    }
    shouldSendNewPacket = false;
}

void PC::handlePhaseChange(const UT::Phase nextPhase){
    m_currentPhase = nextPhase;
    if (m_currentPhase == UT::Phase::DHCP){
        sendDiscoveryDHCP();
    }
}

void PC::sendNewPacket(){
    PacketPtr_t packet = createNewPacket();
    Q_EMIT newPacket(packet, m_gateway->getPortNumber());
    Q_EMIT packetsSent(1);
}


PCPtr_t PC::chooseRandomPC()
{
    PCPtr_t randomPC;
    do {
        int randomIndex = QRandomGenerator::global()->bounded(Network::PCs.size());
        randomPC = Network::PCs.at(randomIndex);
    } while (randomPC.data() == this);

    return randomPC;
}

PacketPtr_t PC::createNewPacket()
{
    PCPtr_t destinationPC = chooseRandomPC();

    PacketPtr_t packet = PacketPtr_t::create(
        DataLinkHeader(this->getMACAddress(), destinationPC->getMACAddress()));

    if (m_IP && destinationPC->m_IP) {
        QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
        ipHeader->setSourceIp(m_IP);
        ipHeader->setDestIp(destinationPC->getIP());
        packet->setIPHeader(ipHeader);
    } else {
        qDebug() << "Source or destination IP not set.";
        return nullptr;
    }

    QByteArray payload = QByteArray("Hello from PC ") + QByteArray::number(m_id);
    packet->setPayload(payload);
    uint16_t checksum = packet->ipHeader()->calculateHeaderChecksum(payload);
    packet->ipHeader()->setHeaderChecksum(checksum);

    packet->setSequenceNumber(QRandomGenerator::global()->generate());
    packet->setPacketType(UT::PacketType::Data);
    packet->incTotalCycles();

    return packet;
}


void PC::handleControlPacket(const PacketPtr_t &data){
    if (data->controlType() == UT::PacketControlType::DHCPOffer){
        handleOfferDHCP(data, m_gateway);
    }
    else if (data->controlType() == UT::PacketControlType::DHCPAcknowledge){
        handleAckDHCP(data, m_gateway);
    } else if (data->controlType() == UT::PacketControlType::Request) {
        handleRequestPacket(data, m_gateway);
        return;
    }
}

void PC::receivePacket(const PacketPtr_t &data, uint8_t portNumber)
{
    if (data->packetType() == UT::PacketType::Control){
        handleControlPacket(data);
    } else
        Q_EMIT packetReceived(data);
}

void PC::broadcastPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort)
{
    if (triggeringPort != nullptr && triggeringPort->getPortNumber() == m_gateway->getPortNumber())
        return;
    Q_EMIT newPacket(packet, m_gateway->getPortNumber());
}

void PC::setIP(IPv4Ptr_t ip)
{
    m_IP = ip;
    if (m_gateway)
        m_gateway->setRouterIP(ip->toString());
}

PortPtr_t PC::gateway(){
    return m_gateway;
}

void PC::setupGateway()
{
    connect(m_gateway.get(), &Port::packetReceived, this, &PC::receivePacket);
    connect(this, &PC::newPacket, m_gateway.get(), &Port::sendPacket);
}

void PC::addPacketForBroadcast(const PacketPtr_t &packet, PortPtr_t triggeringPort){
    broadcastPacket(packet, triggeringPort);
}
