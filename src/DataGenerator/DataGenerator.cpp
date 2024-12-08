#include "DataGenerator.h"
#include <random>
#include <cmath> // For std::pow and std::ceil

DataGenerator::DataGenerator(QObject *parent)
    : QObject(parent)
{}

DataGenerator::DataGenerator(int minValue, QObject *parent)
    : QObject(parent)
    , m_minValue(minValue)
{}

int DataGenerator::generateParetoPacketCounts(double alpha)
{
    QVector<int> packetCounts;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> uniformDist(0.0, 1.0);
    double u = uniformDist(gen);
    double paretoValue = m_minValue / std::pow(1.0 - u, 1.0 / alpha);
    return paretoValue;
}
