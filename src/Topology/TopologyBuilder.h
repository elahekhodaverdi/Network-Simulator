#ifndef TOPOLOGYBUILDER_H
#define TOPOLOGYBUILDER_H

#include <QList>
#include "../Globals/Globals.h"
#include "../Router/Router.h"
class TopologyBuilder
{
public:
    TopologyBuilder();
    static QList<RouterPtr_t> buildTopology(int nodeNumber, UT::TopologyType topologyType);

private:
    static int routersNum;
    static void buildMeshTopology(QList<RouterPtr_t>& routers);
    static void buildRingStarTopology(QList<RouterPtr_t>& routers);
    static void buildTorusTopology(QList<RouterPtr_t>& routers);
    static int getRouterIndexAtMesh(int row, int col, int n);
};

#endif // TOPOLOGYBUILDER_H
