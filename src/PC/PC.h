#ifndef PC_H
#define PC_H

#include "../Node/Node.h"

class PC : public Node
{

public:
    explicit PC(int id, QObject *parent = nullptr);

public Q_SLOTS:
    void sendPacket(QVector<QSharedPointer<PC>> selectedPCs);

protected:
    int m_id;
};

#endif    // PC_H
