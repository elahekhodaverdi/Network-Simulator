#include "AutonomousSystem.h"

AutonomousSystem::AutonomousSystem() {}

AutonomousSystem::AutonomousSystem(int asId, UT::TopologyType type)
    : id(asId)
    , topologyType(type)
{}
void build(QJsonObject config) {}
