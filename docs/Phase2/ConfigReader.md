

## **ConfigReader Class**

The **ConfigReader** class is a utility designed to read and parse configuration files, specifically for setting up network simulations. It processes JSON-formatted files to extract simulation settings and build autonomous systems.

### Fields

All methods in this class are static, ensuring they are accessible without instantiating the class.

### Method Explanations

1. **`readJson`**
    - Reads a JSON file and validates its format.

2. **`readNetworkConfig`**
    - Main entry point for reading and processing the network configuration.
    - **Process**:
      - Reads the JSON object using `readJson`.
      - Splits general settings and autonomous system definitions.
      - Extracts simulation settings via `parseSimulationConfig`.
      - Processes the autonomous systems using `parseAutonomousSystems`.

3. **`parseSimulationConfig`**
    - Parses general simulation settings.
    - **Key Parameters**:
      - **Simulation Duration**: Overall runtime for the simulation.
      - **Cycle Duration**: Interval for each simulation cycle.
      - **TTL (Time-To-Live)**: Duration for packets.
      - **Packets Per Simulation**: Maximum number of packets.
      - **Statistical Distribution**: The distribution model for simulation.
      - **Router Buffer Size**: Buffer capacity of routers.
      - **Router Port Count**: Number of ports per router.
      - **Routing Protocol**: The protocol used for routing.
      - **Routing Table Size**: Maximum size of routing tables.
      - **Routing Packets Per Port Cycle**: Number of packets routed per cycle.
      - **Routing Per Port**: Boolean to enable/disable routing on a per-port basis.
      - **Routing Table Update Interval**: Updates in seconds (or infinite).

4. **`parseAutonomousSystems`**
    - Constructs `AutonomousSystem` instances based on the configuration.
    - **Steps**:
      - Iterates over an array of system definitions.
      - Extracts topology type using `stringToTopologyType`.
      - Builds and appends the system to the `Network::autonomousSystems` list.

5. **Utility Functions**
    - **stringToTopologyType**:
      - Converts string representations (`"Mesh"`, `"RingStar"`, `"Torus"`) into enumeration values.
    - **parseTopologyTypes**:
      - Splits compound topology type strings into a list of enumerated types.
