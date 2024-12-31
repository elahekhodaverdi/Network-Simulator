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

---

This report summarizes the `RIP` class, outlining its key fields, signals, and methods while emphasizing its role in implementing RIP within the simulation.