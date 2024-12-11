#ifndef AUTONOMOUSSYSTEM_H
#define AUTONOMOUSSYSTEM_H

#include <QList>
#include <QString>

class AutonomousSystem
{
public:
    int id;
    QString topologyType;
    int nodeCount;
    QList<int> asGateways;
    QList<int> userGateways;
    int dhcpServer;
    QList<int> brokenRouters;
    QList<QPair<int, QList<QPair<int, int>>>> connections;
    AutonomousSystem();
    AutonomousSystem(int asId,
                     const QString &topoType,
                     int nodes,
                     const QList<int> &asGates,
                     const QList<int> &userGates,
                     int dhcp,
                     const QList<int> &broken,
                     const QList<QPair<int, QList<QPair<int, int>>>> &connects);
};

#endif // AUTONOMOUSSYSTEM_H
