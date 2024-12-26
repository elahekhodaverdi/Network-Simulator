#include "Packet.h"

Packet::Packet(DataLinkHeader dataLinkHeader, QObject *parent)
    : QObject(parent)
    , m_packetType(UT::PacketType::Data)
    , m_sequenceNumber(0)
    , m_waitingCycles(0)
    , m_totalCycles(0)
    , m_dataLinkHeader(dataLinkHeader)
    , m_ipHeader(QSharedPointer<IPHeader>::create(this))
{}

Packet::~Packet()
{
    m_path.clear();
    m_payload.clear();
    m_tcpHeader.reset();
    m_controlType.reset();
}

void Packet::addToPath(QString ip){
    m_path.append(ip);
}

void Packet::incWaitingCycles(){
    m_waitingCycles++;
}

void Packet::incTotalCycles(){
    m_totalCycles++;
}

void Packet::setPacketType(UT::PacketType packetType){
    m_packetType = packetType;
}

void Packet::setPayload(QByteArray payload){
    m_payload = payload;
}

void Packet::setSequenceNumber(uint32_t sequenceNumber){
    m_sequenceNumber = sequenceNumber;
}

void Packet::setWaitingCycles(uint32_t waitingCycles){
    m_waitingCycles = waitingCycles;
}

void Packet::setTotalCycles(uint32_t totalCycles){
    m_totalCycles = totalCycles;
}

void Packet::setIPHeader(QSharedPointer<IPHeader> ipHeader){
    m_ipHeader = ipHeader;
}

void Packet::setControlType(UT::PacketControlType controlType){
    m_controlType = controlType;
}

UT::PacketType Packet::packetType(){
    return m_packetType;
}

QByteArray Packet::payload(){
    return m_payload;
}

uint32_t Packet::sequenceNumber(){
    return m_sequenceNumber;
}

uint32_t Packet::waitingCycles(){
    return m_waitingCycles;
}
uint32_t Packet::totalCycles(){
    return m_totalCycles;
}
QList<QString> Packet::path(){
    return m_path;
}

QSharedPointer<IPHeader> Packet::ipHeader(){
    return m_ipHeader;
}


UT::PacketControlType Packet::controlType(){
    return m_controlType.value();
}

bool Packet::isDHCPPacket()
{
    return (m_controlType.value() == UT::PacketControlType::DHCPAcknowledge)
           || (m_controlType.value() == UT::PacketControlType::DHCPDiscovery)
           || (m_controlType.value() == UT::PacketControlType::DHCPRequest)
           || (m_controlType.value() == UT::PacketControlType::DHCPOffer)
           || (m_controlType.value() == UT::PacketControlType::DHCPNak);
}
