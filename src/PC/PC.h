#ifndef PC_H
#define PC_H

#include <QSharedPointer>
#include <QVector>
#include "../Node/Node.h"
#include "../Port/Port.h"

class PC : public Node
{
    Q_OBJECT

public:
    explicit PC(int id, MACAddress macAddress, QObject *parent = nullptr);
    explicit PC(int id, QObject *parent = nullptr);

public Q_SLOTS:
    void sendPacket(QVector<QSharedPointer<PC>> selectedPCs);
    void setIP(IPv4_t ip);

private:
    PortPtr_t gateaway;
};

typedef QSharedPointer<PC> PCPtr_t;

#endif // PC_H
