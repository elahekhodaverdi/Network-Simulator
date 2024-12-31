## Simulator Class

The `Simulator` class is a singleton responsible for managing the entire simulation lifecycle. It interacts with other components like `EventsCoordinator` and `Network` to coordinate events, track simulation phases, and execute the configured simulation. The class also handles packets and phases during the simulation process.

### Fields

1. **Static Fields**
   - `static Simulator *m_self`: Singleton instance of the `Simulator` class.
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

1. **`routerIsDone()`**: Updates the number of completed routers for the current phase and transitions to the next phase if conditions are met.
2. **`neighboursIdentified()`**: Transitions the simulation to the `Routing` phase.
3. **`executionIsDone()`**: Transitions the simulation to the `Analysis` phase.
4. **`incNumOfPackets(int num)`**: Increments the total packet count by the specified number.
5. **`storeSentPacket(const PacketPtr_t &packet)`**: Stores a sent packet for tracking and analysis purposes.


### Methods

1. **Public Methods**

   - **`Simulator(QObject *parent = nullptr)`**: Constructor that initializes fields and connects signals to appropriate slots.
   - **`~Simulator()`**: Destructor that clears the `eventsCoordinator`.
   - **`static Simulator *instance(QObject *parent = nullptr)`**: Retrieves the singleton instance, creating it if necessary.
   - **`static void release()`**: Destroys the singleton instance.
   - **`void run()`**: Starts the simulation by transitioning to the `Start` phase.

2. **Private Methods**

   - **`void goToNextPhase(UT::Phase nextPhase)`**: Transitions the simulation to the specified phase, updating internal states accordingly.
   - **`void start()`**: Prepares the simulation by loading configurations and initializing the `EventsCoordinator`.
   - **`void analysis()`**: Placeholder for analyzing simulation results.

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
   - Appends the packet to the `packetsSent` list and logs its size.

7. **`analysis()`**
   - Placeholder for performing analysis on the simulation results.

