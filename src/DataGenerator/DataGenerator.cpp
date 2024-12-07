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

QVector<int> DataGenerator::generateParetoPacketCounts(int numSources, double alpha)
{
    QVector<int> packetCounts;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> uniformDist(0.0, 1.0);

    for (int i = 0; i < numSources; ++i) {
        double u = uniformDist(gen);
        double paretoValue = m_minValue / std::pow(1.0 - u, 1.0 / alpha);
        packetCounts.append(static_cast<int>(std::ceil(paretoValue)));
    }

    return packetCounts;
}
