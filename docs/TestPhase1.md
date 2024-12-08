## Phase 1 Tests

### **Testing the EventCoordinator System**

To validate the functionality of the `EventCoordinator` system, we implemented `Node` and `PC` classes. These were used to simulate PCs that receive data packets at regular intervals, as coordinated by the EventCoordinator.

#### **Node and PC Implementation**

The `Node` class serves as a base class for the `PC`, representing individual PCs in the system. Each PC can send data packets when connected to the EventCoordinator and received a signal to do so.

**Node Header File:**
```cpp
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
```

**PC Header File:**
```cpp
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
```

**PC Source File:**
```cpp
#include "PC.h"

PC::PC(int id, QObject *parent) :
    Node(parent)
{
    m_id = id;
}

void PC::sendPacket(QVector<QSharedPointer<PC>> selectedPCs) {
    if (!selectedPCs.contains(this))
        return;
    qDebug() << "sending packet from " << m_id;
}
```

#### **Testing with Main Function**

The `main` function demonstrates the simulation of the system by:

1. Creating an instance of `EventCoordinator` using its singleton pattern.
2. Initializing 10 PCs.
3. Connecting the EventCoordinator's `nextTick` signal to each PC's `sendPacket` slot.
4. Starting the simulation with an interval of 200 milliseconds and a total duration of 10,000 milliseconds.

**Main Function:**
```cpp
#include "../EventsCoordinator/EventsCoordinator.h"
#include <QCoreApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    EventsCoordinator *eventCoordinator = EventsCoordinator::instance();

    QVector<QSharedPointer<PC>> pcs;
    for (int i = 0; i < 10; i++) {
        QSharedPointer<PC> pc = QSharedPointer<PC>::create(i);
        pcs.append(pc);

        QObject::connect(eventCoordinator, &EventsCoordinator::nextTick, pc.get(), &PC::sendPacket);
    }

    eventCoordinator->startSimulation(200, 10000, pcs);
    int result = app.exec();

    EventsCoordinator::release();

    return result;
}
```

---

#### **Output**

The system's test outputs include:
1. A debug log showing the number of data packets distributed in each cycle.
2. Logs for PCs receiving packets, indicating the PC ID.

**Output:**
![EventCoordinator Test Phase 1](../imgs/EventCoordinatorTest_P1.png)

This output validates that:
- Data distribution among cycles matches the configuration.
- Each PC correctly receives packets when selected.

