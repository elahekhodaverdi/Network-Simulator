
## TopologyBuilder Class

The `TopologyBuilder` class is responsible for constructing various network topologies of routers, such as **Mesh**, **Ring-Star**, and **Torus**. It generates routers with unique MAC and IP addresses, and establishes connections (ports) between them based on the selected topology type. The class automates the process of building a network topology by organizing routers, binding ports, and ensuring the correct network layout.

### Fields

- **`routersNum`**: A static variable to keep track of the number of routers created. It is incremented with each new router instance to ensure unique router IDs.

### Methods Explanation

1. **`buildTopology()`**
   - This method is the main entry point for building a topology. It accepts the number of nodes, topology type, and an AS ID to generate routers. It creates each router with a unique MAC address using `MACAddressGenerator` and assigns an IP address based on the AS ID. The routers are then added to a list and connected to the `EventsCoordinator`.
   - After creating the routers, it switches to the specified topology type (`Mesh`, `Ring-Star`, or `Torus`) and builds the network layout by invoking the appropriate internal methods.
   - The method returns a list of constructed routers (`QList<RouterPtr_t>`).

2. **`buildMeshTopology()`**
   - This method arranges the routers in a **Mesh topology**. It organizes the routers into a square grid (n x n) and binds ports between adjacent routers (right-left, up-down).
   - If the number of routers does not form a perfect square, it logs a warning.

3. **`buildRingStarTopology()`**
   - This method arranges the routers into a **Ring-Star topology**. The routers are arranged in a ring with a central router. Each router in the ring is connected to its next neighbor, and additionally, every alternate router is connected to the central router (acting as the star).
   - It checks that the number of routers is valid for a Ring-Star topology (2n + 1 routers) and logs a warning if the condition is not met.

4. **`buildTorusTopology()`**
   - This method arranges the routers in a **Torus topology**, which is an extension of the Mesh topology. It connects the routers as in a Mesh, but also connects the first and last routers in both rows and columns, creating a "wrap-around" effect.

5. **`getRouterIndexAtMesh()`**
   - This utility method calculates the index of a router in a mesh grid based on its row and column position. This is used for establishing connections between adjacent routers in the mesh.

6. **`reset()`**
   - Reset the routersNum 


### Key Functionalities:

- **Router Creation**: Each router is instantiated with a unique MAC address (generated using `MACAddressGenerator`) and an IP address (derived from the AS ID and router number).
- **Port Binding**: Routers are connected through ports. The `PortBindingManager` is responsible for binding ports between routers.
- **Topology Building**: The class can build multiple types of topologies:
   - **Mesh**: A square grid of routers.
   - **Ring-Star**: A ring of routers with a central router acting as a star.
   - **Torus**: A mesh with wrap-around connections in both rows and columns.



## DHCPServer Class

The `DHCPServer` class simulates the behavior of a Dynamic Host Configuration Protocol (DHCP) server, responsible for assigning IP addresses dynamically and responding to client requests within the simulation framework.

### Fields

- **`m_ipRange`:**  
  Represents the base range of IP addresses that the server manages and assigns to clients.

- **`sentOffers`:**  
  A list of client IDs for which IP offers have already been sent.

- **`sentAcks`:**  
  A list of acknowledged IP addresses already assigned to clients.

### Signals

- **`newPacket`:**  
  Emitted when a new packet is generated, along with the triggering port.

### Methods Explanations

1. **`DHCPServer` (Constructor):**  
   Initializes the server with a specified IP range.

2. **`~DHCPServer` (Destructor):**  
   Cleans up the server upon destruction.

3. **`getIP`:**  
   Returns an IP address for a given client ID based on the server's IP range.

4. **`handleDiscoveryPacket`:**  
   Processes DHCP Discovery packets. If a client ID has not yet been offered an IP, generates a DHCP Offer packet and emits it.

5. **`handleRequestPacket`:**  
   Processes DHCP Request packets. If a client has not already been acknowledged, generates a DHCP Acknowledge packet and emits it.

## RoutingProtocol Class

The `RoutingProtocol` class provides a foundational interface for implementing routing protocols in network simulations. It manages routing table entries, decides packet routing, and communicates routing updates through signals. OSPF and RIP classes must define specific routing behaviors like neighbor addition and routing packet processing.

### Fields

- **`m_routingStarted`:** Indicates whether the routing process has been initiated.
- **`routingTable`:** Stores entries representing routes with details like destination, next hop, output port, metric, and protocol type.
- **`m_routerIP`:** Holds the IP address assigned to the router.
- **`m_routingTableUpdatedFromLastTick`:** Tracks whether the routing table changed during the last tick.
- **`routingIsDone`:** Flags the completion of routing.
- **`m_ticksFromLastUpdate`:** Counts ticks since the routing table's last modification.

### Signals

- **`newRoutingPacket`:** Triggered when a new routing packet is generated.
- **`noUpdateAtRoutingTable`:** Triggered if no updates occur in the routing table for a prolonged period.

### Methods Explanations

1. **`printRoutingTable`:** Displays the current routing table.
2. **`handleNewTick`:** Performs actions during each simulation tick based on the current phase.
3. **`setRouterIP`:** Assigns an IP address to the router.
4. **`startRouting`:** Initiates the routing protocol logic.
5. **`findOutPort`:** Determines the output port for a given destination IP.
6. **`addNewNeighbour`:** Adds a neighbor and associates it with a port.
7. **`processRoutingPacket`:** Processes an incoming routing packet.



## RIP Class

The `RIP` class implements the Routing Information Protocol (RIP), a distance-vector routing protocol, within the simulation framework. It extends the `RoutingProtocol` base class and manages routing tables, processes RIP packets, and exchanges routing updates with neighbors.

### Fields

- **`distanceVector`:**  
  Maintains a map of destination IPs to their respective distances (hop counts) in the network.

### Signals

- **`newRoutingPacket`:**  
  Emitted when a new RIP control packet is generated for transmission.

### Methods Explanations

1. **`RIP` (Constructor):** Initializes the RIP protocol and sets up the distance vector.

2. **`startRouting`:** Starts the routing process and sends the initial RIP packet.

3. **`addNewNeighbour`:** Adds a new neighbor to the routing table with an initial metric.

4. **`updateDistanceVector`:** Updates the distance vector and routing table if a better route is discovered.

5. **`processRoutingPacket`:** Processes incoming RIP packets and updates the distance vector based on received information.

6. **`sendRIPPacket`:** Creates and sends a RIP control packet with the router’s current distance vector.

7. **`convertDistanceVectorToJson`:** Converts the distance vector to a JSON format for inclusion in RIP packets.

8. **`convertToDistanceVector`:** Converts a JSON payload from a RIP packet into a distance vector map. 


## OSPF Class

The `OSPF` class implements the Open Shortest Path First protocol, extending `RoutingProtocol`. It employs link-state advertisements (LSAs) to construct a network topology and determine optimal routing paths.

### Fields

- **`lsDatabase`:** Stores LSAs received from the network.
- **`neighbourPorts`:** Maps neighbors' IP addresses to their corresponding ports.

### Methods Explanations

1. **`startRouting`:** Begins routing operations by broadcasting the initial LSA.
2. **`addNewNeighbour`:** Registers a neighbor's details and updates the routing table accordingly.
3. **`processRoutingPacket`:** Interprets and forwards OSPF packets.
4. **`sendLSAPacket`:** Sends LSAs to neighbors.
5. **`processLSA`:** Updates the database with the received LSA. Ensures database consistency and triggers a table recalculation if required.
6. **`recomputeRoutingTable`:** Rebuilds the routing table based on the current network topology. Utilizes Dijkstra's algorithm to derive optimal routes.



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

1. **`routerIsDone()`**: Updates the number of completed routers for the current phase and transitions to the next phase if conditions are met.
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

