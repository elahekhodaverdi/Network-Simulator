#include "TopologyBuilder.h"
#include "../EventsCoordinator/EventsCoordinator.h"
#include "../MACAddress/MACAddressGenerator.h"
#include "../Network/SimulationConfig.h"
#include "../PortBindingManager/PortBindingManager.h"
#include "../Simulator/Simulator.h"
int TopologyBuilder::routersNum = 1;

QList<RouterPtr_t> TopologyBuilder::buildTopology(int nodeNumber, UT::TopologyType topologyType, int asId)
{
    QList<RouterPtr_t> routers;
    for (int i = 0; i < nodeNumber; i++){
        RouterPtr_t newRouter = RouterPtr_t::create(routersNum,
                                                    MACAddressGenerator::getRandomMAC(),
                                                    Simulator::simulationConfig.routerPortCount,
                                                    Simulator::simulationConfig.routerBufferSize);

        routers.append(newRouter);
        QObject::connect(EventsCoordinator::instance(), &EventsCoordinator::nextTick, newRouter.get(), &Router::handleNewTick);

        routersNum++;
    }

    switch (topologyType) {
    case UT::TopologyType::Mesh:
        buildMeshTopology(routers);
        break;
    case UT::TopologyType::RingStar:
        buildRingStarTopology(routers);
        break;
    case UT::TopologyType::Torus:
        buildTorusTopology(routers);
        break;
    default:
        break;
    }

    return routers;
}

void TopologyBuilder::buildMeshTopology(QList<RouterPtr_t>& routers)
{
    int n = std::sqrt(routers.size());

    if (n * n != routers.size()) {
        qWarning() << "Total routers must be a perfect square!";
        return;
    }

    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            int currentIndex = getRouterIndexAtMesh(row, col, n);

            if (routers[currentIndex]->numRemainingPorts() == 0)
                continue;

            // Right - Left
            if (col > 0) {
                int leftIndex = getRouterIndexAtMesh(row, col - 1, n);
                if (routers[leftIndex]->numRemainingPorts() > 0
                   && routers[currentIndex]->numRemainingPorts() > 0) {
                    PortPtr_t port1 = routers[currentIndex]->getAnUnboundPort();
                    PortPtr_t port2 = routers[leftIndex]->getAnUnboundPort();
                    PortBindingManager::bind(port1, port2);
                }
            }

            // Up - Down
            if (row < n - 1) {
                int downIndex = getRouterIndexAtMesh(row + 1, col, n);
                if (routers[downIndex]->numRemainingPorts() > 0
                   && routers[currentIndex]->numRemainingPorts() > 0) {
                    PortPtr_t port1 = routers[currentIndex]->getAnUnboundPort();
                    PortPtr_t port2 = routers[downIndex]->getAnUnboundPort();
                    PortBindingManager::bind(port1, port2);
                }
            }
        }
    }
}

void TopologyBuilder::buildTorusTopology(QList<RouterPtr_t>& routers)
{
    buildMeshTopology(routers);

    int n = std::sqrt(routers.size());
    for (int row = 0; row < n; ++row) {
        int currentIndex = getRouterIndexAtMesh(row, 0, n);
        int endRowIndex = getRouterIndexAtMesh(row, n - 1, n);
        PortPtr_t port1 = routers[currentIndex]->getAnUnboundPort();
        PortPtr_t port2 = routers[endRowIndex]->getAnUnboundPort();
        PortBindingManager::bind(port1, port2);
    }

    for (int col = 0; col < n; ++col) {
        int currentIndex = getRouterIndexAtMesh(0, col, n);
        int endColIndex = getRouterIndexAtMesh(n - 1, col, n);
        PortPtr_t port1 = routers[currentIndex]->getAnUnboundPort();
        PortPtr_t port2 = routers[endColIndex]->getAnUnboundPort();
        PortBindingManager::bind(port1, port2);
    }
}

void TopologyBuilder::buildRingStarTopology(QList<RouterPtr_t>& routers)
{
    int n = (routers.size() - 1) / 2;
    if (n < 1 || (2 * n + 1) != routers.size()) {
        qWarning() << "Invalid number of routers for Ring-Star topology. Requires 2n+1 routers.";
        return;
    }

    RouterPtr_t centralRouter = routers.last();

    for (int i = 0; i < routers.size() - 1; ++i) {
        int next = (i + 1) % (routers.size() - 1);
        if (routers[i]->numRemainingPorts() > 0 && routers[next]->numRemainingPorts() > 0) {
            PortPtr_t port1 = routers[i]->getAnUnboundPort();
            PortPtr_t port2 = routers[next]->getAnUnboundPort();
            PortBindingManager::bind(port1, port2);
        }
    }

    for (int i = 0; i < routers.size() - 1; i += 2) {
        if (routers[i]->numRemainingPorts() > 0 && centralRouter->numRemainingPorts() > 0) {
            PortPtr_t port1 = routers[i]->getAnUnboundPort();
            PortPtr_t port2 = centralRouter->getAnUnboundPort();
            PortBindingManager::bind(port1, port2);
        }
    }
}

int TopologyBuilder::getRouterIndexAtMesh(int row, int col, int n)
{
    return row * n + col;
}

void TopologyBuilder::reset()
{
    routersNum = 1;
}
