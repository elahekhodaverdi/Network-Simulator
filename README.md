# CN_CA3


## **IP Class**

The **IP** class template provides a structured representation of IPv4 and IPv6 addresses, along with utility methods for manipulation and validation. It inherits from the `AbstractIP` base class.

### **AbstractIP Base Class**

**Fields**

- **m_ipValue**: A `uint64_t` value representing the raw IP address.

**Constructor**

- Default: Initializes `m_ipValue` to `0`.
- Copy Constructor.



### **IP (Specialization for IPv4)**

 **Fields**
- Inherits `m_ipValue` from `AbstractIP`.

 **Constructors**
- **Default**: Sets the IP to the maximum `uint64_t` value.
- **From String**: Accepts an IPv4 string and parses it into `m_ipValue`.
- **From Integer**: Accepts a `uint64_t` value directly.
- **Copy Constructor**: Creates a copy of an existing IPv4 instance.

 **Methods**
1. **toString**:
   - Converts `m_ipValue` into  `"X.X.X.X"` format.
   - Uses bitwise shifts and masks to extract octets.
2. **toValue**:
   - Returns the `uint64_t` representation of the IP.
3. **fromString**:
   - Parses an IPv4 string and updates `m_ipValue`.
   - Validates the format using `validateIPv4String`.
4. **validateIPv4String**:
   - Checks if the input string conforms to the IPv4 standard.
   - Ensures there are four octets, each between `0` and `255`.

5. **Operators**
    - **Equality Operator**: Compares the `m_ipValue` of two IPv4 addresses.


## IPHeader Class

The `IPHeader` class represents the structure and functionality of an IPv4 header, as defined in the Internet Protocol (IP). It provides methods to manage and manipulate various header fields, including version, length, type of service, and other relevant parameters. It also includes functions for setting and retrieving these fields, as well as calculating the header checksum.

### Fields

- **`m_versionHeaderLength`**: Stores the version and header length, combined in one byte.
- **`m_typeOfService`**: Stores the type of service field, typically used for routing priority and delay.
- **`m_totalLength`**: Stores the total length of the IP packet, including both header and data.
- **`m_identification`**: Stores the identification field for uniquely identifying fragmented packets.
- **`m_flagsFragmentOffset`**: Stores flags and fragment offset used for packet fragmentation.
- **`m_ttl`**: Stores the time-to-live value, used to limit the lifetime of a packet.
- **`m_protocol`**: Stores the protocol field, identifying the protocol used in the data portion of the IP packet.
- **`m_headerChecksum`**: Stores the checksum of the header for error-checking.
- **`m_sourceIp`**: Stores the source IP address.
- **`m_destIp`**: Stores the destination IP address.

### Method Explanations

1. **Constructors**
    - **`IPHeader(QObject *parent)`**:
        - Initializes the fields to default values. The `ttl` field is set using a value from the `SimulationConfig::TTL`.

2. **Setters**
    - Each of the fields has a corresponding setter method.

3. **Getters**
    - Each of the fields has a corresponding getter method.

4. **`calculateHeaderChecksum(const QByteArray &data)`**
    - Calculates the header checksum using a method based on the data size and values. It processes the data in 16-bit chunks, adds them together, and performs a final sum adjustment to generate the checksum.


## Node Class

The `Node` class represents a network node within a simulated network environment. This class is designed as an abstract base class for Router and PC. Additionally, it incorporates threading capabilities by inheriting from `QThread`, enabling asynchronous operations.

### Fields

1. **`m_id`**:  
   - Stores the unique identifier of the node.  
   - This ID helps in distinguishing nodes within the network.

2. **`m_MACAddress`**:  
   - Holds the MAC address of the node, uniquely identifying it at the data-link layer.  
   - It is generated using `MACAddressGenerator` if not explicitly provided during construction.

3. **`m_IP`**:  
   - Stores the IP address of the node.  
   - The address is dynamically allocated during object creation using the `IPv4Ptr_t::create()` method.

### Methods Explanation

1. **Constructors**:  
   The `Node` class provides two constructors to handle node initialization:
   - **`Node(int id, MACAddress macAddress, QObject *parent = nullptr)`**:  
     Initializes the node with a specified ID and MAC address.
   - **`Node(int id, QObject *parent = nullptr)`**:  
     Automatically generates a random MAC address for the node using `MACAddressGenerator::getRandomMAC()`.

2. **`~Node()`**:  
   - Destructor for the `Node` class.  
   - Ensures the proper cleanup of the dynamically allocated IP address by clearing the `m_IP` pointer if it is not null.

3. **`int getId()`**:  
   - Returns the unique identifier of the node.  

4. **`IPv4Ptr_t getIP()`**:  
   - Provides access to the node's IP address.

5. **`MACAddress getMACAddress()`**:  
   - Returns the MAC address of the node.

6. **`virtual void setIP(IPv4Ptr_t ip) = 0`**:  
   - A pure virtual function that must be implemented by derived classes.  
   - Defines how a node's IP address is set.

7. **`virtual void receivePacket(const PacketPtr_t &data, uint8_t port_number) = 0`**:  
   - A pure virtual slot to handle incoming packets.  
   - Must be implemented by derived classes to define the behavior upon receiving packets.

8. **`void checkCurrentThread()`**:  
   - Verifies if the current thread is the same as the thread of the object.  
   - Outputs debug information about the current thread and the object's thread.  
   - Useful for ensuring thread safety during multi-threaded operations.

### Signals

1. **`void newPacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - A Qt signal emitted when a new packet is created and ready to send. 


## PC Class

The `PC` class models a personal computer within a simulated network environment. It extends the `Node` class, inheriting its core functionalities while implementing additional features for sending and receiving packets. The class utilizes Qt's threading and signal-slot mechanisms for asynchronous operations and efficient network simulation.

### Fields

1. **`m_gateway`**:  
   - Represents the gateway port through which the PC communicates with the network.  
   - This is a dynamically allocated `PortPtr_t` object initialized during the construction of the `PC`.

### Methods Explanation

1. **Constructors**:  
   The `PC` class provides two constructors for initialization:  
   - **`PC(int id, MACAddress macAddress, QObject *parent = nullptr)`**:  
     Initializes the PC with a specific MAC address and ID, creating a gateway for network communication.  
     Starts the thread for the object and sets up the gateway.  
   - **`PC(int id, QObject *parent = nullptr)`**:  
     Initializes the PC with a randomly generated MAC address, ID, and gateway.  
     Also starts the thread and sets up the gateway.

2. **`~PC()`**:  
   - Destructor for the `PC` class.  
   - Ensures proper cleanup by unbinding the gateway port from the `PortBindingManager`.  
   - Disconnects any signal-slot connections related to the gateway and packet communication.

3. **`void sendPacket(QVector<QSharedPointer<PC>> selectedPCs)`**:  
   - Sends a packet to one of the selected PCs if the current PC is part of the selection.  
   - Generates a new packet using `createNewPacket()` and emits the `newPacket` signal to initiate transmission.

4. **`PCPtr_t chooseRandomPC()`**:  
   - Randomly selects a PC from the network that is different from the current PC.  
   - Utilizes `QRandomGenerator` to ensure a fair selection.

5. **`PacketPtr_t createNewPacket()`**:  
   - Creates a new data packet destined for a randomly selected PC.  
   - Sets the source and destination MAC and IP addresses within the packet's headers.  
   - Populates the payload with a message and calculates the IP header checksum.  
   - Returns the constructed packet.  

6. **`void receivePacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - Handles incoming packets through the gateway.  
   - Logs the packet's content and identifies the receiving PC for debugging purposes.

7. **`void setIP(IPv4Ptr_t ip)`**:  
   - Overrides the `Node` class's pure virtual method to set the IP address of the PC.  
   - Updates the router IP address associated with the gateway.

8. **`PortPtr_t gateway()`**:  
   - Returns the gateway port associated with the PC.  
   - Provides access to the port for further configurations or queries.

9. **`void setupGateway()`**:  
   - Configures the gateway by connecting the `packetReceived` signal of the gateway port to the `receivePacket` slot of the PC.  
   - Connects the `newPacket` signal of the PC to the `sendPacket` slot of the gateway port.  
   - Ensures the router IP of the gateway is updated.

### Signals

1. **`void newPacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - Emitted when a new packet is ready to be sent.  
   - Enables seamless communication with the gateway.  


## Router Class

The `Router` class models a network router in a simulated network environment. As a derived class of `Node`, it incorporates routing-specific functionality and is equipped to handle dynamic networking scenarios. It also utilizes threading capabilities through `QThread` for concurrent operations.

### Fields

1. **`ports`**:  
   - Stores a list of pointers to the router's ports.  
   - Initialized during object creation, with each port dynamically allocated and assigned a unique port number.

2. **`ipVersion`**:  
   - Specifies the IP version used by the router.  
   - Default value is `UT::IPVersion::IPv4`.

3. **`buffer`**:  
   - A queue of packets pending to be processed or forwarded by the router.

4. **`DHCPServer`**:  
   - A boolean indicating whether the router is configured as a DHCP server.

5. **`broken`**:  
   - A boolean indicating whether the router is inoperable.  
   - When true, the router halts packet processing.

6. **`routingTable`**:  
   - A list of routing table entries, each containing destination, subnet mask, next hop, output port, metric, and routing protocol.

7. **`maxPorts`**:  
   - The maximum number of ports available on the router.  
   - Default value is 5.

### Methods Explanation

1. **Constructor**:  
   - Initializes the router with a specified ID and MAC address.  
   - Dynamically allocates ports, assigns port numbers, and sets up signal-slot connections for packet handling.  
   - Moves the router object to its thread for asynchronous operations.  

2. **Destructor**:  
   - Ensures proper cleanup of ports by unbinding them from the `PortBindingManager` and disconnecting signals.  
   - Stops the thread before destruction.

3. **`void setRouterAsDHCPServer()`**:  
   - Configures the router as a DHCP server by setting the `DHCPServer` flag to true.

4. **`void setRouterBroken()`**:  
   - Marks the router as inoperable by setting the `broken` flag to true.

5. **`bool routerIsBroken() const`**:  
   - Returns the status of the `broken` flag, indicating whether the router is operable.

6. **`PortPtr_t getAnUnboundPort() const`**:  
   - Retrieves the first unbound port from the router's list of ports.  
   - Returns `nullptr` if all ports are bound.

7. **`int remainingPorts() const`**:  
   - Returns the number of unbound ports available for use.

8. **`void setIP(IPv4Ptr_t ip)`**:  
   - Assigns the specified IP address to the router and updates all ports with the router's IP.

9. **`void printRoutingTable() const`**:  
   - Outputs the router's routing table, displaying entries such as destination, next hop, output port, and metric.

10. **`bool isDHCPServer() const`**:  
    - Returns the status of the `DHCPServer` flag.

11. **`void addRoutingTableEntry(...)`**:  
    - Adds an entry to the routing table with the specified destination, subnet mask, next hop, output port, metric, and routing protocol.

12. **`void sendPacket(QVector<QSharedPointer<PC>> selectedPCs)`**:  
    - Processes the first packet in the buffer and attempts to forward it through the appropriate port based on the routing table.  
    - Increments the waiting cycles for remaining packets in the buffer.  
    - Emits a `newPacket` signal to send the packet.

13. **`void receivePacket(const PacketPtr_t &data, uint8_t port_number)`**:  
    - Handles incoming packets received on a specific port.  
    - Adds the packet to the buffer if the router is operational.

14. **`uint8_t findSendPort(IPv4Ptr_t destIP)`**:  
    - Determines the output port number for a given destination IP using the routing table.  
    - Returns 0 if no route is found.

### Signals

1. **`void newPacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - Emitted when a packet is ready to be sent through a specific port.


## Port Class

The `Port` class represents a network port within a router, managing the sending and receiving of packets. It is designed to be part of a simulated network environment where communication occurs between different nodes and routers. The class provides both public methods and signals/slots, using Qt's `QObject` for event-driven programming.

### Fields

1. **`m_number`**:  
   - Stores the port number of the port.  
   - This number uniquely identifies the port in the router.

2. **`m_numberOfPacketsSent`**:  
   - Keeps track of the number of packets sent by the port.  
   - This is incremented each time a packet is sent through this port.

3. **`m_routerIP`**:  
   - Holds the IP address of the router or the pc that the port belongs to.  
   - Used for identifying which router the port is part of.

### Methods Explanation

1. **Constructor**:  
   The `Port` class provides a constructor to initialize a new port:
   - **`Port(QObject *parent = nullptr)`**:  
     Initializes the port with default values and optionally sets the parent object in a Qt-based application.

2. **`sendPacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - This method sends a packet through the port if the provided `port_number` matches the port's number.  
   - It emits the `packetSent` signal when a packet is sent and increments the `m_numberOfPacketsSent` counter.

3. **`receivePacket(const PacketPtr_t &data)`**:  
   - This method handles the reception of a packet and emits the `packetReceived` signal.  
   - It logs the reception of the packet, indicating the port number and router IP.

4. **`getPortNumber()`**:  
   - Returns the port number of the port.

5. **`getRouterIP()`**:  
   - Returns the IP address of the router associated with the port.

6. **`setPortNumber(uint8_t number)`**:  
   - Sets the port number of the port.

7. **`setRouterIP(QString routerIP)`**:  
   - Sets the IP address of the router associated with the port.

### Signals

1. **`void packetSent(const PacketPtr_t &data)`**:  
   - A Qt signal emitted when a packet is successfully sent from the port.

2. **`void packetReceived(const PacketPtr_t &data, uint8_t port_number)`**:  
   - A Qt signal emitted when a packet is received at the port, carrying the packet data and port number.


## PortBindingManager Class

The `PortBindingManager` class is responsible for managing the bindings between network ports. A binding refers to the connection between two ports, enabling communication between them. This class provides functionality to bind, unbind, and check the status of port connections, supporting the simulation of a network environment where ports can exchange data.

### Fields

1. **`bindings`**:  
   - A static `QMap` that stores the relationships between ports.  
   - The keys are `PortPtr_t` objects representing the ports, and the values are `QList<PortPtr_t>` objects that contain the list of ports that are bound to the key port.

### Methods Explanation

1. **Constructor**:  
   - **`PortBindingManager(QObject *parent = nullptr)`**:  
     Initializes the `PortBindingManager` object. The constructor does not take any specific initialization beyond the `QObject` parent.

2. **`printBindings()`**:  
   - This method prints the current bindings of all ports in the system.  
   - It iterates over the `bindings` map and outputs the port numbers of the bound ports to the console, showing which ports are connected.

3. **`printBindingsForaPort(PortPtr_t port)`**:  
   - This method prints the bindings associated with a specific port.  
   - It checks whether the port exists in the `bindings` map and then outputs the connected ports and their router IPs.

4. **`bind(const PortPtr_t &port1, const PortPtr_t &port2)`**:  
   - This method binds two ports together, enabling them to communicate with each other.  
   - It ensures that a port cannot be bound to itself and that the ports are not already bound.  
   - Once the binding is established, the method connects the `packetSent` signal of both ports to the `receivePacket` slot of the other port, allowing for packet transmission between them.

5. **`unbind(const PortPtr_t &port)`**:  
   - This method unbinds all ports that are bound to a given port.  
   - It removes the port from the `bindings` map and disconnects any established connections between the given port and its bound ports.

6. **`unbind(const PortPtr_t &port1, const PortPtr_t &port2)`**:  
   - This method unbinds two specific ports that are connected.  
   - It removes the connection from the `bindings` map and disconnects the signals and slots that were established for packet transmission between the ports.

7. **`isBounded(const PortPtr_t &port)`**:  
   - This method checks if a specific port has any active bindings.  
   - It returns `true` if the port is bound to any other ports, and `false` otherwise.

### Signals

- **`void bindingChanged(const QString &router1, uint8_t port1, const QString &router2, uint8_t port2, bool bind)`**:  
   - This signal (currently commented out) would be emitted whenever the binding status between two ports changes. It could be used for debugging or logging purposes.




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

### Key Functionalities:

- **Router Creation**: Each router is instantiated with a unique MAC address (generated using `MACAddressGenerator`) and an IP address (derived from the AS ID and router number).
- **Port Binding**: Routers are connected through ports. The `PortBindingManager` is responsible for binding ports between routers.
- **Topology Building**: The class can build multiple types of topologies:
   - **Mesh**: A square grid of routers.
   - **Ring-Star**: A ring of routers with a central router acting as a star.
   - **Torus**: A mesh with wrap-around connections in both rows and columns.




## **AutonomousSystem Class**

The `AutonomousSystem` class models a networked autonomous system, encapsulating its structure, configuration, and interactions with other systems.


### **Fields**

- **`id`**: Represents the unique identifier of the autonomous system (AS).

- **`topologyType`**: Specifies the topology type of the AS, such as mesh, torus, ring-star.

- **`nodeCount`**: Indicates the number of nodes (routers) in the AS.

- **`asGateways`**: Holds the list of routers that act as AS Gateways, enabling interconnection between different autonomous systems.

- **`userGateways`**: Stores the routers that act as User Gateways, providing entry points for user devices into the network.

- **`dhcpServer`**: Represents the router designated as the DHCP server for dynamically assigning IP addresses to devices.

- **`brokenRouters`**: Tracks routers marked as broken or non-functional.

- **`connections`**: Stores the associations between routers and connected user devices (PCs).

- **`routers`**: Contains all routers within the autonomous system.


### Methods Explanations

1. **Constructors**

    - **Default Constructor:**

    ```cpp
    AutonomousSystem();
    ```

    Initializes an empty autonomous system.

    - **Parameterized Constructor:**

    ```cpp
    AutonomousSystem(int asId, UT::TopologyType type);
    ```

    Initializes the autonomous system with a unique ID (`asId`) and a specified topology type (`type`).


2. **`build(QJsonObject config)`**
    - Builds the topology and configures the autonomous system using a JSON object. It performs the following steps:
        1. Sets the number of nodes (`nodeCount`) and builds the router topology using `TopologyBuilder::buildTopology`.
        2. Configures AS Gateways (`setASGateaways`), User Gateways (`setUserGateaways`), and the DHCP server (`setDHCPServer`).
        3. Marks specified routers as broken (`setBrokenRouters`).
        4. Associates user devices (PCs) with routers using `setGateways`.
        5. Establishes inter-AS connections with other autonomous systems via `setConnectToAS`.

3. **`findRouterById(int id)`**

    - Searches for and returns a router by its ID within the autonomous system. Returns `nullptr` if the router is not found.

4. **`setASGateaways(QJsonArray ASgatewayIds)`**

     - Configures the AS Gateways by assigning the routers identified in the JSON array. Gateways enable communication between different autonomous systems.

5. **`setUserGateaways(QJsonArray gatewayIds)`**

    - Configures the User Gateways by assigning the routers identified in the JSON array. These gateways act as entry points for user devices into the network.

6. **`setDHCPServer(int dhcpId)`**

    - Sets a specific router as the DHCP server, allowing it to dynamically assign IP addresses.

7. **`setBrokenRouters(QJsonArray brokenRouterIds)`**

    - Marks specific routers as broken, based on the IDs provided in the JSON array. This is used for simulating network failures.

8. **`setGateways(QJsonArray gateways)`**

     - Associates user devices (PCs) with routers, creating connections between them. For each user device:
        1. A new `PC` object is created with a unique IP address.
        2. The device is connected to a router via an unbound port.


9. **`setConnectToAS(QJsonArray ASs)`**

    - Establishes connections between this AS and other autonomous systems. For each connection:
        1. The source and target routers are identified by their IDs.
        2. Ports on these routers are bound together using the `PortBindingManager`.





## **ConfigReader Class**

The **ConfigReader** class is a utility designed to read and parse configuration files, specifically for setting up network simulations. It processes JSON-formatted files to extract simulation settings and build autonomous systems.

### Fields
All methods in this class are static, ensuring they are accessible without instantiating the class.

### Method Explanations

1. **`readJson`**
    - Reads a JSON file and validates its format.


2. **`readNetworkConfig`**
    - Main entry point for reading and processing the network configuration.
    - **Process**:
      - Reads the JSON object using `readJson`.
      - Splits general settings and autonomous system definitions.
      - Extracts simulation settings via `parseSimulationConfig`.
      - Processes the autonomous systems using `parseAutonomousSystems`.

3. **`parseSimulationConfig`**
    - Parses general simulation settings.
    - **Key Parameters**:
      - **Simulation Duration**: Overall runtime for the simulation.
      - **Cycle Duration**: Interval for each simulation cycle.
      - **TTL (Time-To-Live)**: Duration for packets.
      - **Packets Per Simulation**: Maximum number of packets.
      - **Statistical Distribution**: The distribution model for simulation.
      - **Router Buffer Size**: Buffer capacity of routers.
      - **Router Port Count**: Number of ports per router.
      - **Routing Protocol**: The protocol used for routing.
      - **Routing Table Size**: Maximum size of routing tables.
      - **Routing Packets Per Port Cycle**: Number of packets routed per cycle.
      - **Routing Per Port**: Boolean to enable/disable routing on a per-port basis.
      - **Routing Table Update Interval**: Updates in seconds (or infinite).

4. **`parseAutonomousSystems`**
    - Constructs `AutonomousSystem` instances based on the configuration.
    - **Steps**:
      - Iterates over an array of system definitions.
      - Extracts topology type using `stringToTopologyType`.
      - Builds and appends the system to the `Network::autonomousSystems` list.

5. **Utility Functions**
    - **stringToTopologyType**:
      - Converts string representations (`"Mesh"`, `"RingStar"`, `"Torus"`) into enumeration values.
    - **parseTopologyTypes**:
      - Splits compound topology type strings into a list of enumerated types.



## **SimulationConfig Class**

The `SimulationConfig` class serves as a centralized configuration system for simulation parameters. It defines static variables that represent key settings required to manage simulation behavior.

### **Fields**

1. **Static Fields:**
   - `simulationDurationMs`: The total duration of the simulation in milliseconds.
   - `cycleDurationMs`: Duration of a single simulation cycle in milliseconds.
   - `TTL`: Default time-to-live value for packets.
   - `packetsPerSimulation`: Number of packets processed during the simulation.
   - `statisticalDistribution`: Distribution type for simulation.
   - `routerBufferSize`: Buffer size of routers.
   - `routerPortCount`: Number of ports in each router.
   - `routingProtocol`: Protocol used for routing.
   - `routingTableUpdateInterval`: Interval for routing table updates in milliseconds.
   - `routingPerPort`: Whether routing is performed per port.
   - `routingTableSize`: Maximum size of routing tables.
   - `routingPacketsPerPortCycle`: Packets processed per port per cycle.

2. **Private Methods:**
   - `convertTimeToMs(const QString &timeString)`: Converts a time string to milliseconds.

### **Methods Explanation**

1. **Constructors**
    - **Default Constructor:**

        Initializes an empty configuration with default values.

        ```cpp
        SimulationConfig::SimulationConfig() {}
        ```

    - **Parameterized Constructor:**

        Populates simulation settings using provided arguments, converting time values from string to milliseconds.

        ```cpp
        SimulationConfig(const QString &simDuration, const QString &cycleDuration, ...);
        ```

2. **`convertTimeToMs`**

    - Converts a time string into milliseconds using regex matching.
        - Supports formats like `"10s"` or `"500ms"`.
        - Returns `0` for invalid formats.




## **Network Class**

The `Network` class models the overall simulation network, managing autonomous systems (AS), events coordination, and other components.

### **Fields**
1. **Static Fields:**
   - `PCs`: List of all PCs in the network.
   - `simulationConfig`: The simulation configuration object.
   - `autonomousSystems`: List of autonomous systems in the network.
   - `eventsCoordinator`: Pointer to the events coordinator.

### **Methods Explanation**

1. **Constructors**
    - **Default Constructor:**
        - Initializes the `eventsCoordinator` singleton.

        ```cpp
        Network::Network()
        {
            eventsCoordinator = EventsCoordinator::instance();
        }
        ```

    - **Destructor:**
        - Releases the events coordinator.

        ```cpp
        Network::~Network()
        {
            eventsCoordinator->release();
        }
        ```

2. **`findASById`**

    - Searches for and retrieves an autonomous system by its ID.
    - Returns `nullptr` if the AS is not found.

    ```cpp
    AutonomousSystemPtr_t Network::findASById(int id);
    ```

3. **`run`**

    - Initializes and runs the network simulation:
        - Reads the configuration from a JSON file.
        - Starts the simulation with parameters from `SimulationConfig`.

