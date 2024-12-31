## **Event Coordinator System**  

The **`EventsCoordinator`** class is the backbone of this project, managing the simulation of events and distributing data across PCs at defined intervals.  

### **Header File Explanation**  

The header file defines the structure of the `EventsCoordinator` class:  

- **Key Variables**:  
  - `m_self`: A static instance pointer to ensure this class follows the **Singleton Pattern**, allowing only one instance of the class.  
  - `m_running`: A `bool` to track whether the simulation is active.  
  - `m_intervalMs`: The time (in milliseconds) between events.  
  - `m_durationMs`: The total duration (in milliseconds) for which the simulation will run.  
  - `m_dataArray`: A vector storing the number of data packets to send during each interval.  
  - `m_pcs`: A `QVector` of `PC` shared pointers that represent the PCs receiving data.  
  - `m_dataGenerator`: An instance of the `DataGenerator` class, used to generate packet counts.  
  - `m_numExecutionCycles`: An `int` to count the number of execution cycles completed.  
  - `m_timer`: A `QSharedPointer` to a `QTimer` used for periodic execution.  
  - `m_currentPhase`: The current operational phase of the simulation, represented as `UT::Phase`.  
  - `ticksInCurrentPhase`: Tracks the number of ticks within the current phase.  

- **Key Functions**:  
  - `startSimulation`: Prepares and begins the simulation.  
  - `stopSimulation`: Stops the simulation gracefully.  
  - `run`: The main function executed by the thread to manage events.  
  - `runExecutionCycle`: Handles a single execution cycle of the simulation.  
  - `onTimerTick`: Processes timer events and manages phase-based logic.  
  - `changePhase`: Changes the operational phase of the system.  
  - `phaseToString`: Converts `UT::Phase` values to their string representations.  
  - `instance` and `release`: Manage the lifecycle of the singleton instance.  
  - Setter functions: `setIntervalMs`, `setDurationMs`, and `setPcs` allow configuration of simulation parameters.  


### **Source File Explanation**  

#### **Constructor (`EventsCoordinator`)**  

```cpp
EventsCoordinator::EventsCoordinator(QThread *parent)
    : QThread(parent), m_dataGenerator(new DataGenerator())
{
    std::fill(m_dataArray.begin(), m_dataArray.end(), 0);
}
```

- Initializes the `EventsCoordinator` object.  
- Allocates memory for `m_dataGenerator`.  
- Fills `m_dataArray` with zeros initially.  

#### **Destructor**  

```cpp
EventsCoordinator::~EventsCoordinator()
{
    delete m_dataGenerator;
}
```

- Ensures proper cleanup by deleting the `DataGenerator` instance.  

#### **Singleton Management**  

- **`instance` Function**:  
  ```cpp
  EventsCoordinator *EventsCoordinator::instance(QThread *parent)
  {
      if (!m_self) {
          m_self = new EventsCoordinator(parent);
      }
      return m_self;
  }
  ```
  - Creates or retrieves the single instance of the class.  

- **`release` Function**:  
  ```cpp
  void EventsCoordinator::release()
  {
      delete m_self;
      m_self = nullptr;
  }
  ```
  - Deletes the singleton instance and sets the pointer to `nullptr`.  


#### **`startSimulation` Function**  

This is where the simulation setup happens:  

```cpp
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
    int totalPackets = 0;
    QString res = "";
    for (size_t i = 0; i < m_dataArray.size(); ++i) {
        res += ":" + QString::number(m_dataArray[i]);
        totalPackets += m_dataArray[i];
    }
    qDebug() << totalPackets << "Number of Packets in each Cycle: " << res;

    m_running = true;
    start();
}
```

1. **Initialize Parameters**:  
   - Prepares `m_dataArray` to store packet counts for each interval.  
   - Uses a Pareto distribution to generate packet counts while avoiding overload.  

2. **Debugging Output**:  
   - Outputs the packet distribution for testing purposes.  

3. **Starts the thread**:  
   - Sets `m_running` to `true` and begins thread execution.  


#### **`stopSimulation` Function**  

```cpp
void EventsCoordinator::stopSimulation()
{
    m_running = false;
    if (m_timer) {
        m_timer->stop();
    }
    wait();
    Q_EMIT executionIsDone();
}
```

- Stops the simulation, halts the timer, and signals completion.  


#### **`run` Function**  

```cpp
void EventsCoordinator::run()
{
    m_timer = QSharedPointer<QTimer>::create();
    connect(m_timer.get(), &QTimer::timeout, this, &EventsCoordinator::onTimerTick);

    m_timer->start(m_intervalMs);

    exec();
}
```

- Creates and starts a timer to handle periodic execution.  
- Enters an event loop to process timer ticks.  


#### **`runExecutionCycle` Function**  

```cpp
void EventsCoordinator::runExecutionCycle()
{
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
```

- Executes a single cycle by selecting PCs to receive packets and emitting a signal.  


#### **`onTimerTick` Function**  

```cpp
void EventsCoordinator::onTimerTick()
{
    ticksInCurrentPhase++;
    switch (m_currentPhase) {
    case UT::Phase::IdentifyNeighbours:
        if (ticksInCurrentPhase >= 2) {
            Q_EMIT neighboursDetectionIsDone();
        }
        break;
    case UT::Phase::Execution:
        runExecutionCycle();
    default:
        break;
    }

    Q_EMIT nextTick(m_currentPhase);
}
```

- Handles periodic tasks based on the current phase.  


### **Code Flow Summary**  

1. The `startSimulation` function prepares the simulation by initializing variables and distributing data.  
2. The thread begins running the `run` function, which starts the timer.  
3. The `onTimerTick` function processes timer events and triggers phase-specific logic.  
4. Data packets are distributed during execution cycles, and results are sent using signals.  
5. The simulation continues until the duration is completed or `stopSimulation` is called.  
