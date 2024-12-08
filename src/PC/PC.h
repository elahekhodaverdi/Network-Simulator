#ifndef PC_H
#define PC_H

#include "../Node/Node.h"
#include <QVector>
#include <QSharedPointer>

class PC : public Node
{
    Q_OBJECT

public:
    explicit PC(int id, QObject *parent = nullptr);

public Q_SLOTS:
    void sendPacket(QVector<QSharedPointer<PC>> selectedPCs);

protected:
    int m_id;
};

#endif    // PC_H
