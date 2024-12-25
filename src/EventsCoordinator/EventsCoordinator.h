#ifndef EVENTSCOORDINATOR_H
#define EVENTSCOORDINATOR_H

#include <QObject>
#include <QSharedPointer>
#include <QThread>
#include <QTimer>
#include "../DataGenerator/DataGenerator.h"
#include "../PC/PC.h"
#include <chrono>
#include <vector>

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
    void setIntervalMs(int intervalMs);
    void setDurationMs(int durationMs);
    void setPcs(int pcs);


Q_SIGNALS:
    void nextTick(const QVector<PCPtr_t> &selectedPCs);
    void executionIsDone();

public Q_SLOTS:
    void changePhase(UT::Phase nextPhase);

private Q_SLOTS:
    void onTimerTick();

private:
    inline static EventsCoordinator *m_self = nullptr;
    bool                             m_running {false};
    int                              m_intervalMs;
    int                              m_durationMs;
    std::vector<int>                 m_dataArray;
    QVector<PCPtr_t>                 m_pcs;
    DataGenerator                   *m_dataGenerator {nullptr};
    int                              m_currentCycle{0};
    QSharedPointer<QTimer>           m_timer{nullptr};
    UT::Phase                        m_currentPhase{UT::Phase::Idle};

    void                             run() override;
    void                             startSimulation();
    void                             stopSimulation();
};

#endif
