#include "PC.h"

PC::PC(int id, QObject *parent) :
    Node {parent}
{
    m_id = id;
}

void PC::sendPacket(QVector<QSharedPointer<PC>> selectedPCs){
    if (!selectedPCs.contains(this))
        return;
    qDebug() << "sending packet from " << m_id;
}
