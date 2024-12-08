## TCPHeader Class

The `TCPHeader` class represents the header of a TCP packet.

### Fields

- **`m_sourcePort`**: Stores the source port number (16 bits).
- **`m_destPort`**: Stores the destination port number (16 bits).
- **`m_sequenceNumber`**: Stores the sequence number of the packet (32 bits).
- **`m_acknowledgementNumber`**: Stores the acknowledgment number (32 bits).
- **`m_flags`**: Represents the control flags (8 bits).
- **`m_dataOffset`**: Indicates the size of the TCP header in 32-bit words (8 bits).
- **`m_checksum`**: Stores the checksum for error-checking purposes (16 bits).
- **`m_windowSize`**: Represents the size of the receive window (16 bits).
- **`m_urgentPointer`**: Points to the location of urgent data within the packet (16 bits).

### Methods Explanations

1. **Constructor:**
   - `TCPHeader(QObject *parent = nullptr)`: Initializes the class and sets the default checksum to `0`.

2. **Setters:**
   - Each of the fields has a corresponding setter method, such as `setSourcePort()`, `setDestPort()`, `setSequenceNumber()`, and so on.

3. **Getters:**
   - Each of the fields has a corresponding getter method, such as `sourcePort()`, `destPort()`, `sequenceNumber()`, and so on, to retrieve the value of the field.

4. **`calculateChecksum` Method:**

    - Calculates the checksum for the TCP header using the pseudo-header (source and destination IPs) and the actual header data.
    - Uses a helper function, onesComplementSum, to compute the one's complement sum of the data.

5. **`onesComplementSum` Helper Function:**
    - Computes the one's complement sum for a given byte array.
