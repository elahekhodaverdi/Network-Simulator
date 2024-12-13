#include <QCoreApplication>
#include <QSharedPointer>
#include <QVector>
#include "src/IP/IP.h"
#include "src/MACAddress/MACAddress.h"
#include "src/MACAddress/MACAddressGenerator.h"
#include "src/PC/PC.h"
#include "src/Port/Port.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "Starting PC class tests...";

    // Check initial values
    MACAddress mac1({0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E});
    PCPtr_t pc1 = PCPtr_t::create(1, mac1);
    qDebug() << "PC 1 created with ID:" << pc1->getId();
    qDebug() << "PC 1 MAC Address:" << mac1.toString();

    // Set IP and validate
    IPv4Ptr_t ip1 = QSharedPointer<IPv4_t>::create("192.168.1.1");
    // pc1->setIP(ip1);
    // qDebug() << "PC 1 assigned IP:" << pc1->getIP()->toString();

    // Simulate packet sending
    PortPtr_t port1 = PortPtr_t::create();
    pc1->gateway() = port1;
    QVector<PCPtr_t> selectedPCs = {pc1};
    pc1->sendPacket(selectedPCs);

    // Test 4: Create another PC and test communication
    MACAddress mac2({0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA});
    PCPtr_t pc2 = PCPtr_t::create(2, mac2);
    IPv4Ptr_t ip2 = QSharedPointer<IPv4_t>::create("192.168.1.2");
    pc2->setIP(ip2);
    qDebug() << "PC 2 created with ID:" << pc2->getId();
    qDebug() << "PC 2 assigned IP:" << pc2->getIP()->toString();

    // Connect PCs and test packet transmission
    QObject::connect(port1.get(), &Port::packetReceived, pc2.get(), &PC::receivePacket);
    PacketPtr_t packet = PacketPtr_t::create(
        DataLinkHeader(MACAddressGenerator::getRandomMAC(), MACAddressGenerator::getRandomMAC()));
    port1->sendPacket(packet, 1);

    qDebug() << "PC 2 should now have received a packet.";

    // End of tests
    qDebug() << "PC class tests completed.";
    return 0;
}
