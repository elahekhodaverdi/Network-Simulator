#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QDebug>

class Node : public QObject
{
    Q_OBJECT

public:
    explicit Node(QObject *parent = nullptr);

Q_SIGNALS:

};

#endif    // NODE_H
