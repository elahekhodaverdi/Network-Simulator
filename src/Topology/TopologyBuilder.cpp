#include "TopologyBuilder.h"
#include "../MACAddress/MACAddressGenerator.h"
int TopologyBuilder::routersNum = 1;

QList<RouterPtr_t> TopologyBuilder::buildTopology(int nodeNumber,
                                                  UT::TopologyType topologyType,
                                                  PortBindingManager& portBinderManager)
{
    QList<RouterPtr_t> routers;
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

void TopologyBuilder::buildMeshTopology(QList<RouterPtr_t>& routers,
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

            // Right - Left
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

            // Up - Down
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

void TopologyBuilder::buildTorusTopology(QList<RouterPtr_t>& routers,
                                         PortBindingManager& portBinderManager)
{
    buildMeshTopology(routers, portBinderManager);

    int n = std::sqrt(routers.size());
    for (int row = 0; row < n; ++row) {
        int current_index = getRouterIndexAtMesh(row, 0, n);
        int endRowIndex = getRouterIndexAtMesh(row, n - 1, n);
        PortPtr_t port1 = PortPtr_t::create();
        PortPtr_t port2 = PortPtr_t::create();
        portBinderManager.bind(port1, port2);
        routers[current_index]->addPort(port1);
        routers[endRowIndex]->addPort(port2);
    }

    for (int col = 0; col < n; ++col) {
        int current_index = getRouterIndexAtMesh(0, col, n);
        int endColIndex = getRouterIndexAtMesh(n - 1, col, n);
        PortPtr_t port1 = PortPtr_t::create();
        PortPtr_t port2 = PortPtr_t::create();
        portBinderManager.bind(port1, port2);
        routers[current_index]->addPort(port1);
        routers[endColIndex]->addPort(port2);
    }
}

void TopologyBuilder::buildRingStarTopology(QList<RouterPtr_t>& routers,
                                            PortBindingManager& portBindingManager)
{
    int n = (routers.size() - 1) / 2;
    if (n < 1 || (2 * n + 1) != routers.size()) {
        qWarning() << "Invalid number of routers for Ring-Star topology. Requires 2n+1 routers.";
        return;
    }

    RouterPtr_t centralRouter = routers.last();

    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n;

        if (routers[i]->remainingPorts() > 0 && routers[next]->remainingPorts() > 0) {
            PortPtr_t port1 = PortPtr_t::create();
            PortPtr_t port2 = PortPtr_t::create();
            portBindingManager.bind(port1, port2);
            routers[i]->addPort(port1);
            routers[next]->addPort(port2);
        }
    }

    for (int i = 0; i < n; i += 2) {
        if (routers[i]->remainingPorts() > 0 && centralRouter->remainingPorts() > 0) {
            PortPtr_t port1 = PortPtr_t::create();
            PortPtr_t port2 = PortPtr_t::create();
            portBindingManager.bind(port1, port2);
            routers[i]->addPort(port1);
            centralRouter->addPort(port2);
        }
    }
}

int TopologyBuilder::getRouterIndexAtMesh(int row, int col, int n)
{
    return row * n + col;
}
