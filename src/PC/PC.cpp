#include "PC.h"

PC::PC(int id, MACAddress macAddress, QObject *parent)
    : Node(id, macAddress, parent)
{
}

void PC::sendPacket(QVector<QSharedPointer<PC>> selectedPCs) {
    if (!selectedPCs.contains(this))
        return;
    qDebug() << "sending packet from " << m_id;
}
