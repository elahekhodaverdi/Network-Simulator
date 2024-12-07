#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>

class DataGenerator : public QObject
{
    Q_OBJECT
public:
    explicit DataGenerator(QObject *parent = nullptr);
    explicit DataGenerator(int minValue, QObject*parent = nullptr);
    int generateParetoPacketCounts(double alpha);
Q_SIGNALS:

private:
    int m_minValue = 1;
};

#endif // DATAGENERATOR_H
