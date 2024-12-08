#include "../EventsCoordinator/EventsCoordinator.h"
#include <QCoreApplication>
#include <QObject>
#include <iostream>



int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    EventsCoordinator *eventCoordinator = EventsCoordinator::instance();

    // QObject::connect(eventCoordinator, &EventsCoordinator::nextTick, [](int dataCount) {
    //     std::cout << "Data generated in this cycle: " << dataCount << std::endl;
    // });
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

