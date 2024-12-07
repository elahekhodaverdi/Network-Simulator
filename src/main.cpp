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
    qDebug() << 1;
    std::vector<int> pcs = {1, 2, 3, 34, 5,};
    eventCoordinator->startSimulation(200, 10000, pcs);
    qDebug() << 2;


    //return result;
}

