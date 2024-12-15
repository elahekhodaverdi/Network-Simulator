## Node Class

The `Node` class represents a network node within a simulated network environment. This class is designed as an abstract base class for Router and PC. Additionally, it incorporates threading capabilities by inheriting from `QThread`, enabling asynchronous operations.

### Fields

1. **`m_id`**:  
   - Stores the unique identifier of the node.  
   - This ID helps in distinguishing nodes within the network.

2. **`m_MACAddress`**:  
   - Holds the MAC address of the node, uniquely identifying it at the data-link layer.  
   - It is generated using `MACAddressGenerator` if not explicitly provided during construction.

3. **`m_IP`**:  
   - Stores the IP address of the node.  
   - The address is dynamically allocated during object creation using the `IPv4Ptr_t::create()` method.

### Methods Explanation

1. **Constructors**:  
   The `Node` class provides two constructors to handle node initialization:
   - **`Node(int id, MACAddress macAddress, QObject *parent = nullptr)`**:  
     Initializes the node with a specified ID and MAC address.
   - **`Node(int id, QObject *parent = nullptr)`**:  
     Automatically generates a random MAC address for the node using `MACAddressGenerator::getRandomMAC()`.

2. **`~Node()`**:  
   - Destructor for the `Node` class.  
   - Ensures the proper cleanup of the dynamically allocated IP address by clearing the `m_IP` pointer if it is not null.

3. **`int getId()`**:  
   - Returns the unique identifier of the node.  

4. **`IPv4Ptr_t getIP()`**:  
   - Provides access to the node's IP address.

5. **`MACAddress getMACAddress()`**:  
   - Returns the MAC address of the node.

6. **`virtual void setIP(IPv4Ptr_t ip) = 0`**:  
   - A pure virtual function that must be implemented by derived classes.  
   - Defines how a node's IP address is set.

7. **`virtual void receivePacket(const PacketPtr_t &data, uint8_t port_number) = 0`**:  
   - A pure virtual slot to handle incoming packets.  
   - Must be implemented by derived classes to define the behavior upon receiving packets.

8. **`void checkCurrentThread()`**:  
   - Verifies if the current thread is the same as the thread of the object.  
   - Outputs debug information about the current thread and the object's thread.  
   - Useful for ensuring thread safety during multi-threaded operations.

### Signals

1. **`void newPacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - A Qt signal emitted when a new packet is created and ready to send. 
