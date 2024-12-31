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
