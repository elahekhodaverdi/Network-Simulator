
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
