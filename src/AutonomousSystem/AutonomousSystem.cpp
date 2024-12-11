#include "AutonomousSystem.h"
#include "../Topology/TopologyBuilder.h"
AutonomousSystem::AutonomousSystem() {}

AutonomousSystem::AutonomousSystem(int asId, UT::TopologyType type)
    : id(asId)
    , topologyType(type)
{}
void AutonomousSystem::build(QJsonObject config)
{
    nodeCount = config["node_count"].toInt();
    routers = TopologyBuilder::buildTopology(nodeCount, topologyType);
}
