## IP Class

```cpp
#include <QCoreApplication>
#include <QDebug>
#include "../IP/IP.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    IPv4_t ip1("192.168.1.1");
    qDebug() << "IP1:" << ip1.toString();

    IPv4_t ip2(3232235777); //  192.168.1.1
    qDebug() << "IP2:" << ip2.toString();

    // Comparing
    qDebug() << "IP1 == IP2:" << (ip1 == ip2);

    // Convert to a numeric value
    qDebug() << "IP1 Value:" << ip1.toValue();

    // invalid
    try {
        IPv4_t invalidIP("300.300.300.300");
    } catch (const std::exception &e) {
        qDebug() << "Caught exception for invalid IP:" << e.what();
    }

    return app.exec();
}
```


```cpp
#include <QCoreApplication>
#include <QDebug>
#include "../Packet/Packet.h"
#include "../Port/Port.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Port port;

    port.setPortNumber(1);
    qDebug() << "Port Number:" << port.getPortNumber(); // 1

    port.setRouterIP("192.168.1.1");
    qDebug() << "Router IP:" << port.getRouterIP(); // 192.168.1.1

    DataLinkHeader header(MACAddress("00:11:22:33:44:55"), MACAddress("66:77:88:99:AA:BB"));

    PacketPtr_t packet = QSharedPointer<Packet>::create(header);
    packet->setPayload("Test Data");

    QObject::connect(&port, &Port::packetSent, [](const PacketPtr_t &pkt) {
        qDebug() << "Packet Sent with Data:" << pkt->payload();
    });

    QObject::connect(&port, &Port::packetReceived, [](const PacketPtr_t &pkt) {
        qDebug() << "Packet Received with Data:" << pkt->payload();
    });

    port.sendPacket(packet, 1);
    port.receivePacket(packet);

    return app.exec();
}
```


```cpp
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

```
