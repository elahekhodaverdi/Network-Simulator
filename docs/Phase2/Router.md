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