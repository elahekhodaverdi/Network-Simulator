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
