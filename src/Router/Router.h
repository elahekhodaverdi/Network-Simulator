#ifndef ROUTER_H
#define ROUTER_H

#include "../Node/Node.h"

class Router : public Node
{
public:
    explicit Router(QObject *parent = nullptr);
};

#endif    // ROUTER_H
