## PC Class

The `PC` class models a personal computer within a simulated network environment. It extends the `Node` class, inheriting its core functionalities while implementing additional features for sending and receiving packets.

### Fields

1. **`m_gateway`**:
   - Represents the gateway port through which the PC communicates with the network.
   - This is a dynamically allocated `PortPtr_t` object initialized during the construction of the `PC`.

2. **`shouldSendNewPacket`**:
   - A boolean flag indicating whether the PC should send a new packet in the current simulation phase.
   - Default value is `false`.

### Methods Explanation

1. **Constructors**:
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

3. **`setIP`**:
   - Overrides the `Node` class's pure virtual method to set the IP address of the PC.

4. **`gateway`**:
   - Returns the gateway port associated with the PC.

5. **`setupGateway`**:
   - Configures the gateway by connecting the `packetReceived` signal of the gateway port to the `receivePacket` slot of the PC.
   - Connects the `newPacket` signal of the PC to the `sendPacket` slot of the gateway port.
   - Ensures the router IP of the gateway is updated.

6. **`handleNewTick`**:
   - Handles updates for the current simulation phase.
   - Checks and processes phase transitions, and sends a new packet if the `shouldSendNewPacket` flag is set.

7. **`setShouldSendPacket(QVector<QSharedPointer<PC>> selectedPCs)`**:
   - Updates the `shouldSendNewPacket` flag based on whether the current PC is among the selected PCs.

8. **`sendNewPacket()`**:
   - Generates and sends a new data packet through the gateway.

9. **`chooseRandomPC()`**:
   - Randomly selects a PC from the network that is different from the current PC.
   - Ensures a fair selection using `QRandomGenerator`.

10. **`createNewPacket()`**:
    - Creates a new data packet destined for a randomly selected PC.
    - Populates the MAC, IP headers, payload, and checksum for the packet.
    - Returns the constructed packet.

11. **`handleControlPacket(const PacketPtr_t &data)`**:
    - Processes control packets such as DHCP offers, acknowledgments, and requests.
    - Handles specific actions based on the control type.

12. **`receivePacket(const PacketPtr_t &data, uint8_t portNumber)`**:
    - Handles incoming packets through the gateway.
    - Emits the `packetReceived` signal for further processing.
    - Identifies and processes control packets if applicable.

13. **`broadcastPacket(const PacketPtr_t &packet, PortPtr_t triggeringPort)`**:
    - Sends a broadcast packet through the gateway, avoiding retransmission from the triggering port.

14. **`addPacketForBroadcast(const PacketPtr_t &packet, PortPtr_t triggeringPort)`**:
    - Adds a packet for broadcasting to the network.
    - Delegates the actual transmission to the `broadcastPacket` method.

15. **`handlePhaseChange(const UT::Phase nextPhase)`**:
    - Updates the simulation phase and performs phase-specific actions, such as initiating DHCP discovery.

### Signals

1. **`newPacket(const PacketPtr_t &data, uint8_t port_number)`**:
   - Emitted when a new packet is ready to be sent.

2. **`packetReceived(const PacketPtr_t &data)`**:
   - Emitted when a packet is received by the PC.
