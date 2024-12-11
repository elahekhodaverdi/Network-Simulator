#include "TopologyBuilder.h"

int TopologyBuilder::routersNum = 1;

QList<Router*> TopologyBuilder::buildTopology(int nodeNumber, UT::TopologyType topologyType)
{
    QList<Router*> routers;
    for (int i = 0; i < nodeNumber; i++)
        routers.append(new Router(routersNum++));

    switch (topologyType) {
    case UT::TopologyType::Mesh:
        buildMeshTopology(routers);
        break;
    case UT::TopologyType::RingStar:
        buildRingStarTopology(routers);
        break;
    case UT::TopologyType::Torus:
        break;
    default:
        break;
    }

    return routers;
}

void TopologyBuilder::buildMeshTopology(QList<Router*>& routers) {}
void TopologyBuilder::buildRingStarTopology(QList<Router*>& routers) {}
