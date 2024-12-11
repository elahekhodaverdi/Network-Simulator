#ifndef TOPOLOGYBUILDER_H
#define TOPOLOGYBUILDER_H

#include <QList>
#include "../Globals/Globals.h"
#include "../Router/Router.h"
class TopologyBuilder
{
public:
    TopologyBuilder();
    static QList<Router*> buildTopology(int nodeNumber, UT::TopologyType topologyType);

private:
    static int routersNum;
    static void buildMeshTopology(QList<Router*>& routers);
    static void buildRingStarTopology(QList<Router*>& routers);
};

#endif // TOPOLOGYBUILDER_H
