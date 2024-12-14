#include <QCoreApplication>
#include <QDebug>
#include "../PortBindingManager/PortBindingManager.h"
#include "../Topology/TopologyBuilder.h"

void printBoundedPorts(QList<PortPtr_t> ports)
{
    qDebug() << "--------------------------------------";

    for (const auto &port : ports) {
        if (!PortBindingManager::isBounded(port))
            break;
        qDebug() << "Port with ID" << port->getPortNumber();
        PortBindingManager::printBindingsForaPort(port);
    }
    qDebug() << "--------------------------------------\n";
}
void testMeshTopology()
{
    qDebug() << "Testing Mesh Topology...";

    int numberOfNodes = 9;
    uint16_t asID = 1;

    QList<RouterPtr_t> routers = TopologyBuilder::buildTopology(numberOfNodes,
                                                                UT::TopologyType::Mesh,
                                                                asID);

    for (int i = 0; i < routers.size(); ++i) {
        RouterPtr_t router = routers.at(i);
        qDebug() << "\nRouter" << router->getId() << "with IP" << router->getIP()->toString();

        printBoundedPorts(router->getPorts());
    }
    qDebug() << "\nMesh Topology Test Complete!";
}

void testTorusTopology()
{
    qDebug() << "Testing Torus Topology...";

    int numberOfNodes = 9;
    uint16_t asID = 2;

    auto routers = TopologyBuilder::buildTopology(numberOfNodes, UT::TopologyType::Torus, asID);

    for (int i = 0; i < routers.size(); ++i) {
        RouterPtr_t router = routers.at(i);
        qDebug() << "\nRouter" << router->getId() << "with IP" << router->getIP()->toString();

        printBoundedPorts(router->getPorts());
    }
    qDebug() << "\nTorus Topology Test Complete!";
}

void testRingStarTopology()
{
    qDebug() << "Testing Ring-Star Topology...";

    int numberOfNodes = 7;
    uint16_t asID = 2;

    auto routers = TopologyBuilder::buildTopology(numberOfNodes, UT::TopologyType::RingStar, asID);

    for (int i = 0; i < routers.size(); ++i) {
        RouterPtr_t router = routers.at(i);
        qDebug() << "\nRouter" << router->getId() << "with IP" << router->getIP()->toString();

        printBoundedPorts(router->getPorts());
    }
    qDebug() << "\nRing-Star Topology Test Complete!";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Test each topology
    testMeshTopology();
    testTorusTopology();
    testRingStarTopology();
    return a.exec();
}
