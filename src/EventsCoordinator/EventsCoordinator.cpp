#include "EventsCoordinator.h"
#include <random>
#include <algorithm>
#include <QThread>
#include <QDebug>

EventsCoordinator::EventsCoordinator(QThread *parent)
    : QThread(parent), m_dataGenerator(new DataGenerator())
{
    std::fill(m_dataArray.begin(), m_dataArray.end(), 0);
}

EventsCoordinator::~EventsCoordinator()
{
    delete m_dataGenerator;
}

EventsCoordinator *EventsCoordinator::instance(QThread *parent)
{
    if (!m_self) {
        m_self = new EventsCoordinator(parent);
    }
    return m_self;
}

void EventsCoordinator::release()
{
    delete m_self;
    m_self = nullptr;
    qDebug() << "done";
}

void EventsCoordinator::startSimulation()
{
    m_dataArray.assign(m_durationMs / m_intervalMs, 0);
    size_t TOTAL_PACKETS = m_dataArray.size();
    std::fill(m_dataArray.begin(), m_dataArray.end(), 0);

    for (size_t i = 0; i < TOTAL_PACKETS; ++i) {
        while (true){
            size_t generatedData = m_dataGenerator->generateParetoPacketCounts(1) - 1;
            if (generatedData >= m_dataArray.size() || m_dataArray[generatedData] >= m_pcs.size()){
                continue;
            }
            m_dataArray[generatedData] += 1;
            break;
        }
    }
    QString res = "";
    for (size_t i = 0; i < m_dataArray.size(); ++i) {
        res += ":" + QString::number(m_dataArray[i]);
    }
    qDebug() << "Number of Packets in each Cycle: " << res;

    m_running = true;
    start();
}

void EventsCoordinator::stopSimulation()
{
    m_running = false;
    if (m_timer) {
        m_timer->stop();
    }
    wait();
    Q_EMIT executionIsDone();
}

void EventsCoordinator::run()
{
    m_timer = QSharedPointer<QTimer>::create();
    connect(m_timer.get(), &QTimer::timeout, this, &EventsCoordinator::onTimerTick);

    m_timer->start(m_intervalMs);

    exec();
}

void EventsCoordinator::runExecutionCycle(){
    if (!m_running || m_numExecutionCycles >= (m_durationMs / m_intervalMs)) {
        Q_EMIT executionIsDone();
        return;
    }
    QVector<PCPtr_t> selectedPCs;
    if (m_dataArray[m_numExecutionCycles] > 0) {
        std::vector<int> indices(m_pcs.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});
        for (int j = 0; j < m_dataArray[m_numExecutionCycles]; ++j) {
            selectedPCs.push_back(m_pcs[indices[j]]);
        }
    }
    Q_EMIT newPacket(selectedPCs);
    ++m_numExecutionCycles;
}

void EventsCoordinator::onTimerTick()
{
    ticksInCurrentPhase++;
    switch (m_currentPhase) {
    case UT::Phase::IdentifyNeighbors:
        if (ticksInCurrentPhase >= 2)
            Q_EMIT neighboursDetectionIsDone();
        break;
    case UT::Phase::Execution:
        runExecutionCycle();
    default:
        break;
    }

    Q_EMIT nextTick(m_currentPhase);
}

void EventsCoordinator::changePhase(UT::Phase nextPhase){
    m_currentPhase = nextPhase;
    ticksInCurrentPhase = 0;
    if (m_currentPhase == UT::Phase::Start)
        startSimulation();
}

void EventsCoordinator::setDurationMs(int durationMs){
    m_durationMs = durationMs;
}

void EventsCoordinator::setIntervalMs(int intervalMs){
    m_intervalMs = intervalMs;
}

void EventsCoordinator::setPcs(QVector<PCPtr_t> pcs){
    m_pcs = pcs;
}
