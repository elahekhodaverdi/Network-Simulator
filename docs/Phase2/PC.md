## PC Class

The `PC` class models a personal computer within a simulated network environment. It extends the `Node` class, inheriting its core functionalities while implementing additional features for sending and receiving packets. The class utilizes Qt's threading and signal-slot mechanisms for asynchronous operations and efficient network simulation.

### Fields

1. **`m_gateway`**:  
   - Represents the gateway port through which the PC communicates with the network.  
   - This is a dynamically allocated `PortPtr_t` object initialized during the construction of the `PC`.

### Methods Explanation

1. **Constructors**:  
   The `PC` class provides two constructors for initialization:  
   - **`PC(int id, MACAddress macAddress, QObject *parent = nullptr)`**:  
     Initializes the PC with a specific MAC address and ID, creating a gateway for network communication.  
     Starts the thread for the object and sets up the gateway.  
   - **`PC(int id, QObject *parent = nullptr)`**:  
     Initializes the PC with a randomly generated MAC address, ID, and gateway.  
     Also starts the thread and sets up the gateway.

2. **`~PC()`**:  
   - Destructor for the `PC` class.  
   - Ensures proper cleanup by unbinding the gateway port from the `PortBindingManager`.  
   - Disconnects any signal-slot connections related to the gateway and packet communication.

3. **`void sendPacket(QVector<QSharedPointer<PC>> selectedPCs)`**:  
   - Sends a packet to one of the selected PCs if the current PC is part of the selection.  
   - Generates a new packet using `createNewPacket()` and emits the `newPacket` signal to initiate transmission.

4. **`PCPtr_t chooseRandomPC()`**:  
   - Randomly selects a PC from the network that is different from the current PC.  
   - Utilizes `QRandomGenerator` to ensure a fair selection.

5. **`PacketPtr_t createNewPacket()`**:  
   - Creates a new data packet destined for a randomly selected PC.  
   - Sets the source and destination MAC and IP addresses within the packet's headers.  
   - Populates the payload with a message and calculates the IP header checksum.  
   - Returns the constructed packet.  

6. **`void receivePacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - Handles incoming packets through the gateway.  
   - Logs the packet's content and identifies the receiving PC for debugging purposes.

7. **`void setIP(IPv4Ptr_t ip)`**:  
   - Overrides the `Node` class's pure virtual method to set the IP address of the PC.  
   - Updates the router IP address associated with the gateway.

8. **`PortPtr_t gateway()`**:  
   - Returns the gateway port associated with the PC.  
   - Provides access to the port for further configurations or queries.

9. **`void setupGateway()`**:  
   - Configures the gateway by connecting the `packetReceived` signal of the gateway port to the `receivePacket` slot of the PC.  
   - Connects the `newPacket` signal of the PC to the `sendPacket` slot of the gateway port.  
   - Ensures the router IP of the gateway is updated.

### Signals

1. **`void newPacket(const PacketPtr_t &data, uint8_t port_number)`**:  
   - Emitted when a new packet is ready to be sent.  
   - Enables seamless communication with the gateway.  