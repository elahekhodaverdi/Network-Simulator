
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
