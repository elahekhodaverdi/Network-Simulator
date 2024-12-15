
## **Network Class**

The `Network` class models the overall simulation network, managing autonomous systems (AS), events coordination, and other components.

### **Fields**
1. **Static Fields:**
   - `PCs`: List of all PCs in the network.
   - `simulationConfig`: The simulation configuration object.
   - `autonomousSystems`: List of autonomous systems in the network.
   - `eventsCoordinator`: Pointer to the events coordinator.

### **Methods Explanation**

1. **Constructors**
    - **Default Constructor:**
        - Initializes the `eventsCoordinator` singleton.

        ```cpp
        Network::Network()
        {
            eventsCoordinator = EventsCoordinator::instance();
        }
        ```

    - **Destructor:**
        - Releases the events coordinator.

        ```cpp
        Network::~Network()
        {
            eventsCoordinator->release();
        }
        ```

2. **`findASById`**

    - Searches for and retrieves an autonomous system by its ID.
    - Returns `nullptr` if the AS is not found.

    ```cpp
    AutonomousSystemPtr_t Network::findASById(int id);
    ```

3. **`run`**

    - Initializes and runs the network simulation:
        - Reads the configuration from a JSON file.
        - Starts the simulation with parameters from `SimulationConfig`.

