#ifndef NODE_H
#define NODE_H

#include <QThread>
#include <QDebug>

class Node : public QThread
{
    Q_OBJECT

public:
    explicit Node(QObject *parent = nullptr);
};

#endif    // NODE_H
