## Network Class

The `Network` class models the simulation network, managing its autonomous systems (AS), PCs, and providing utility functions for router statistics.

### Fields

1. **Static Fields**
   - **`PCs`**: A `QList` containing pointers to all PCs in the network.
   - **`autonomousSystems`**: A `QList` containing pointers to all autonomous systems in the network.

### **Methods Explanation**

#### **Constructors**

1. **Default Constructor**

2. **Destructor**

#### **Static Methods**

1. **`findASById`**
   - Searches for and retrieves an autonomous system by its ID.
   - Returns `nullptr` if no matching autonomous system is found.

2. **`getAllRoutersIPs`**
   - Collects and returns a list of all router IPs across the autonomous systems.

   ```cpp
   QList<IPv4Ptr_t> Network::getAllRoutersIPs()
   {
       QList<IPv4Ptr_t> allRoutersIPs;
       for (AutonomousSystemPtr_t AS : autonomousSystems) {
           allRoutersIPs.append(AS->getAllRoutersIPs());
       }
       return allRoutersIPs;
   }
   ```

3. **`numOfRouters`**

Calculates and returns the total number of routers in all autonomous systems.

4. **`numOfBrokenRouters`**

Calculates and returns the total number of broken routers across all autonomous systems.
