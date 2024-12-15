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
