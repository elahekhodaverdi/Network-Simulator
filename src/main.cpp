#include "../EventsCoordinator/EventsCoordinator.h"
#include <QCoreApplication>
#include <QObject>
#include <iostream>



int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    EventsCoordinator *eventCoordinator = EventsCoordinator::instance();

    QObject::connect(eventCoordinator, &EventsCoordinator::nextTick, [](int dataCount) {
        std::cout << "Data generated in this cycle: " << dataCount << std::endl;
    });

    eventCoordinator->startSimulation(200, 10000);

    int result = app.exec();

    EventsCoordinator::release();

    return result;
}

