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
