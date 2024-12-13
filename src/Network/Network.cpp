#include "Network.h"

AutonomousSystem *Network::findASById(int id)
{
    AutonomousSystem *target = nullptr;
    for (AutonomousSystem *as : autonomousSystems) {
        if (as->id == id) {
            target = as;
            break;
        }
    }
    return target;
}
