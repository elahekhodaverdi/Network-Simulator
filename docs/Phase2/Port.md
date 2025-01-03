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