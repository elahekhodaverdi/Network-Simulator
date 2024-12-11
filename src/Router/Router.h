#ifndef ROUTER_H
#define ROUTER_H

#include "../Node/Node.h"

class Router : public Node
{
    Q_OBJECT

public:
    explicit Router(int nodeId, QObject *parent = nullptr);

private:
    int Id;
};

#endif    // ROUTER_H
