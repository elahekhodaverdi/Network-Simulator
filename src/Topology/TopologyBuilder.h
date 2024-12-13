#ifndef TOPOLOGYBUILDER_H
#define TOPOLOGYBUILDER_H

#include <QList>
#include "../Globals/Globals.h"
#include "../PortBindingManager/PortBindingManager.h"
#include "../Router/Router.h"
class TopologyBuilder
{
public:
    TopologyBuilder();
    static QList<Router*> buildTopology(int nodeNumber,
                                        UT::TopologyType topologyType,
                                        PortBindingManager& portBinderManager);

private:
    static int routersNum;
    static void buildMeshTopology(QList<Router*>& routers, PortBindingManager& portBinderManager);
    static void buildTorusTopology(QList<Router*>& routers, PortBindingManager& portBinderManager);
    static void buildRingStarTopology(QList<Router*>& routers,
                                      PortBindingManager& portBinderManager);

    static int getRouterIndexAtMesh(int row, int col, int n);
};

#endif // TOPOLOGYBUILDER_H
