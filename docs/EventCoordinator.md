### Event Coordinator System  

The **`EventsCoordinator`** class is the backbone of this project, managing the simulation of events and distributing data across PCs at defined intervals.  

---

#### **Header File Explanation**  

The header file defines the structure of the `EventsCoordinator` class:  

- **Key Variables**:  
  - `m_self`: A static instance pointer to ensure this class follows the **Singleton Pattern**, allowing only one instance of the class.  
  - `m_running`: A `bool` to track whether the simulation is active.  
  - `m_intervalMs`: The time (in milliseconds) between events.  
  - `m_durationMs`: The total duration (in milliseconds) for which the simulation will run.  
  - `m_dataArray`: A vector storing the number of data packets to send during each interval.  
  - `m_pcs`: A vector of `PC` objects that receive data.  
  - `m_dataGenerator`: An instance of the `DataGenerator` class, used to generate packet counts.  

- **Key Functions**:  
  - `startSimulation`: Prepares and begins the simulation.  
  - `stopSimulation`: Stops the simulation gracefully.  
  - `run`: The main function executed by the thread to manage events.  
  - `instance` and `release`: Manage the lifecycle of the singleton instance.  

---

#### **Source File Explanation**  

##### **Constructor (`EventsCoordinator`)**  

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

---

##### **Destructor**  

```cpp
EventsCoordinator::~EventsCoordinator()
{
    delete m_dataGenerator;
}
```

- Ensures proper cleanup by deleting the `DataGenerator` instance.  

---

##### **Singleton Management**  

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
      qDebug() << "done";
  }
  ```
  - Deletes the singleton instance and sets the pointer to `nullptr`.  

---

##### **`startSimulation` Function**  

This is where the simulation setup happens:  

```cpp
void EventsCoordinator::startSimulation(int intervalMs, int durationMs, const QVector<QSharedPointer<PC>> &pcs)
{
    m_intervalMs = intervalMs;
    m_durationMs = durationMs;
    m_pcs = pcs;
    m_dataArray.assign(durationMs / intervalMs, 0);
}
```

1. **Initialize Parameters**:  
   - `m_intervalMs` and `m_durationMs` store the timing configurations.  
   - `m_pcs` is a list of PCs that will receive data.  
   - `m_dataArray` is resized to hold the number of intervals and initialized to zero.  

2. **Data Distribution Logic**:  
   ```cpp
   size_t TOTAL_PACKETS = m_dataArray.size();
   for (size_t i = 0; i < TOTAL_PACKETS; ++i) {
       while (true) {
           size_t generatedData = m_dataGenerator->generateParetoPacketCounts(1) - 1;
           if (generatedData >= m_dataArray.size() || m_dataArray[generatedData] >= m_pcs.size()) {
               continue;
           }
           m_dataArray[generatedData] += 1;
           break;
       }
   }
   ```
   - `TOTAL_PACKETS` determines the total number of time intervals.  
   - For each interval, it generates a random data packet count using Pareto distribution.  
   - **Conditions**:  
     - `generatedData >= m_dataArray.size()`: Ensures the index is within bounds.  
     - `m_dataArray[generatedData] >= m_pcs.size()`: Ensures no single PC is overloaded.  

3. **Debugging Output(For Testing)**:  
   ```cpp
   QString res = "";
   for (size_t i = 0; i < m_dataArray.size(); ++i) {
       res += ":" + QString::number(m_dataArray[i]);
   }
   qDebug() << "Number of Packets in each Cycle:" <<  res;
   ```
   - Displays the data distribution for test and debugging.  

4. Starts the thread by setting `m_running` to `true` and calling `start()`.  

---

##### **`stopSimulation` Function**  

```cpp
void EventsCoordinator::stopSimulation()
{
    m_running = false;
    wait();
}
```

- Stops the simulation and ensures the thread completes its work before exiting.  

---

##### **`run` Function**  

The `run` function is the heart of the simulation:  

```cpp
void EventsCoordinator::run()
{
    int cycleCount = m_durationMs / m_intervalMs;
    for (int i = 0; i < cycleCount && m_running; ++i) {
        QVector<QSharedPointer<PC>> selectedPCs;
        if (m_dataArray[i] > 0) {
            std::vector<int> indices(m_pcs.size());
            std::iota(indices.begin(), indices.end(), 0);
            std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});
            for (int j = 0; j < m_dataArray[i]; ++j) {
                selectedPCs.push_back(m_pcs[indices[j]]);
            }
        }
        Q_EMIT nextTick(selectedPCs);
        QThread::msleep(m_intervalMs);
    }
}
```

1. **Calculate Cycles**:  
   - `cycleCount` determines how many intervals will run during the simulation.  

2. **Select PCs**:  
   - If `m_dataArray[i] > 0`, it means there is data to distribute in this interval.  
   - Shuffles the indices of PCs to randomize selection.  
   - Picks PCs based on the data count for that interval.  

3. **Emit Signal**:  
   - Emits the `nextTick` signal with the selected PCs for that interval.  

4. **Delay**:  
   - Waits for `m_intervalMs` milliseconds before moving to the next interval.  

---

#### **Code Flow Summary**  

1. The `startSimulation` function prepares the simulation by initializing variables and distributing data.  
2. The thread begins running the `run` function.  
3. At each interval, it selects PCs and sends data to them using signals.  
4. The simulation continues until the total duration is completed or `stopSimulation` is called.  


