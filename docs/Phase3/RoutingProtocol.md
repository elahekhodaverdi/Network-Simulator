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
