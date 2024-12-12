#include "TopologyBuilder.h"
#include "../MACAddress/MACAddressGenerator.h"
int TopologyBuilder::routersNum = 1;

QList<Router*> TopologyBuilder::buildTopology(int nodeNumber,
                                              UT::TopologyType topologyType,
                                              PortBindingManager& portBinderManager)
{
    QList<Router*> routers;
    for (int i = 0; i < nodeNumber; i++)
        routers.append(new Router(routersNum++, MACAddressGenerator::getRandomMAC()));

    switch (topologyType) {
    case UT::TopologyType::Mesh:
        buildMeshTopology(routers, portBinderManager);
        break;
    case UT::TopologyType::RingStar:
        buildRingStarTopology(routers, portBinderManager);
        break;
    case UT::TopologyType::Torus:
        break;
    default:
        break;
    }

    return routers;
}

void TopologyBuilder::buildMeshTopology(QList<Router*>& routers,
                                        PortBindingManager& portBinderManager)
{
    int n = std::sqrt(routers.size());

    if (n * n != routers.size()) {
        qWarning() << "Total routers must be a perfect square!";
        return;
    }

    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            int currentIndex = getRouterIndexAtMesh(row, col, n);

            if (routers[currentIndex]->remainingPorts() == 0)
                continue; // Skip if no ports are available

            // Left neighbor
            if (col > 0) {
                int leftIndex = getRouterIndexAtMesh(row, col - 1, n);
                if (routers[leftIndex]->remainingPorts() > 0
                    && routers[currentIndex]->remainingPorts() > 0) {
                    PortPtr_t port1 = PortPtr_t::create();
                    PortPtr_t port2 = PortPtr_t::create();
                    portBinderManager.bind(port1, port2);
                    routers[currentIndex]->addPort(port1);
                    routers[leftIndex]->addPort(port2);
                }
            }

            // Right neighbor
            if (col < n - 1) {
                int rightIndex = getRouterIndexAtMesh(row, col + 1, n);
                if (routers[rightIndex]->remainingPorts() > 0
                    && routers[currentIndex]->remainingPorts() > 0) {
                    PortPtr_t port1 = PortPtr_t::create();
                    PortPtr_t port2 = PortPtr_t::create();
                    portBinderManager.bind(port1, port2);
                    routers[currentIndex]->addPort(port1);
                    routers[rightIndex]->addPort(port2);
                }
            }

            // Upper neighbor
            if (row > 0) {
                int upIndex = getRouterIndexAtMesh(row - 1, col, n);
                if (routers[upIndex]->remainingPorts() > 0
                    && routers[currentIndex]->remainingPorts() > 0) {
                    PortPtr_t port1 = PortPtr_t::create();
                    PortPtr_t port2 = PortPtr_t::create();
                    portBinderManager.bind(port1, port2);
                    routers[currentIndex]->addPort(port1);
                    routers[upIndex]->addPort(port2);
                }
            }

            // Lower neighbor
            if (row < n - 1) {
                int downIndex = getRouterIndexAtMesh(row + 1, col, n);
                if (routers[downIndex]->remainingPorts() > 0
                    && routers[currentIndex]->remainingPorts() > 0) {
                    PortPtr_t port1 = PortPtr_t::create();
                    PortPtr_t port2 = PortPtr_t::create();
                    portBinderManager.bind(port1, port2);
                    routers[currentIndex]->addPort(port1);
                    routers[downIndex]->addPort(port2);
                }
            }
        }
    }
}

void TopologyBuilder::buildRingStarTopology(QList<Router*>& routers,
                                            PortBindingManager& portBinderManager)
{}

int TopologyBuilder::getRouterIndexAtMesh(int row, int col, int n)
{
    return row * n + col;
}
