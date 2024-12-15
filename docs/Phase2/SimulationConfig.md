
## **SimulationConfig Class**

The `SimulationConfig` class serves as a centralized configuration system for simulation parameters. It defines static variables that represent key settings required to manage simulation behavior.

### **Fields**

1. **Static Fields:**
   - `simulationDurationMs`: The total duration of the simulation in milliseconds.
   - `cycleDurationMs`: Duration of a single simulation cycle in milliseconds.
   - `TTL`: Default time-to-live value for packets.
   - `packetsPerSimulation`: Number of packets processed during the simulation.
   - `statisticalDistribution`: Distribution type for simulation.
   - `routerBufferSize`: Buffer size of routers.
   - `routerPortCount`: Number of ports in each router.
   - `routingProtocol`: Protocol used for routing.
   - `routingTableUpdateInterval`: Interval for routing table updates in milliseconds.
   - `routingPerPort`: Whether routing is performed per port.
   - `routingTableSize`: Maximum size of routing tables.
   - `routingPacketsPerPortCycle`: Packets processed per port per cycle.

2. **Private Methods:**
   - `convertTimeToMs(const QString &timeString)`: Converts a time string to milliseconds.

### **Methods Explanation**

1. **Constructors**
    - **Default Constructor:**

        Initializes an empty configuration with default values.

        ```cpp
        SimulationConfig::SimulationConfig() {}
        ```

    - **Parameterized Constructor:**

        Populates simulation settings using provided arguments, converting time values from string to milliseconds.

        ```cpp
        SimulationConfig(const QString &simDuration, const QString &cycleDuration, ...);
        ```

2. **`convertTimeToMs`**

    - Converts a time string into milliseconds using regex matching.
        - Supports formats like `"10s"` or `"500ms"`.
        - Returns `0` for invalid formats.
