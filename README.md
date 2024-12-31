
## Port Class

The `Port` class represents a network port within a router, responsible for managing the sending and receiving of packets. It is designed to function as part of a simulated network environment, enabling communication between nodes and routers. The class integrates with Qt's `QObject` to support event-driven programming through signals and slots.

### Fields

1. **`m_number`**:  
   - Stores the unique identifier (number) for the port.

2. **`m_numberOfPacketsSent`**:  
   - Tracks the total number of packets sent from this port.  
   - Incremented with each successful packet transmission.

3. **`m_routerIP`**:  
   - Holds the IP address of the router or PC to which the port belongs.

4. **`m_isInterASConnection`**:  
   - Boolean flag indicating if the port connects to a different Autonomous System (AS).  
   - Default value is `false`.

### Methods Explanation

1. **Constructor**:  
   Initializes the `Port` object with default values.
   - **`Port(QObject *parent = nullptr)`**:  
     Creates a new port and optionally sets its parent object.

2. **`sendPacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - Sends a packet if the provided `port_number` matches the port's number.  
   - Emits the `packetSent` signal and increments `m_numberOfPacketsSent`.

3. **`receivePacket(const PacketPtr_t &data)`**:  
   - Processes an incoming packet and emits the `packetReceived` signal with the packet data and port number.

4. **`getPortNumber()`**:  
   - Returns the port's unique identifier.

5. **`getRouterIP()`**:  
   - Retrieves the IP address of the router or PC associated with this port.

6. **`setPortNumber(uint8_t number)`**:  
   - Assigns a unique number to the port.

7. **`setRouterIP(QString routerIP)`**:  
   - Assigns an IP address to the router or PC associated with the port.

8. **`setIsInterASConnection()`**:  
   - Marks the port as part of an inter-AS connection.

9. **`isInterAsConnection()`**:  
   - Returns `true` if the port is an inter-AS connection, otherwise `false`.

### Signals

1. **`packetSent(const PacketPtr_t &data)`**:  
   - Emitted when a packet is successfully sent through the port.

2. **`packetReceived(const PacketPtr_t &data, uint8_t port_number)`**:  
   - Emitted when a packet is received by the port, providing the packet data and port number. 

## Router Class

The `Router` class represents a network router that extends the functionality of the `Node` class. It handles various tasks such as managing ports, routing packets, interacting with a DHCP server, and supporting different routing protocols like OSPF and RIP. It is designed to function within a simulation environment.

### Fields

- **`maxPorts`:** Specifies the maximum number of ports that the router can support.
- **`maxBufferSize`:** Defines the maximum size of the packet buffer.
- **`broken`:** Indicates whether the router is in a broken state and cannot process packets.
- **`dhcpServer`:** Manages DHCP operations if the router is acting as a DHCP server.
- **`ports`:** Stores the list of ports associated with the router.
- **`buffer`:** Temporarily holds packets before they are processed or forwarded.
- **`routingProtocol`:** Holds the instance of the currently active routing protocol (e.g., OSPF, RIP).
- **`ipVersion`:** Specifies the IP version the router is using (default: IPv4).
- **`packetsToSend`:** Maps ports to packets that are ready to be sent.

### Signals

Signals are used in the `Router` class to enable communication between components in the Qt framework. They facilitate event-driven programming and ensure modularity.

- **`routingProtocolIsDone`:** Emitted when the routing protocol completes its operation.
- **`newPacket`:** Signals that a new packet is ready for transmission.
- **`packetReceived`:** Emitted when a packet is received.

### Methods Explanations

1. **`Router` (Constructor):** Initializes the router by setting up ports, configuring the routing protocol, and starting the thread for the router's operations.

2. **`~Router` (Destructor):** Cleans up resources by unbinding ports, disconnecting signals, and stopping the thread.

3. **`markAsBroken`:** Marks the router as broken, disabling its ability to process most packets.

4. **`isBroken`:** Returns the router's broken status.

5. **`setIP`:** Assigns an IP address to the router and updates its routing protocol and ports with the new address.

6. **`setAsDHCPServer`:** Configures the router to function as a DHCP server within a specified IP range.

7. **`getPorts`:** Returns the list of ports.

8. **`getAnUnboundPort`:** Retrieves an unbound port that is available for use.

9. **`numRemainingPorts`:** Calculates the number of unbound ports remaining.

10. **`addPacketTobBuffer`:** Adds a packet to the router's buffer for later processing.

11. **`handleNewTick`:** Handles events associated with a new simulation phase, updating the buffer, sending packets, and interacting with the routing protocol.

12. **`updateBuffer`:** Processes the packet buffer, preparing packets for transmission based on routing decisions.

13. **`handlePhaseChange`:** Manages actions specific to different simulation phases, such as DHCP, neighbor identification, and routing.

14. **`initializeRoutingProtocol`:** Instantiates and configures the routing protocol based on the simulation configuration.

15. **`connectPortsToSignals`:** Establishes connections between port signals and router methods to facilitate packet handling.

16. **`receivePacket`:** Processes incoming packets, decrementing their TTL and handling them based on their type.

17. **`handleControlPacket`:** Processes control packets, such as DHCP messages and routing requests.

18. **`printRoutingTable`:** Outputs the current routing table.

19. **`numBoundPorts`:** Returns the number of currently bound ports.


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

### Methods Explanations

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

