## Simulator Class

The Simulator class is a singleton responsible for managing the entire simulation lifecycle. It interacts with other components like `EventsCoordinator` and `Network` to coordinate events, track simulation phases, and execute the configured simulation. The class also includes methods for analyzing simulation results, resetting the simulation, and calculating various metrics such as packet loss and average hop count.

### Fields

1. **Static Fields**
   - `static Simulator *m_self`: Singleton instance of the Simulator class.
   - `static SimulationConfig simulationConfig`: Configuration object for simulation settings.

2. **Private Fields**
   - `QSharedPointer<EventsCoordinator> eventsCoordinator`: Shared pointer to the `EventsCoordinator` instance.
   - `Network network`: Represents the network for the simulation.
   - `int numOfRoutersDone`: Tracks the number of routers that have completed a phase.
   - `UT::Phase currentPhase`: Current phase of the simulation.
   - `QVector<PacketPtr_t> packetsSent`: List of sent packets during the simulation.
   - `int numOfPackets`: Total number of packets sent during the simulation.

### Signals

- **`phaseChanged(UT::Phase nextPhase)`**: Emitted when the simulation moves to a new phase.

### Slots

1. **r`outerIsDone()`**: Updates the number of completed routers for the current phase and transitions to the next phase if conditions are met.
2. **`neighboursIdentified()`**: Transitions the simulation to the `Routing` phase.
3. **`executionIsDone()`**: Transitions the simulation to the `Analysis` phase.
4. **`incNumOfPackets(int num)`**: Increments the total packet count by the specified number.
5. **`storeSentPacket(const PacketPtr_t &packet)`**: Stores a sent packet for tracking and analysis purposes.

### Methods Explanations

1. **Public Methods**

   - **`Simulator(QObject *parent = nullptr)`**: Constructor.
   - **`~Simulator()`**: Destructor.
   - **`static Simulator *instance(QObject *parent = nullptr)`**: Retrieves the singleton instance, creating it if necessary.
   - **`static void release()`**: Destroys the singleton instance.
   - **`void run()`**: Starts the simulation by transitioning to the `Start` phase.

2. **Private Methods**

   - **`void goToNextPhase(UT::Phase nextPhase)`**: Transitions the simulation to the specified phase, updating internal states accordingly.
   - **`void start()`**: Prepares the simulation by loading configurations and initializing the `EventsCoordinator`.
   - **`void analysis()`**: Provides an interactive interface for analyzing simulation results.
   - **`void reset()`**: Resets the simulation to its initial state.
   - **`void calculatePacketLoss()`**: Calculates and logs the packet loss percentage.
   - **`void calculateAverageHopCount()`**: Calculates and logs the average hop count for sent packets.
   - **`void calculateWaitingCyclesStats()`**: Calculates and logs statistics on packet waiting cycles.
   - **`void printRoutingTable(const QString &routerId)`**: Prints the routing table for a specific router.
   - **`void listUsedRouters()`**: Lists all routers that were used during the simulation.
   - **`void listPoorRouters()`**: Lists routers that were not used during the simulation.
   - **`void listTopRouters()`**: Lists the top routers based on packet deliveries.
   - **`void exitSimulation()`**: Cleans up and exits the simulation.

### Method Explanation

1. **`Simulator(QObject *parent = nullptr)`**
   - Initializes `numOfRoutersDone` to 0 and `currentPhase` to `UT::Phase::Idle`.
   - Connects the `phaseChanged` signal to the `changePhase` slot of `EventsCoordinator`.
   - Connects signals from `EventsCoordinator` to relevant slots like `executionIsDone` and `neighboursIdentified`.

2. **`run()`**
   - Invokes `goToNextPhase` to start the simulation.

3. **`goToNextPhase(UT::Phase nextPhase)`**
   - Updates `currentPhase` and resets `numOfRoutersDone`.
   - Transitions to the appropriate method or emits the `phaseChanged` signal based on the new phase.

4. **`start()`**
   - Reads simulation configuration from a JSON file.
   - Initializes the `EventsCoordinator` with simulation parameters.
   - Emits the `phaseChanged` signal and transitions to the `DHCP` phase.

5. **`routerIsDone()`**
   - Tracks completed routers and determines if the current phase is ready for a transition.
   - Resets `numOfRoutersDone` after a successful transition.

6. **`storeSentPacket(const PacketPtr_t &packet)`**
   - Appends the packet to the `packetsSent` list.

7. **`analysis()`**
   - Provides an interactive console interface for performing analysis on simulation results.
   - Recognizes commands such as:
     - `"Packet-loss"`: Calculates packet loss percentage.
     - `"Hop-count-avg"`: Calculates the average hop count.
     - `"Waiting-cycles-stat"`: Calculates waiting cycle statistics.
     - `"Print-routing-table <routerId>"`: Prints the routing table of a specific router.
     - `"Used-routers"`: Lists routers used in the simulation.
     - `"Poor-routers"`: Lists unused routers.
     - `"Top-routers"`: Lists the top routers based on usage.
     - `"Reset"`: Resets the simulation.
     - `"Exit"`: Exits the simulation.

8. **`reset()`**
   - Resets the network, clears sent packets, and restarts the simulation.

9. **`calculatePacketLoss()`**
   - Logs the packet loss percentage if packets were sent.

10. **`calculateAverageHopCount()`**
    - Logs the average hop count for packets sent during the simulation.

11. **`calculateWaitingCyclesStats()`**
    - Logs statistics (total, min, max, and average) on packet waiting cycles.

12. **`printRoutingTable(const QString &routerId)`**
    - Prints the routing table for a router specified by its ID.

13. **`listUsedRouters()`**
    - Logs all routers used during the simulation.

14. **`listPoorRouters()`**
    - Logs routers that were not used during the simulation.

15. **`listTopRouters()`**
    - Logs the top routers ranked by packet deliveries.

16. **`exitSimulation()`**
    - Cleans up resources and exits the simulation.
