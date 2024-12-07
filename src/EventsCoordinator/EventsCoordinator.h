#ifndef EVENTSCOORDINATOR_H
#define EVENTSCOORDINATOR_H

#include <QObject>
#include <QThread>
#include <vector>
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
    static void release();

    void startSimulation(int intervalMs, int durationMs, const std::vector<int> &pcs);
    void stopSimulation();

Q_SIGNALS:
    void nextTick(const std::vector<int> &selectedPCs);

private:
    void run() override;

private:
    inline static EventsCoordinator *m_self = nullptr;
    bool                             m_running {false};
    int                              m_intervalMs;
    int                              m_durationMs;
    std::vector<int>                 m_dataArray;
    std::vector<int>                 m_pcs;
    DataGenerator                   *m_dataGenerator {nullptr};
};

#endif
