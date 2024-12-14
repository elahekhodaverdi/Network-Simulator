## Packet Class

The `Packet` class represents a network packet and provides methods to manipulate its properties. It encapsulates various attributes such as packet type, payload, sequence number, waiting and total cycles, and the path it traverses. The class also integrates headers such as `DataLinkHeader` and optionally supports `TCPHeader`.

### Fields

- **`m_packetType`**: An instance of `UT::PacketType` that specifies the type of the packet.
- **`m_payload`**: A `QByteArray` storing the packet's data payload.
- **`m_sequenceNumber`**: A 32-bit unsigned integer representing the packet's sequence number.
- **`m_waitingCycles`**: A 32-bit unsigned integer indicating how many cycles the packet has been waiting.
- **`m_totalCycles`**: A 32-bit unsigned integer tracking the total number of cycles the packet has been in the system.
- **`m_path`**: A `QList<QString>` representing the IP path the packet has traversed.
- **`m_dataLinkHeader`**: An instance of `DataLinkHeader` representing the data link layer header associated with the packet.
- **`m_tcpHeader`**: An optional `TCPHeader` instance providing support for TCP-related information, if applicable.
- **`m_controlType`**: An optional `UT::PacketControlType` for specifying control-related metadata for the packet.

### Constructor

1. **`Packet(DataLinkHeader dataLinkHeader, QObject* parent = nullptr)`**
   - Initializes a `Packet` object with a given `DataLinkHeader`.
   - Sets `m_totalCycles` and `m_waitingCycles` to zero.

### Method Explanations

1. **`addToPath(QString ip)`**
   - Adds an IP address to the packet's path.
   - Appends the provided IP string to `m_path`.

2. **`incWaitingCycles()`**
   - Increments the `m_waitingCycles` field by one.

3. **`incTotalCycles()`**
   - Increments the `m_totalCycles` field by one.

4. **Setters**
    - Each of the fields has a corresponding setter method.

5. **Getters**
    - Each of the fields has a corresponding getter method.

### Destructor

- **`~Packet()`**
   - Ensures proper cleanup of the `Packet` instance when it goes out of scope.
  
### Future Expansion
  - IPHeader integration for network layer information.
  
  