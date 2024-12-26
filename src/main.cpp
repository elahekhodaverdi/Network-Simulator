#include <QCoreApplication>
#include <QDebug>
#include "Simulator/Simulator.h"


int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Simulator* simulator = Simulator::instance();
    simulator->run();
    return a.exec();
}
