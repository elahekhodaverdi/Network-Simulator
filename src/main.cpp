#include <QCoreApplication>
#include <QDebug>
#include "Network/Network.h"



int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Network network;
    network.run();

    return a.exec();
}
