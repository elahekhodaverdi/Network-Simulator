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
