#include <QCoreApplication>
#include <QSharedPointer>
#include <QVector>
#include "src/IP/IP.h"
#include "src/MACAddress/MACAddress.h"
#include "src/MACAddress/MACAddressGenerator.h"
#include "src/PC/PC.h"
#include "src/Packet/Packet.h"
#include "src/Port/Port.h"
#include "src/PortBindingManager/PortBindingManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "PC class tests...";

    MACAddress mac1({0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E});
    MACAddress mac2({0x00, 0x2A, 0x2B, 0x3C, 0x4D, 0x5E});
    IPv4Ptr_t ip1 = QSharedPointer<IPv4_t>::create("192.168.1.1");
    IPv4Ptr_t ip2 = QSharedPointer<IPv4_t>::create("192.168.1.2");

    PCPtr_t pc1 = PCPtr_t::create(1, mac1);
    PCPtr_t pc2 = PCPtr_t::create(1, mac2);

    qDebug() << "PC 1 created with ID:" << pc1->getId();
    qDebug() << "PC 1 MAC Address:" << mac1.toString();

    PortPtr_t port1 = PortPtr_t::create();
    port1->setPortNumber(1);
    pc1->setGateway(port1);
    pc1->setIP(ip1);
    qDebug() << "PC 1 assigned IP:" << pc1->getIP()->toString();

    QVector<PCPtr_t> selectedPCs = {pc1};
    pc1->sendPacket(selectedPCs);

    PortPtr_t port2 = PortPtr_t::create();
    port2->setPortNumber(2);

    PortBindingManager::bind(port1, port2);
    PacketPtr_t packet = PacketPtr_t::create(
        DataLinkHeader(MACAddressGenerator::getRandomMAC(), MACAddressGenerator::getRandomMAC()));
    packet->setPayload("Finally sent");
    port2->sendPacket(packet, 2);

    qDebug() << "PC 1 should now have received a packet.";
    return 0;
}
