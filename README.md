# CN CA3


## MACAddress Class

The `MACAddress` class represents and validates MAC addresses, providing functionality to store, convert, and validate MAC addresses. It supports initialization from both a string and a byte array.

### Fields

- **`m_address`**: stores the six bytes of the MAC address.

### Method Explanations

1. **Constructors**
    - **with array**:
        - This constructor directly initializes the `m_address` field using a 6-byte array.
    - **with QString**
        - Parses the input string if it follows the format `XX:XX:XX:XX:XX:XX` using the `isValid()` method.
        - If valid, it converts the string to its 6-byte representation. Invalid inputs result in a warning, and the address is set to all zeros.

3. **`toString()`**
   - Generates a colon-separated hexadecimal string representation of the MAC address.

4. **`isValid()`**
   - Uses a regular expression to validate the format of a MAC address string.

5. **Equality Operator (`==`)**
   - Compares each byte of the `m_address` field between two `MACAddress` objects.



## MACAddressGenerator Class

The `MACAddressGenerator` class provides functionality to generate and manage random MAC addresses. It ensures uniqueness by keeping track of previously assigned addresses and generates new ones based on a predefined Organizationally Unique Identifier (OUI).

### Fields

- **`m_assignedMACs`**: A static `QList<MACAddress>` that stores previously assigned MAC addresses to avoid duplication.
- **`m_OUIBytes`**: A static `std::array<uint8_t, 3>` representing the Organizationally Unique Identifier (OUI) portion of a MAC address. This is randomly initialized at runtime.

### Method Explanations

1. **Deleted Constructor**
   - The class is designed as a utility class with only static members and methods. The constructor is explicitly deleted to prevent instantiation.

2. **`generateRandomMAC()`**
   - Generates a MAC address by combining:
     - The predefined OUI bytes stored in `m_OUIBytes`.
     - Three randomly generated NIC-specific bytes.
   - Combines these bytes into a full 6-byte array and returns it as a `MACAddress`.

3. **`isMACAssigned()`**
   - Checks if the given MAC address exists in the `m_assignedMACs` list.
   - Returns `true` if the MAC address has been assigned, `false` otherwise.

4. **`getRandomMAC()`**
   - Calls `generateRandomMAC()` to create a new MAC address.
   - Ensures uniqueness by checking against `m_assignedMACs` using `isMACAssigned()`.
   - If the generated MAC address is already assigned, the method retries until a unique address is generated.
   - The unique address is added to `m_assignedMACs` and returned.


## DataLinkHeader Class

The `DataLinkHeader` class represents the data link layer header used in network communication.

### **Fields**

- **`m_sourceMACAddress`**: Represents the source MAC address of the frame (`MACAddress`).

- **`m_destinationMACAddress`**: Represents the destination MAC address of the frame (`MACAddress`).

### **Methods**

1. **Constructors**

    - Initializes a new `DataLinkHeader` instance with the specified source and destination MAC addresses.
    - A copy constructor that creates a new `DataLinkHeader` by copying the source and destination MAC addresses from another instance.

2. **Setters**

    - Each of the fields has a corresponding setter method.

3. **Getters**
    - Each of the fields has a corresponding getter method.

## TCPHeader Class

The `TCPHeader` class represents the header of a TCP packet.

### Fields

- **`m_sourcePort`**: Stores the source port number (16 bits).
- **`m_destPort`**: Stores the destination port number (16 bits).
- **`m_sequenceNumber`**: Stores the sequence number of the packet (32 bits).
- **`m_acknowledgementNumber`**: Stores the acknowledgment number (32 bits).
- **`m_flags`**: Represents the control flags (8 bits).
- **`m_dataOffset`**: Indicates the size of the TCP header in 32-bit words (8 bits).
- **`m_checksum`**: Stores the checksum for error-checking purposes (16 bits).
- **`m_windowSize`**: Represents the size of the receive window (16 bits).
- **`m_urgentPointer`**: Points to the location of urgent data within the packet (16 bits).

### Methods Explanations

1. **Constructor:**
   - `TCPHeader(QObject *parent = nullptr)`: Initializes the class and sets the default checksum to `0`.

2. **Setters:**
   - Each of the fields has a corresponding setter method, such as `setSourcePort()`, `setDestPort()`, `setSequenceNumber()`, and so on.

3. **Getters:**
   - Each of the fields has a corresponding getter method, such as `sourcePort()`, `destPort()`, `sequenceNumber()`, and so on, to retrieve the value of the field.

4. **`calculateChecksum` Method:**

    - Calculates the checksum for the TCP header using the pseudo-header (source and destination IPs) and the actual header data.
    - Uses a helper function, onesComplementSum, to compute the one's complement sum of the data.

5. **`onesComplementSum` Helper Function:**
    - Computes the one's complement sum for a given byte array.


## Packet Class

The `Packet` class represents a network packet and provides methods to manipulate its properties. It encapsulates various attributes such as packet type, payload, sequence number, waiting and total cycles, and the path it traverses. The class also integrates headers such as `DataLinkHeader` and optionally supports `TCPHeader`.

### Fields

- **`m_packetType`**: An instance of `UT::PacketType` that specifies the type of the packet.
- **`m_payload`**: A `QByteArray` storing the packet's data payload.
- **`m_sequenceNumber`**: A 32-bit unsigned integer representing the packet's sequence number.
- **`m_waitingCycles`**: A 32-bit unsigned integer indicating how many cycles the packet has been waiting.
- **`m_totalCycles`**: A 32-bit unsigned integer tracking the total number of cycles the packet has been in the system.
- **`m_path`**: A `QList<QString>` representing the IP path the packet has traversed.
- **`m_dataLinkHeader`**: An instance of `DataLinkHeader` representing the data link layer header associated with the packet.
- **`m_tcpHeader`**: An optional `TCPHeader` instance providing support for TCP-related information, if applicable.
- **`m_controlType`**: An optional `UT::PacketControlType` for specifying control-related metadata for the packet.

### Constructor

1. **`Packet(DataLinkHeader dataLinkHeader, QObject* parent = nullptr)`**
   - Initializes a `Packet` object with a given `DataLinkHeader`.
   - Sets `m_totalCycles` and `m_waitingCycles` to zero.

### Method  

1. **`addToPath(QString ip)`**
   - Adds an IP address to the packet's path.
   - Appends the provided IP string to `m_path`.

2. **`incWaitingCycles()`**
   - Increments the `m_waitingCycles` field by one.

3. **`incTotalCycles()`**
   - Increments the `m_totalCycles` field by one.

4. **Setters**
    - Each of the fields has a corresponding setter method.

5. **Getters**
    - Each of the fields has a corresponding getter method.

### Destructor

- **`~Packet()`**
   - Ensures proper cleanup of the `Packet` instance when it goes out of scope.
  
### Future Expansion
  - IPHeader integration for network layer information.
  
  

## DataGenerator Class

The `DataGenerator` class is responsible for generating data values based on the Pareto distribution. The class is designed to provide flexibility in initialization and data generation parameters.


### Fields

- **`m_minValue`**: A private member variable of type `int` that specifies the minimum value for the Pareto distribution. Defaults to `1`.

### Method Explanations

1. **Constructor**
    - **Default**: Initializes the `DataGenerator` object with default values (`m_minValue` = 1).

    - **Parameterized**: Initializes the `DataGenerator` with a user-defined minimum value for the Pareto distribution.

2. **`generateParetoPacketCounts`**  
   - **Purpose**: Generates packet counts following the Pareto distribution using the formula:  
     $$
     X = \frac{\text{minValue}}  {(1 - U)^{\frac{1}{\alpha}}}
     $$  
     where $U$ is a uniformly distributed random value between 0 and 1, and $\alpha$ is the Pareto shape parameter.  
   - **Details**:  
   We'll discuss a step-by-step explanation of the formula used in `generateParetoPacketCounts` and how it was derived just below.


### Understanding the Pareto Distribution Formula

The probability density function (PDF) is given by:

$$
f(x) = \alpha \frac{{x_m^\alpha}}{{x^{\alpha + 1}}}, \quad x \geq x_m, \, \alpha > 0
$$

Here:
- $x$: The variable of interest.
- $x_m$: The minimum value of $x$ (also called the scale parameter).
- $\alpha$: The shape parameter, controlling the "heaviness" of the distribution tail.

From the PDF, we derive the cumulative distribution function (CDF), which gives the probability that a random variable $X$ is less than or equal to some value $x$:

$$
F(x) = 1 - \left(\frac{x_m}{x}\right)^\alpha, \quad x \geq x_m
$$

To generate a random number following this distribution, we use the inverse transform sampling method.

### Inverse Transform Sampling

The key idea of inverse transform sampling is:
1. Start with a random number $U$ uniformly distributed in \([0, 1]\).
2. Transform $U$ using the inverse of the CDF $F(x)$ to produce a random value $X$ following the desired distribution.

For the Pareto distribution, the CDF is:

$$
F(x) = 1 - \left(\frac{x_m}{x}\right)^\alpha
$$

Setting $F(x) = U$, we get:

$$
U = 1 - \left(\frac{x_m}{x}\right)^\alpha
$$

Rearranging for $x$:
$$
x = x_m \cdot (1 - U)^{-1/\alpha}
$$

This formula allows us to generate a random value $x$ following the Pareto distribution given:

- $U$: A uniformly distributed random value.
- $x_m$: The minimum value (scale parameter).
- $\alpha$: The shape parameter.


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
  - `reset`: Resets simulation state and variables.
  - `cleanup`: Cleans up and releases resources.
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


#### **`startSimulation`**  

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


#### **`stopSimulation`**  

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


#### **`run`**  

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


#### **`runExecutionCycle`**  

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


#### **`onTimerTick`**  

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


## Node Class

The `Node` class represents a network node within a simulated network environment. This class is designed as an abstract base class for Router and PC. Additionally, it incorporates threading capabilities by inheriting from `QThread`, enabling asynchronous operations.

### Fields

1. **`m_id`**:
   - Stores the unique identifier of the node.

2. **`m_MACAddress`**:
   - Holds the MAC address of the node, uniquely identifying it at the data-link layer.

3. **`m_IP`**:
   - Stores the IP address of the node.

4. **`m_currentPhase`**:
   - Represents the current phase of the node in the simulation, defined by the `UT::Phase` enum.

5. **`m_dhcpIsDone`**:
   - A boolean flag indicating whether the DHCP process is completed.

### Methods Explanation

1. **Constructors**:
   - **`Node`**:
     Initializes the node with a specified ID and MAC address.
   - **`Node`**:
     Automatically generates a random MAC address for the node using `MACAddressGenerator::getRandomMAC()`.

2. **`~Node()`**:
   - Destructor for the `Node` class.
   - Ensures the proper cleanup of the dynamically allocated IP address by clearing the `m_IP` pointer if it is not null.

3. **`getId`**:
   - Returns the unique identifier of the node.

4. **`getIP`**:
   - Provides access to the node's IP address.

5. **`getMACAddress`**:
   - Returns the MAC address of the node.

6. **`setIP`**:
   - A pure virtual function that must be implemented by derived classes.
   - Defines how a node's IP address is set.

7. **`receivePacket`**:
   - A pure virtual slot to handle incoming packets.
   - Must be implemented by derived classes to define the behavior upon receiving packets.

8. **`checkCurrentThread`**:
   - Verifies if the current thread is the same as the thread of the object.
   - Outputs debug information about the current thread and the object's thread.
   - Useful for ensuring thread safety during multi-threaded operations.

9. **`sendDiscoveryDHCP`**:
   - Sends a DHCP discovery packet as part of the DHCP process.

10. **`sendRequestDHCP`**:
    - Sends a DHCP request packet to obtain an IP address after receiving an offer.

11. **`handleOfferDHCP`**:
    - Handles incoming DHCP offer packets.
    - Extracts the offered IP address and sends a DHCP request if the ID matches.

12. **`handleAckDHCP`**:
    - Processes DHCP acknowledgment packets to finalize the DHCP process.
    - Sets the `m_dhcpIsDone` flag to `true`.

13. **`setDHCPDone`**:
    - Marks the DHCP process as complete by setting the `m_dhcpIsDone` flag and emitting the `dhcpIsDone` signal.

14. **`handleRequestPacket`**:
    - Handles incoming request packets by sending a response packet to the source.

15. **`sendResponsePacket`**:
    - Creates and sends a response packet to the source IP address specified in the request.

### **Signals**

1. **`newPacket`**:
   - A Qt signal emitted when a new packet is created and ready to send.

2. **`dhcpIsDone`**:
   - A Qt signal emitted upon completion of the DHCP process.

3. **`newPacketSent`**:
   - A Qt signal emitted after sending a new packet (actually for sending a new packet, it notifies).

4. **`packetReceived`**:
   - A Qt signal emitted when a packet is received.



## PC Class

The `PC` class models a personal computer within a simulated network environment. It extends the `Node` class, inheriting its core functionalities while implementing additional features for sending and receiving packets.

### Fields

1. **`m_gateway`**:
   - Represents the gateway port through which the PC communicates with the network.
   - This is a dynamically allocated `PortPtr_t` object initialized during the construction of the `PC`.

2. **`shouldSendNewPacket`**:
   - A boolean flag indicating whether the PC should send a new packet in the current simulation phase.
   - Default value is `false`.

### Methods Explanation

1. **Constructors**:
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

3. **`setIP`**:
   - Overrides the `Node` class's pure virtual method to set the IP address of the PC.

4. **`gateway`**:
   - Returns the gateway port associated with the PC.

5. **`setupGateway`**:
   - Configures the gateway by connecting the `packetReceived` signal of the gateway port to the `receivePacket` slot of the PC.
   - Connects the `newPacket` signal of the PC to the `sendPacket` slot of the gateway port.
   - Ensures the router IP of the gateway is updated.

6. **`handleNewTick`**:
   - Handles updates for the current simulation phase.
   - Checks and processes phase transitions, and sends a new packet if the `shouldSendNewPacket` flag is set.

7. **`setShouldSendPacket(QVector<QSharedPointer<PC>> selectedPCs)`**:
   - Updates the `shouldSendNewPacket` flag based on whether the current PC is among the selected PCs.

8. **`sendNewPacket()`**:
   - Generates and sends a new data packet through the gateway.

9. **`chooseRandomPC()`**:
   - Randomly selects a PC from the network that is different from the current PC.
   - Ensures a fair selection using `QRandomGenerator`.

10. **`createNewPacket()`**:
    - Creates a new data packet destined for a randomly selected PC.
    - Populates the MAC, IP headers, payload, and checksum for the packet.
    - Returns the constructed packet.

11. **`handleControlPacket(const PacketPtr_t &data)`**:
    - Processes control packets such as DHCP offers, acknowledgments, and requests.
    - Handles specific actions based on the control type.

12. **`receivePacket(const PacketPtr_t &data, uint8_t portNumber)`**:
    - Handles incoming packets through the gateway.
    - Emits the `packetReceived` signal for further processing.
    - Identifies and processes control packets if applicable.

13. **`broadcastPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort)`**:
    - Sends a broadcast packet through the gateway, avoiding retransmission from the triggering port.

14. **`addPacketForBroadcast(const PacketPtr_t &packet, PortPtr_t triggeringPort)`**:
    - Adds a packet for broadcasting to the network.
    - Delegates the actual transmission to the `broadcastPacket` method.

15. **`handlePhaseChange(const UT::Phase nextPhase)`**:
    - Updates the simulation phase and performs phase-specific actions, such as initiating DHCP discovery.

### Signals

1. **`newPacket(const PacketPtr_t &data, uint8_t port_number)`**:
   - Emitted when a new packet is ready to be sent.

2. **`packetReceived(const PacketPtr_t &data)`**:
   - Emitted when a packet is received by the PC.


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


## Network Class

The `Network` class models the simulation network, managing its autonomous systems (AS), PCs, and providing utility functions for router statistics.

### Fields

1. **Static Fields**
   - **`PCs`**: A `QList` containing pointers to all PCs in the network.
   - **`autonomousSystems`**: A `QList` containing pointers to all autonomous systems in the network.

### **Methods Explanation**

#### **Constructors**

1. **Default Constructor**

2. **Destructor**

#### **Static Methods**

1. **`findASById`**  
   - Searches for and retrieves an autonomous system by its ID.  
   - Returns `nullptr` if no matching autonomous system is found.  


2. **`findRouterById`**  
   - Searches for and retrieves a router by its ID across all autonomous systems.  
   - Returns `nullptr` if no matching router is found.  


3. **`getAllRoutersIPs`**  
   - Collects and returns a list of all router IPs across the autonomous systems.  

4. **`numOfRouters`**  
   - Calculates and returns the total number of routers in all autonomous systems.  

5. **`numOfBrokenRouters`**  
   - Calculates and returns the total number of broken routers across all autonomous systems.  

6. **`reset`**  
   - Resets the network by clearing all PCs and autonomous systems.  
   - Frees up resources used by the network.  

## ConfigReader Class

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



## Phase 1 Tests

### **Testing the EventCoordinator System**

To validate the functionality of the `EventCoordinator` system, we implemented `Node` and `PC` classes. These were used to simulate PCs that receive data packets at regular intervals, as coordinated by the EventCoordinator.

#### **Node and PC Implementation**

The `Node` class serves as a base class for the `PC`, representing individual PCs in the system. Each PC can send data packets when connected to the EventCoordinator and received a signal to do so.

**Node Header File:**
```cpp
#ifndef NODE_H
#define NODE_H

#include <QThread>
#include <QDebug>

class Node : public QThread
{
    Q_OBJECT

public:
    explicit Node(QObject *parent = nullptr);
};

#endif    // NODE_H
```

**PC Header File:**
```cpp
#ifndef PC_H
#define PC_H

#include "../Node/Node.h"
#include <QVector>
#include <QSharedPointer>

class PC : public Node
{
    Q_OBJECT

public:
    explicit PC(int id, QObject *parent = nullptr);

public Q_SLOTS:
    void sendPacket(QVector<QSharedPointer<PC>> selectedPCs);

protected:
    int m_id;
};

#endif    // PC_H
```

**PC Source File:**
```cpp
#include "PC.h"

PC::PC(int id, QObject *parent) :
    Node(parent)
{
    m_id = id;
}

void PC::sendPacket(QVector<QSharedPointer<PC>> selectedPCs) {
    if (!selectedPCs.contains(this))
        return;
    qDebug() << "sending packet from " << m_id;
}
```

#### **Testing with Main Function**

The `main` function demonstrates the simulation of the system by:

1. Creating an instance of `EventCoordinator` using its singleton pattern.
2. Initializing 10 PCs.
3. Connecting the EventCoordinator's `nextTick` signal to each PC's `sendPacket` slot.
4. Starting the simulation with an interval of 200 milliseconds and a total duration of 10,000 milliseconds.

**Main Function:**
```cpp
#include "../EventsCoordinator/EventsCoordinator.h"
#include <QCoreApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    EventsCoordinator *eventCoordinator = EventsCoordinator::instance();

    QVector<QSharedPointer<PC>> pcs;
    for (int i = 0; i < 10; i++) {
        QSharedPointer<PC> pc = QSharedPointer<PC>::create(i);
        pcs.append(pc);

        QObject::connect(eventCoordinator, &EventsCoordinator::nextTick, pc.get(), &PC::sendPacket);
    }

    eventCoordinator->startSimulation(200, 10000, pcs);
    int result = app.exec();

    EventsCoordinator::release();

    return result;
}
```

---

#### **Output**

The system's test outputs include:
1. A debug log showing the number of data packets distributed in each cycle.
2. Logs for PCs receiving packets, indicating the PC ID.

**Output:**
![EventCoordinator Test Phase 1](./imgs/EventCoordinatorTest_P1.png)

This output validates that:
- Data distribution among cycles matches the configuration.
- Each PC correctly receives packets when selected.

### **Testing the MAC Address**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include "MACAddress/MACAddress.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::array<uint8_t, 6> addressArray = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
    MACAddress mac1(addressArray);
    qDebug() << "MACAddress from array:" << mac1.toString(); //12:34:56:78:9A:BC

    MACAddress mac2("AB:CD:EF:12:34:56");
    qDebug() << "MACAddress from valid string:" << mac2.toString(); //AB:CD:EF:12:34:56

    MACAddress mac3("INVALID:ADDRESS");
    qDebug() << "MACAddress from invalid string:" << mac3.toString(); //00:00:00:00:00:00

    qDebug() << "Is '12:34:56:78:9A:BC' valid?" << mac2.isValid("12:34:56:78:9A:BC"); // true
    qDebug() << "Is 'INVALID' valid?" << mac2.isValid("INVALID"); //false

    MACAddress mac4("12:34:56:78:9A:BC");
    qDebug() << "Are mac1 and mac4 equal?" << (mac1 == mac4); // true
    qDebug() << "Are mac2 and mac4 equal?" << (mac2 == mac4); // false

    return 0;
}
```

**What is being tested**:
1. **MAC Address Initialization**: We test if the `MACAddress` class can correctly handle MAC address initialization from both a byte array and a valid string. Additionally, we check how the class handles an invalid string (expecting it to default to `00:00:00:00:00:00`).
   
2. **MAC Address Validation**: The `isValid()` method is tested to ensure it accurately identifies valid MAC addresses (e.g., `12:34:56:78:9A:BC`) and rejects invalid strings (e.g., `INVALID`).
   
3. **Equality Check**: The `==` operator is tested for comparing two `MACAddress` objects, verifying that the operator correctly identifies equality when two addresses are the same and inequality when they differ.

**Desired Output**:
1. **MACAddress from array**: The output should display `12:34:56:78:9A:BC`, confirming that the class can properly initialize a MAC address from a byte array.
   
2. **MACAddress from valid string**: The output for this MAC address should be `AB:CD:EF:12:34:56`, showing that the class correctly parses a valid MAC address string.

3. **MACAddress from invalid string**: The output should be `00:00:00:00:00:00`, indicating that the class defaults to the "zero" address when provided with an invalid string like `INVALID:ADDRESS`.

4. **MAC Address validation**:
   - It should print `true` for the valid MAC address `12:34:56:78:9A:BC`.
   - It should print `false` for the invalid string `INVALID`.

5. **Equality Check**:
   - It should print `true` when comparing `mac1` and `mac4` because they are the same.
   - It should print `false` when comparing `mac2` and `mac4`, as they are different addresses.

#### **Output**
![MAC Address Test](./imgs/MacAddressTest_P1.png)

### Testing the MAC Address Generator

```cpp
#include <QDebug>
#include "MACAddress/MACAddress.h"
#include "MACAddress/MACAddressGenerator.h"

int main()
{
    QList<MACAddress> generatedMACs;

    for (int i = 0; i < 5; ++i) {
        MACAddress randomMAC = MACAddressGenerator::getRandomMAC();
        qDebug() << "Generated MAC Address #" << (i + 1) << ":" << randomMAC.toString();

        bool isUnique = true;
        for (const auto& mac : generatedMACs) {
            if (mac == randomMAC) {
                isUnique = false;
                break;
            }
        }

        if (isUnique) {
            qDebug() << "The MAC address is unique.";
        } else {
            qDebug() << "The MAC address is not unique.";
        }

        generatedMACs.append(randomMAC);
    }
    MACAddress tempMac(generatedMACs.first().toString());
    qDebug() << "is mac assigned?: " << MACAddressGenerator::isMACAssigned(tempMac);
    return 0;
}
```

**What is being tested**:
1. **Random MAC Address Generation**: The `MACAddressGenerator` class is tested to ensure it generates random and valid MAC addresses. We expect the generated addresses to follow the correct format (six groups of two hexadecimal characters separated by colons).

2. **Uniqueness of Generated MAC Addresses**: Each generated MAC address is checked for uniqueness within the current run. We test if the generator produces duplicates by comparing each newly generated address to the previously generated ones.

3. **MAC Address Assignment**: After generating five MAC addresses, we test whether the first MAC address in the list has already been assigned using `isMACAssigned()`. This verifies that the generator is keeping track of generated addresses.

**Desired Output**:
1. **Generated MAC Addresses**: Each of the five generated MAC addresses should be printed in valid MAC address format (e.g., `Generated MAC Address #1: XX:XX:XX:XX:XX:XX`), with each address being different.

2. **Uniqueness Check**: After generating each address, the system should output whether the generated MAC address is unique:
   - "The MAC address is unique." if the address has not been seen before in the current run.
   - "The MAC address is not unique." if the address is a duplicate.

3. **Assigned MAC Check**: After generating the five addresses, the program checks if the first address is assigned by calling `isMACAssigned()`. It will print either `true` or `false`, depending on whether the address has been previously assigned.

#### **Output**

![MAC Address Generator Test](./imgs/MacAddressGeneratorTest_P1.png)



## Phase 2 Tests

### **Testing the IP**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include "../IP/IP.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    IPv4_t ip1("192.168.1.1");
    qDebug() << "IP1:" << ip1.toString();

    IPv4_t ip2(3232235777); //  192.168.1.1
    qDebug() << "IP2:" << ip2.toString();

    // Comparing
    qDebug() << "IP1 == IP2:" << (ip1 == ip2);

    // Convert to a numeric value
    qDebug() << "IP1 Value:" << ip1.toValue();

    // invalid
    try {
        IPv4_t invalidIP("300.300.300.300");
    } catch (const std::exception &e) {
        qDebug() << "Caught exception for invalid IP:" << e.what();
    }

    return app.exec();
}
```

**What is being tested**:
1. **IP Initialization**: We test the `IPv4_t` class for its ability to initialize an IP address from both a string representation and a numeric value.
2. **IP Comparison**: We check if the `==` operator works correctly when comparing two IP addresses, ensuring that it identifies equal and unequal addresses.
3. **IP to Numeric Conversion**: We verify that the `toValue()` method can convert an IP address into its equivalent numeric form.
4. **Invalid IP Address**: We test the handling of an invalid IP address and ensure that the program throws an exception for values like `300.300.300.300`.

**Desired Output**:
1. **IP1 and IP2**: Both should display as `192.168.1.1`, demonstrating that initialization from numeric value and string works correctly.
2. **IP Comparison**: The comparison should output `true` since both `ip1` and `ip2` represent the same address.
3. **IP to Value**: The numeric value of `192.168.1.1` (3232235777) should be printed.
4. **Invalid IP**: An exception is caught, and the message for invalid IP is printed.

#### **Output**
![IP Test Phase 2](./imgs/IPTest_P2.png)

### **Testing the Port**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include "../Packet/Packet.h"
#include "../Port/Port.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Port port;

    port.setPortNumber(1);
    qDebug() << "Port Number:" << port.getPortNumber(); // 1

    port.setRouterIP("192.168.1.1");
    qDebug() << "Router IP:" << port.getRouterIP(); // 192.168.1.1

    DataLinkHeader header(MACAddress("00:11:22:33:44:55"), MACAddress("66:77:88:99:AA:BB"));

    PacketPtr_t packet = QSharedPointer<Packet>::create(header);
    packet->setPayload("Test Data");

    QObject::connect(&port, &Port::packetSent, [](const PacketPtr_t &pkt) {
        qDebug() << "Packet Sent with Data:" << pkt->payload();
    });

    QObject::connect(&port, &Port::packetReceived, [](const PacketPtr_t &pkt) {
        qDebug() << "Packet Received with Data:" << pkt->payload();
    });

    port.sendPacket(packet, 1);
    port.receivePacket(packet);

    return app.exec();
}
```

**What is being tested**:
1. **Port Initialization**: The `Port` class is tested for its ability to set and get the port number and router IP.
2. **Packet Sending and Receiving**: We test the `sendPacket()` and `receivePacket()` methods by creating a packet and verifying that it is sent and received correctly.
3. **Signal-Slot Mechanism**: The test ensures that signals like `packetSent` and `packetReceived` are emitted and that connected slots handle the events properly.

**Desired Output**:
1. **Port Number**: The port number `1` should be printed, confirming that the `setPortNumber` and `getPortNumber` methods work.
2. **Router IP**: The router IP `192.168.1.1` should be displayed, confirming that the IP setter and getter work.
3. **Packet Sending and Receiving**: Debug logs will show the packet's payload when it is sent and received, ensuring the correct functionality of packet communication.

#### **Output**
![Port Test Phase 2](./imgs/PortTest_P2.png)

### **Testing the Router**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include "src/MACAddress/MACAddressGenerator.h"
#include "src/Packet/Packet.h"
#include "src/PortBindingManager/PortBindingManager.h"
#include "src/Router/Router.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // MAC address for the router
    MACAddress macAddress = MACAddressGenerator::generateRandomMAC();

    // Initialize Router
    qDebug() << "Creating Router with ID 1...";
    Router router(1, macAddress);

    // Check Router properties
    qDebug() << "Router ID:" << router.getId();
    qDebug() << "MAC Address:" << macAddress.toString();
    qDebug() << "Router has" << router.remainingPorts() << "remaining ports.";

    // Set Router as DHCP Server
    router.setRouterAsDHCPServer();
    qDebug() << "Is DHCP Server?" << router.isDHCPServer();

    // Break the Router
    router.setRouterBroken();
    qDebug() << "Is Router Broken?" << router.routerIsBroken();

    // Bind Ports
    qDebug() << "Binding Ports...";
    PortPtr_t port1 = router.getAnUnboundPort();
    PortPtr_t port2 = PortPtr_t::create();
    port2->setPortNumber(11);

    if (port1 && port2) {
        PortBindingManager::bind(port1, port2);
        qDebug() << "Bound port" << port1->getPortNumber() << "to port" << port2->getPortNumber();
    } else {
        qDebug() << "Not enough unbound ports!";
    }

    // Check remaining unbound ports
    qDebug() << "Router has" << router.remainingPorts() << "remaining ports.";

    // Set Router IP
    IPv4Ptr_t routerIP = IPv4Ptr_t::create("192.168.1.1");
    router.setIP(routerIP);
    qDebug() << "Router IP set to:" << router.getIP()->toString();

    // Add a packet to the buffer and test packet forwarding
    qDebug() << "Testing Packet Sending and Receiving...";
    PacketPtr_t packet = PacketPtr_t::create(DataLinkHeader(macAddress, macAddress));
    packet->setPacketType(UT::PacketType::Data);
    packet->setPayload("Test Router Recieving Packet");

    port2->sendPacket(packet, 11);

    // Print Routing Table
    IPv4Ptr_t IP1 = IPv4Ptr_t::create("192.168.1.2");
    IPv4Ptr_t IP2 = IPv4Ptr_t::create("192.168.1.3");
    router.addRoutingTableEntry(IP1, 24, IP2, port1, 3, UT::RoutingProtocol::OSPF);
    router.printRoutingTable();

    return app.exec();
}
```

**What is being tested**:
1. **Router Initialization**: We test the creation of a router and check its properties such as ID, MAC address, and remaining ports.
2. **DHCP Server Status**: We verify if the router can be set as a DHCP server and check its status.
3. **Router Failure**: We simulate the router's failure by calling the `setRouterBroken()` method and verify its status.
4. **Port Binding**: We test binding ports using the `PortBindingManager` and check if the correct ports are bound together.
5. **Packet Forwarding**: The router is tested for forwarding packets from one port to another.

**Desired Output**:
1. **Router Properties**: The router's ID, MAC address, and remaining ports should be printed correctly.
2. **DHCP Server Status**: The output should confirm whether the router is acting as a DHCP server.
3. **Router Failure**: The router's failure status should be checked and displayed as `true` or `false`.
4. **Port Binding**: The output should confirm successful binding of ports and show the remaining unbound ports.
5. **Packet Forwarding**: A packet should be sent from one port and received by another, confirming that packet forwarding works correctly.

#### **Output**
![Router Test Phase 2](./imgs/RouterTest_P2.png)


### **Testing the Topology Builder**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include "../PortBindingManager/PortBindingManager.h"
#include "../Topology/TopologyBuilder.h"

void printBoundedPorts(const QList<PortPtr_t>& ports)
{
    qDebug() << "--------------------------------------";

    for (const auto& port : ports) {
        if (!PortBindingManager::isBounded(port))
            continue;

        qDebug() << "Port with ID" << port->getPortNumber();
        PortBindingManager::printBindingsForaPort(port);
    }

    qDebug() << "--------------------------------------\n";
}

void testTopology(const QString& topologyName,
                  UT::TopologyType topologyType,
                  int numberOfNodes,
                  uint16_t asID)
{
    qDebug() << "Testing" << topologyName << "Topology...";

    auto routers = TopologyBuilder::buildTopology(numberOfNodes, topologyType, asID);

    for (const auto& router : routers) {
        qDebug() << "\nRouter" << router->getId() << "with IP" << router->getIP()->toString();
        printBoundedPorts(router->getPorts());
    }

    qDebug() << "\n" << topologyName << "Topology Test Complete!";
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    // Test different topologies
    testTopology("Mesh", UT::TopologyType::Mesh, 9, 1);
    testTopology("Torus", UT::TopologyType::Torus, 9, 2);
    testTopology("Ring-Star", UT::TopologyType::RingStar, 7, 2);

    return a.exec();
}
```

**What is being tested**:
1. **Topology Generation**: We test the creation of topologies (`Mesh`, `Torus`, `Ring-Star`) using `TopologyBuilder`.
2. **Port Binding**: We ensure that ports in each topology are properly bound and display their information.

**Desired Output**:
1. **Topology Test**: Displays router details including router ID, IP address, and bounded ports for each topology.
   

### **Testing Config Reader**

```cpp
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QString>
#include "Network/Network.h"
#include "Utils/ConfigReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString projectDir = QString(PROJECT_DIR_PATH);
    QString configFilePath = QDir(projectDir).filePath("assets/config.json");

    ConfigReader::readNetworkConfig(configFilePath);

    // SimulationConfig
    qDebug() << "SimulationConfig:";
    qDebug() << "Duration (ms):" << Network::simulationConfig.simulationDurationMs;
    qDebug() << "Cycle Duration (ms):" << Network::simulationConfig.cycleDurationMs;
    qDebug() << "TTL:" << Network::simulationConfig.TTL;
    qDebug() << "Packets per Simulation:" << Network::simulationConfig.packetsPerSimulation;
    qDebug() << "Statistical Distribution:" << Network::simulationConfig.statisticalDistribution;
    qDebug() << "Router Buffer Size:" << Network::simulationConfig.routerBufferSize;
    qDebug() << "Router Port Count:" << Network::simulationConfig.router

PortCount;
    qDebug() << "Routing Protocol:" << Network::simulationConfig.routingProtocol;
    qDebug() << "Routing Table Update Interval:"
             << Network::simulationConfig.routingTableUpdateInterval;
    qDebug() << "Routing Per Port:" << Network::simulationConfig.routingPerPort;
    qDebug() << "Routing Table Size:" << Network::simulationConfig.routingTableSize;
    qDebug() << "Routing Packets per Port Cycle:"
             << Network::simulationConfig.routingPacketsPerPortCycle;

    // Autonomous Systems
    qDebug() << "\nAutonomous Systems:";
    for (const auto *asSystem : Network::autonomousSystems) {
        qDebug() << "AS ID:" << asSystem->id;
        QString topologyTypeStr;
        switch (asSystem->topologyType) {
        case UT::TopologyType::Mesh:
            topologyTypeStr = "Mesh";
            break;
        case UT::TopologyType::RingStar:
            topologyTypeStr = "RingStar";
            break;
        case UT::TopologyType::Torus:
            topologyTypeStr = "Torus";
            break;
        }

        qDebug() << "Topology Type:" << topologyTypeStr;
        qDebug() << "Node Count:" << asSystem->nodeCount;
        qDebug() << "AS Gateways Count:" << asSystem->asGateways.size();
        qDebug() << "User Gateways Count:" << asSystem->userGateways.size();
        qDebug() << "DHCP Server Set:" << (asSystem->dhcpServer ? "Yes" : "No");
        qDebug() << "Broken Routers Count:" << asSystem->brokenRouters.size();
        qDebug() << "Connections Count:" << asSystem->connections.size();
    }

    return 0;
}
```

**What is being tested**:
1. **Configuration Reading**: We test the `ConfigReader` class to ensure it can read configuration values from a JSON file.
2. **Simulation Config**: We display simulation parameters like duration, cycle time, TTL, etc.
3. **Autonomous System Details**: The autonomous system data (topology, gateway count, router info) is displayed.

**Desired Output**:
1. **SimulationConfig**: Displays configuration parameters.
2. **Autonomous Systems**: Displays details of the autonomous systems as configured in the `config.json`.

#### **Output**
![Config Test](./imgs/ConfigReaderTest_P2.png)



### **Network Integration Test**

```cpp
#include <QCoreApplication>
#include "Network/Network.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Network network;
    network.run();

    return a.exec();
}
```

**What is being tested**:
1. **System Integration**: This test ensures that the combination of all the network-related classes (routers, PCs, event coordinators, etc.) can function together correctly within the simulation. It verifies that these components interact properly, with routers and PCs able to send and receive packets through the network.
  
2. **Multi-threading**: The test also validates that the system properly handles different components (such as routers and PCs) running in different threads. This is crucial for simulating a real network environment, where various devices must operate concurrently.

3. **Event Coordination**: It checks whether the `EventCoordinator` class is effectively managing events in the simulation. This includes ensuring that the simulation steps are properly synchronized and handled in different threads, ensuring smooth operation.

4. **Packet Transmission**: The test verifies that packets can be sent and received between routers and PCs. The flow of data is checked to confirm that communication between network devices occurs as expected within the simulation.

5. **Logging and Monitoring**: Logs are enabled to track the activity of routers and PCs, specifically checking that:
   - Routers and PCs are executing in separate threads.
   - Packet transmissions occur as expected without blocking or delays.
   - Events such as packet sends and receives are properly logged, allowing for real-time monitoring of the simulation.

**Desired Output**:
1. **Log Messages**: The logs should show that routers and PCs are running in separate threads, with thread identifiers displayed alongside the log messages.
  
2. **Packet Transmission**: The logs should also confirm that packets are being successfully transmitted between the devices. This will be evident from messages indicating packet sending and receiving events.

3. **Event Coordination**: The output should show synchronized event handling, with events being executed in the correct order according to the simulation's timeline.

4. **Error Handling**: In case of any issues, the logs should help identify misconfigurations, such as incorrect event timings, thread synchronization issues, or failed packet transmissions.

#### **Output**
![Network Test](./imgs/NetworkTest_P2.png)

We also used QT creator's debugging tools to check if the threads are running correctly. The following screenshot show the threads running in the Network Integration Test:

![Thread Test](./imgs/ThreadTest_P2.png)




## Analysis

We ran the simulator ten times for each of the RIP and OSPF protocols and recorded the results for Packet Loss, Hop Count Average, Total Waiting Cycles, Max Waiting Cycles, and Average Waiting Cycles. We also paid attention to routers that were unused and those that were most utilized and analyzed them. To improve the analysis, we calculated the average of the results from these ten runs for each protocol.

This is a sample of what we checked in each simulation:

```
Enter command for analysis (or 'Exit' to end):
Packet-loss
Packet loss percentage: 46.6833 %  Total packets: 6000
Enter command for analysis (or 'Exit' to end):
Hop-count-avg
Average hop count: 5.06846
Enter command for analysis (or 'Exit' to end):
Waiting-cycles-stat
Waiting Cycles Stats: Total: 6343 Min: 0 Max: 20 Avg: 1.98281
Enter command for analysis (or 'Exit' to end):
Poor-routers
Routers not used during simulation: QSet("192.168.100.7", "192.168.100.13", "192
.168.200.23", "192.168.100.5", "192.168.100.14", "192.168.100.9")
Enter command for analysis (or 'Exit' to end):
Top-routers
Top routers by packet deliveries:
Router: "192.168.200.20" Packets: 1574        
Router: "192.168.100.4" Packets: 1552
Router: "192.168.100.2" Packets: 1456
Router: "192.168.100.3" Packets: 1450
```

### OSPF

After averaging the results and some simple analysis we get this results:

```
Average Packet loss percentage: 41.8 %  Total packets: 6000
Average of Average hop count: 5.24
Average of waiting Cycles Stats: Total: 93031 Min: 0 Max: 19.6 Avg: 2.664
```

In this protocol, the number of unused routers in all our results was consistently 6, although they were different routers in each case. Additionally, the routers with the highest utilization were always the same 4 routers, with IDs 2, 3, 4, and 20. The reason these routers are the most utilized is that, in most scenarios and routes, when there is a choice between two routers leading to the same destination with equal cost, these routers are given priority. As a result, they end up transmitting the highest number of packets.

### RIP

```
Average Packet loss percentage: 39.9576 %  Total packets: 6000
Average of Average hop count: 5.3038
Average of waiting Cycles Stats: Total: 92536 Min: 0 Max: 20.3 Avg: 2.569
```

In this protocol, the number of unused routers in all our results was consistently 4, although they were different routers in each case. Similarly, the routers with the highest utilization were always the same 4 routers, with IDs 2, 3, 4, and 20. The reason for this behavior in this protocol is similar to the explanation above: in most scenarios and routes, when there is a choice between two routers leading to the same destination with equal cost, these routers are prioritized, resulting in them transmitting the highest number of packets.


## Comparison

### **1. Packet Loss**
- **RIP**: Average packet loss is **39.96%**.  
- **OSPF**: Average packet loss is **41.8%**.  
- **Comparison**:  
  - RIP has a slightly **lower packet loss** compared to OSPF, indicating that RIP is marginally better at successfully delivering packets.  
  - The difference, however, is minor (1.84%), and additional tests with different network conditions might show variability.


### **2. Hop Count (Average)**  
- **RIP**: Average hop count is **5.3038**.  
- **OSPF**: Average hop count is **5.24**.  
- **Comparison**:  
  - OSPF has a **slightly lower average hop count**, meaning packets travel through fewer routers on average.  
  - This might suggest that OSPF's routing decisions are slightly more efficient in terms of path selection compared to RIP.  


### **3. Waiting Cycles (Latency)**
- **RIP**:  
  - **Total waiting cycles**: **92,536**  
  - **Maximum waiting cycles**: **20.3**  
  - **Average waiting cycles**: **2.569**  
- **OSPF**:  
  - **Total waiting cycles**: **93,031**  
  - **Maximum waiting cycles**: **19.6**  
  - **Average waiting cycles**: **2.664**  
- **Comparison**:  
  - **RIP** has slightly **lower total waiting cycles** and **lower average waiting cycles**, which indicates it might process packets with less overall latency.  
  - However, **OSPF** has a marginally **lower maximum waiting cycle** (19.6 vs. 20.3), suggesting that OSPF might be better at avoiding extreme delays in some scenarios.  


### **4. Router Utilization**  
- **RIP**:  
  - **Unused Routers**: Consistently **4**, but different ones across runs.  
  - **Most Utilized Routers**: Routers with IDs **2, 3, 4, and 20**.  
- **OSPF**:  
  - **Unused Routers**: Consistently **6**, but different ones across runs.  
  - **Most Utilized Routers**: Same routers as in RIP: **2, 3, 4, and 20**.  
- **Comparison**:  
  - RIP appears to utilize more routers on average since fewer are consistently unused.  
  - In both protocols, the same 4 routers are highly utilized. This suggests a **bottleneck in the network topology**, where certain routers are preferred due to equal-cost paths or routing algorithms prioritizing them.



| **Metric**              | **RIP**                      | **OSPF**                     | **Better Protocol**  |
|--------------------------|------------------------------|------------------------------|----------------------|
| **Packet Loss**          | 39.96%                      | 41.8%                       | **RIP**              |
| **Hop Count (Avg)**      | 5.3038                      | 5.24                        | **OSPF**             |
| **Avg Waiting Cycles**   | 2.569                       | 2.664                       | **RIP**              |
| **Max Waiting Cycles**   | 20.3                        | 19.6                        | **OSPF**             |
| **Router Utilization**   | Fewer unused routers (4)     | More unused routers (6)      | **RIP**              |


### **Conclusion**  
- **RIP** performs better in terms of **packet delivery success**, **average waiting cycles**, and **router utilization**, making it more effective for reducing network bottlenecks in this simulation.  
- **OSPF**, however, shows advantages in **path efficiency (lower hop count)** and slightly **lower maximum latency**, indicating better path optimization and consistent performance in extreme conditions.  
- The choice between the two protocols would depend on specific network requirements.  
  - For networks where **packet delivery and utilization** are critical, **RIP** might be preferable.  
  - For networks prioritizing **path efficiency and handling extreme delays**, **OSPF** could be better.  
