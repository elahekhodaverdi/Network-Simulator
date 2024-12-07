#include "EventsCoordinator.h"
#include <QTimer>
#include <QEventLoop>
#include <iostream>

EventsCoordinator::EventsCoordinator(QThread *parent)
    : QThread {parent}, m_dataGenerator {new DataGenerator(1)}
{}

EventsCoordinator::~EventsCoordinator()
{
    delete m_dataGenerator;
}

EventsCoordinator *
EventsCoordinator::instance(QThread *parent)
{
    if (!m_self)
    {
        m_self = new EventsCoordinator(parent);
    }

    return m_self;
}

void
EventsCoordinator::release()
{
    if (m_self)
    {
        delete m_self;
        m_self = nullptr;
    }
}

void
EventsCoordinator::startSimulation(int intervalMs, int durationMs)
{
    m_intervalMs = intervalMs;
    m_durationMs = durationMs;
    m_running    = true;

    if (!isRunning())
        start();
}

void
EventsCoordinator::stopSimulation()
{
    m_running = false;
    quit();
    wait();
}

void
EventsCoordinator::run()
{
    int elapsedTime = 0;

    QEventLoop loop;
    QTimer timer;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    while (m_running && elapsedTime < m_durationMs)
    {
        // Generate data
        int dataCount = m_dataGenerator->generateParetoPacketCounts(1);
        Q_EMIT nextTick(dataCount);

        // Wait for the next tick
        timer.start(m_intervalMs);
        loop.exec();

        elapsedTime += m_intervalMs;
    }

    m_running = false;
}
