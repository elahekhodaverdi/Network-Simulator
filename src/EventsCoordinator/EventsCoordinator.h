#ifndef EVENTSCOORDINATOR_H
#define EVENTSCOORDINATOR_H

#include <QObject>
#include <QThread>
#include <chrono>
#include "../DataGenerator/DataGenerator.h"

class DataGenerator;

class EventsCoordinator : public QThread
{
    Q_OBJECT

    typedef std::chrono::milliseconds Millis;

    explicit EventsCoordinator(QThread *parent = nullptr);

public:
    ~EventsCoordinator() override;
    static EventsCoordinator *instance(QThread *parent = nullptr);
    static void               release();

public:
    void startSimulation(int intervalMs, int durationMs);
    void stopSimulation();

Q_SIGNALS:
    void nextTick(int dataCount); // Signal to emit data count

private:
    void run() override;

private:
    inline static EventsCoordinator *m_self = nullptr;
    bool                             m_running {false};
    int                              m_intervalMs;
    int                              m_durationMs;
    DataGenerator                   *m_dataGenerator {nullptr};
};

#endif
