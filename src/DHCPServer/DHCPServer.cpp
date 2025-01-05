#include "DHCPServer.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QThread>

DHCPServer::DHCPServer(int asID, QObject *parent)
    : QObject{parent}
    , m_asID(asID)
{
    m_ipRange = QString("192.168.%1.").arg(asID * 100);
    initialLogFile(asID);
}

DHCPServer::~DHCPServer()
{
}

QString DHCPServer::getIP(int id){
    return m_ipRange + QString::number(id);
}

void DHCPServer::handleDiscoveryPacket(PacketPtr_t packet)
{
    int id = packet->payload().toInt();
    if (sentOffers.contains(id))
        return;
    sentOffers.append(id);
    QString ip = getIP(id);
    QJsonObject jsonObject;
    jsonObject["id"] = id;
    jsonObject["ip"] = ip;
    QJsonDocument jsonDoc(jsonObject);
    QByteArray payload = jsonDoc.toJson(QJsonDocument::Compact);

    PacketPtr_t offerPacket = PacketPtr_t::create(DataLinkHeader());
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setTTL(5);
    offerPacket->setIPHeader(ipHeader);
    offerPacket->setPacketType(UT::PacketType::Control);
    offerPacket->setControlType(UT::PacketControlType::DHCPOffer);
    offerPacket->setPayload(payload);

    logToFile(id, ip, packet->dataLinkHeader().sourceMACAddress().toString());
    Q_EMIT newPacket(offerPacket, nullptr);
}

void DHCPServer::handleRequestPacket(PacketPtr_t packet){
    PacketPtr_t ackPacket = PacketPtr_t::create(DataLinkHeader());
    IPv4Ptr_t sourceIp = packet->ipHeader()->sourceIp();
    if (sentAcks.contains(sourceIp->toString()))
        return;
    sentAcks.append(sourceIp->toString());
    QSharedPointer<IPHeader> ipHeader = QSharedPointer<IPHeader>::create();
    ipHeader->setTTL(5);
    ipHeader->setDestIp(sourceIp);
    ackPacket->setIPHeader(ipHeader);
    ackPacket->setPacketType(UT::PacketType::Control);
    ackPacket->setControlType(UT::PacketControlType::DHCPAcknowledge);
    Q_EMIT newPacket(ackPacket, nullptr);
}

void DHCPServer::logToFile(int nodeID, const QString &ip, const QString &macAddress)
{
    if (logFilePath.isEmpty()) {
        qWarning() << "Log file path is not set.";
        return;
    }

    QFile file(logFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString("%1,%2,%3\n").arg(nodeID).arg(macAddress).arg(ip);
        file.close();
    } else {
        qWarning() << "Unable to open log file for appending: " << logFilePath;
    }
}

void DHCPServer::initialLogFile(int asID)
{
    QString projectDir = QString(PROJECT_DIR_PATH);
    QString logDirPath = QDir(projectDir).filePath("logs");
    logFilePath = QDir(logDirPath).filePath(QString("dhcp_server_%1.txt").arg(asID));

    QDir logDir(logDirPath);
    if (!logDir.exists()) {
        if (!logDir.mkpath(".")) {
            qWarning() << "Unable to create log directory: " << logDirPath;
            return;
        }
    }

    QFile file(logFilePath);
    if (file.exists()) {
        if (!file.remove()) {
            qWarning() << "Unable to delete existing log file: " << logFilePath;
            return;
        }
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "router_id,mac_address,ip\n";
        file.close();
    } else {
        qWarning() << "Unable to create log file: " << logFilePath;
    }
}

void DHCPServer::cleanLogFile()
{
    if (m_asID != -1)
        initialLogFile(m_asID);
}
