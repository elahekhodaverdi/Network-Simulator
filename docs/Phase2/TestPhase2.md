## Phase 2 Tests

### **Testing the IP**

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

**What is being tested**:
1. **IP Initialization**: We test the `IPv4_t` class for its ability to initialize an IP address from both a string representation and a numeric value.
2. **IP Comparison**: We check if the `==` operator works correctly when comparing two IP addresses, ensuring that it identifies equal and unequal addresses.
3. **IP to Numeric Conversion**: We verify that the `toValue()` method can convert an IP address into its equivalent numeric form.
4. **Invalid IP Address**: We test the handling of an invalid IP address and ensure that the program throws an exception for values like `300.300.300.300`.

**Desired Output**:
1. **IP1 and IP2**: Both should display as `192.168.1.1`, demonstrating that initialization from numeric value and string works correctly.
2. **IP Comparison**: The comparison should output `true` since both `ip1` and `ip2` represent the same address.
3. **IP to Value**: The numeric value of `192.168.1.1` (3232235777) should be printed.
4. **Invalid IP**: An exception is caught, and the message for invalid IP is printed.

#### **Output**
![IP Test Phase 2](../imgs/IPTest_P2.png)

### **Testing the Port**

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

**What is being tested**:
1. **Port Initialization**: The `Port` class is tested for its ability to set and get the port number and router IP.
2. **Packet Sending and Receiving**: We test the `sendPacket()` and `receivePacket()` methods by creating a packet and verifying that it is sent and received correctly.
3. **Signal-Slot Mechanism**: The test ensures that signals like `packetSent` and `packetReceived` are emitted and that connected slots handle the events properly.

**Desired Output**:
1. **Port Number**: The port number `1` should be printed, confirming that the `setPortNumber` and `getPortNumber` methods work.
2. **Router IP**: The router IP `192.168.1.1` should be displayed, confirming that the IP setter and getter work.
3. **Packet Sending and Receiving**: Debug logs will show the packet's payload when it is sent and received, ensuring the correct functionality of packet communication.

#### **Output**
![Port Test Phase 2](../imgs/PortTest_P2.png)

### **Testing the Router**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include "src/MACAddress/MACAddressGenerator.h"
#include "src/Packet/Packet.h"
#include "src/PortBindingManager/PortBindingManager.h"
#include "src/Router/Router.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // MAC address for the router
    MACAddress macAddress = MACAddressGenerator::generateRandomMAC();

    // Initialize Router
    qDebug() << "Creating Router with ID 1...";
    Router router(1, macAddress);

    // Check Router properties
    qDebug() << "Router ID:" << router.getId();
    qDebug() << "MAC Address:" << macAddress.toString();
    qDebug() << "Router has" << router.remainingPorts() << "remaining ports.";

    // Set Router as DHCP Server
    router.setRouterAsDHCPServer();
    qDebug() << "Is DHCP Server?" << router.isDHCPServer();

    // Break the Router
    router.setRouterBroken();
    qDebug() << "Is Router Broken?" << router.routerIsBroken();

    // Bind Ports
    qDebug() << "Binding Ports...";
    PortPtr_t port1 = router.getAnUnboundPort();
    PortPtr_t port2 = PortPtr_t::create();
    port2->setPortNumber(11);

    if (port1 && port2) {
        PortBindingManager::bind(port1, port2);
        qDebug() << "Bound port" << port1->getPortNumber() << "to port" << port2->getPortNumber();
    } else {
        qDebug() << "Not enough unbound ports!";
    }

    // Check remaining unbound ports
    qDebug() << "Router has" << router.remainingPorts() << "remaining ports.";

    // Set Router IP
    IPv4Ptr_t routerIP = IPv4Ptr_t::create("192.168.1.1");
    router.setIP(routerIP);
    qDebug() << "Router IP set to:" << router.getIP()->toString();

    // Add a packet to the buffer and test packet forwarding
    qDebug() << "Testing Packet Sending and Receiving...";
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader(macAddress, macAddress));
    packet->setPacketType(UT::PacketType::Data);
    packet->setPayload("Test Router Recieving Packet");

    port2->sendPacket(packet, 11);

    // Print Routing Table
    IPv4Ptr_t IP1 = IPv4Ptr_t::create("192.168.1.2");
    IPv4Ptr_t IP2 = IPv4Ptr_t::create("192.168.1.3");
    router.addRoutingTableEntry(IP1, 24, IP2, port1, 3, UT::RoutingProtocol::OSPF);
    router.printRoutingTable();

    return app.exec();
}
```

**What is being tested**:
1. **Router Initialization**: We test the creation of a router and check its properties such as ID, MAC address, and remaining ports.
2. **DHCP Server Status**: We verify if the router can be set as a DHCP server and check its status.
3. **Router Failure**: We simulate the router's failure by calling the `setRouterBroken()` method and verify its status.
4. **Port Binding**: We test binding ports using the `PortBindingManager` and check if the correct ports are bound together.
5. **Packet Forwarding**: The router is tested for forwarding packets from one port to another.

**Desired Output**:
1. **Router Properties**: The router's ID, MAC address, and remaining ports should be printed correctly.
2. **DHCP Server Status**: The output should confirm whether the router is acting as a DHCP server.
3. **Router Failure**: The router's failure status should be checked and displayed as `true` or `false`.
4. **Port Binding**: The output should confirm successful binding of ports and show the remaining unbound ports.
5. **Packet Forwarding**: A packet should be sent from one port and received by another, confirming that packet forwarding works correctly.

#### **Output**
![Router Test Phase 2](../imgs/RouterTest_P2.png)


### **Testing the Topology Builder**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include "../PortBindingManager/PortBindingManager.h"
#include "../Topology/TopologyBuilder.h"

void printBoundedPorts(const QList<PortPtr_t>& ports)
{
    qDebug() << "--------------------------------------";

    for (const auto& port : ports) {
        if (!PortBindingManager::isBounded(port))
            continue;

        qDebug() << "Port with ID" << port->getPortNumber();
        PortBindingManager::printBindingsForaPort(port);
    }

    qDebug() << "--------------------------------------\n";
}

void testTopology(const QString& topologyName,
                  UT::TopologyType topologyType,
                  int numberOfNodes,
                  uint16_t asID)
{
    qDebug() << "Testing" << topologyName << "Topology...";

    auto routers = TopologyBuilder::buildTopology(numberOfNodes, topologyType, asID);

    for (const auto& router : routers) {
        qDebug() << "\nRouter" << router->getId() << "with IP" << router->getIP()->toString();
        printBoundedPorts(router->getPorts());
    }

    qDebug() << "\n" << topologyName << "Topology Test Complete!";
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    // Test different topologies
    testTopology("Mesh", UT::TopologyType::Mesh, 9, 1);
    testTopology("Torus", UT::TopologyType::Torus, 9, 2);
    testTopology("Ring-Star", UT::TopologyType::RingStar, 7, 2);

    return a.exec();
}
```

**What is being tested**:
1. **Topology Generation**: We test the creation of topologies (`Mesh`, `Torus`, `Ring-Star`) using `TopologyBuilder`.
2. **Port Binding**: We ensure that ports in each topology are properly bound and display their information.

**Desired Output**:
1. **Topology Test**: Displays router details including router ID, IP address, and bounded ports for each topology.
   
#### **Output**
![Topology Test](../imgs/TopologyTest_P2.png)

---

### **Testing Config Reader**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QString>
#include "Network/Network.h"
#include "Utils/ConfigReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString projectDir = QString(PROJECT_DIR_PATH);
    QString configFilePath = QDir(projectDir).filePath("assets/config.json");

    ConfigReader::readNetworkConfig(configFilePath);

    // SimulationConfig
    qDebug() << "SimulationConfig:";
    qDebug() << "Duration (ms):" << Network::simulationConfig.simulationDurationMs;
    qDebug() << "Cycle Duration (ms):" << Network::simulationConfig.cycleDurationMs;
    qDebug() << "TTL:" << Network::simulationConfig.TTL;
    qDebug() << "Packets per Simulation:" << Network::simulationConfig.packetsPerSimulation;
    qDebug() << "Statistical Distribution:" << Network::simulationConfig.statisticalDistribution;
    qDebug() << "Router Buffer Size:" << Network::simulationConfig.routerBufferSize;
    qDebug() << "Router Port Count:" << Network::simulationConfig.router

PortCount;
    qDebug() << "Routing Protocol:" << Network::simulationConfig.routingProtocol;
    qDebug() << "Routing Table Update Interval:"
             << Network::simulationConfig.routingTableUpdateInterval;
    qDebug() << "Routing Per Port:" << Network::simulationConfig.routingPerPort;
    qDebug() << "Routing Table Size:" << Network::simulationConfig.routingTableSize;
    qDebug() << "Routing Packets per Port Cycle:"
             << Network::simulationConfig.routingPacketsPerPortCycle;

    // Autonomous Systems
    qDebug() << "\nAutonomous Systems:";
    for (const auto *asSystem : Network::autonomousSystems) {
        qDebug() << "AS ID:" << asSystem->id;
        QString topologyTypeStr;
        switch (asSystem->topologyType) {
        case UT::TopologyType::Mesh:
            topologyTypeStr = "Mesh";
            break;
        case UT::TopologyType::RingStar:
            topologyTypeStr = "RingStar";
            break;
        case UT::TopologyType::Torus:
            topologyTypeStr = "Torus";
            break;
        }

        qDebug() << "Topology Type:" << topologyTypeStr;
        qDebug() << "Node Count:" << asSystem->nodeCount;
        qDebug() << "AS Gateways Count:" << asSystem->asGateways.size();
        qDebug() << "User Gateways Count:" << asSystem->userGateways.size();
        qDebug() << "DHCP Server Set:" << (asSystem->dhcpServer ? "Yes" : "No");
        qDebug() << "Broken Routers Count:" << asSystem->brokenRouters.size();
        qDebug() << "Connections Count:" << asSystem->connections.size();
    }

    return 0;
}
```

**What is being tested**:
1. **Configuration Reading**: We test the `ConfigReader` class to ensure it can read configuration values from a JSON file.
2. **Simulation Config**: We display simulation parameters like duration, cycle time, TTL, etc.
3. **Autonomous System Details**: The autonomous system data (topology, gateway count, router info) is displayed.

**Desired Output**:
1. **SimulationConfig**: Displays configuration parameters.
2. **Autonomous Systems**: Displays details of the autonomous systems as configured in the `config.json`.

#### **Output**
![Config Test](../imgs/ConfigTest_P2.png)

