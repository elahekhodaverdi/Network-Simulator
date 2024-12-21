#include "RoutingProtocol.h"

RoutingProtocol::RoutingProtocol(QObject *parent)
    : QObject{parent}
{}


void RoutingProtocol::updateRoutingTable(RoutingTableEntry newEntry)
{
    auto it = std::find_if(routingTable.begin(),
                           routingTable.end(),
                           [&newEntry](const RoutingTableEntry &entry) {
                               return entry.destination == newEntry.destination;
                           });

    if (it == routingTable.end()) {
        routingTable.append(newEntry);
    } else if (it->metric > newEntry.metric) {
        *it = newEntry;
    }
}

void RoutingProtocol::printRoutingTable() const
{
    QTextStream out(stdout);

    out << "---------------------------------------------------------------\n";
    out << QString("%1\t%2\t%3\t%4\t%5\n")
             .arg("Prtcol", -6)
             .arg("Destination IP", -20)
             .arg("Next Hop IP", -20)
             .arg("Out Port", -10)
             .arg("Metric", -6);
    out << "---------------------------------------------------------------\n";

    for (const RoutingTableEntry &entry : routingTable) {
        out << QString("%1\t%2\t%3\t%4\t%5\n")
        .arg((entry.protocol == UT::RoutingProtocol::OSPF ? "O" : "R"), -6)
          .arg(entry.destination->toString() + "\\" + entry.subnetMask->toString(),
               -20)
          .arg(entry.nextHop->toString(), -20)
          .arg((entry.outPort ? QString::number(entry.outPort->getPortNumber()) : "N/A"),
               -10)
          .arg(entry.metric, -6);
    }

    out << "---------------------------------------------------------------\n";
}


PortPtr_t RoutingProtocol::findOutPort(IPv4Ptr_t destIP) {
    for (const RoutingTableEntry &entry : routingTable) {
        if (*entry.destination == *destIP) {
            return entry.outPort;
        }
    }

    //qDebug() << "No route found for destination IP: " << destIP->toString();
    return nullptr;
}
