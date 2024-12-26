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
    void setPcs(QVector<PCPtr_t> pcs);


Q_SIGNALS:
    void nextTick(UT::Phase currentPhase);
    void newPacket(QVector<PCPtr_t> selectedPCs);
    void neighboursDetectionIsDone();
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
    int                              m_numExecutionCycles{0};
    QSharedPointer<QTimer>           m_timer{nullptr};
    UT::Phase                        m_currentPhase{UT::Phase::Idle};
    int ticksInCurrentPhase{0};

    void run() override;
    void                             startSimulation();
    void                             stopSimulation();
    void                             runExecutionCycle();
};

#endif
