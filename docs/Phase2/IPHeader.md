## IPHeader Class

The `IPHeader` class represents the structure and functionality of an IPv4 header, as defined in the Internet Protocol (IP). It provides methods to manage and manipulate various header fields, including version, length, type of service, and other relevant parameters. It also includes functions for setting and retrieving these fields, as well as calculating the header checksum.

### Fields

- **`m_versionHeaderLength`**: Stores the version and header length, combined in one byte.
- **`m_typeOfService`**: Stores the type of service field, typically used for routing priority and delay.
- **`m_totalLength`**: Stores the total length of the IP packet, including both header and data.
- **`m_identification`**: Stores the identification field for uniquely identifying fragmented packets.
- **`m_flagsFragmentOffset`**: Stores flags and fragment offset used for packet fragmentation.
- **`m_ttl`**: Stores the time-to-live value, used to limit the lifetime of a packet.
- **`m_protocol`**: Stores the protocol field, identifying the protocol used in the data portion of the IP packet.
- **`m_headerChecksum`**: Stores the checksum of the header for error-checking.
- **`m_sourceIp`**: Stores the source IP address.
- **`m_destIp`**: Stores the destination IP address.

### Method Explanations

1. **Constructors**
    - **`IPHeader(QObject *parent)`**:
        - Initializes the fields to default values. The `ttl` field is set using a value from the `SimulationConfig::TTL`.

2. **Setters**
    - Each of the fields has a corresponding setter method.

3. **Getters**
    - Each of the fields has a corresponding getter method.

4. **`calculateHeaderChecksum(const QByteArray &data)`**
    - Calculates the header checksum using a method based on the data size and values. It processes the data in 16-bit chunks, adds them together, and performs a final sum adjustment to generate the checksum.
