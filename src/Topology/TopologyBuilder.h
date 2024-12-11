#ifndef TOPOLOGYBUILDER_H
#define TOPOLOGYBUILDER_H

#include <QList>
#include "../Globals/Globals.h"
#include "../Router/Router.h"
class TopologyBuilder
{
public:
    TopologyBuilder();
    QList<Router*> buildTopology(int nodeNumber, UT::TopologyType topologyType);

private:
    int routersNum;
    void buildMeshTopology(QList<Router*>& routers);
    void buildRingStarTopology(QList<Router*>& routers);
};

#endif // TOPOLOGYBUILDER_H
