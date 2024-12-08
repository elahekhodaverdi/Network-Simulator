
### MACAddressGenerator Class

The `MACAddressGenerator` class provides functionality to generate and manage random MAC addresses. It ensures uniqueness by keeping track of previously assigned addresses and generates new ones based on a predefined Organizationally Unique Identifier (OUI).

### Fields

- **`m_assignedMACs`**: A static `QList<MACAddress>` that stores previously assigned MAC addresses to avoid duplication.
- **`m_OUIBytes`**: A static `std::array<uint8_t, 3>` representing the Organizationally Unique Identifier (OUI) portion of a MAC address. This is randomly initialized at runtime.

### Method Explanations

1. **Deleted Constructor**
   - The class is designed as a utility class with only static members and methods. The constructor is explicitly deleted to prevent instantiation.

2. **`generateRandomMAC()`**
   - Generates a MAC address by combining:
     - The predefined OUI bytes stored in `m_OUIBytes`.
     - Three randomly generated NIC-specific bytes.
   - Combines these bytes into a full 6-byte array and returns it as a `MACAddress`.

3. **`isMACAssigned()`**
   - Checks if the given MAC address exists in the `m_assignedMACs` list.
   - Returns `true` if the MAC address has been assigned, `false` otherwise.

4. **`getRandomMAC()`**
   - Calls `generateRandomMAC()` to create a new MAC address.
   - Ensures uniqueness by checking against `m_assignedMACs` using `isMACAssigned()`.
   - If the generated MAC address is already assigned, the method retries until a unique address is generated.
   - The unique address is added to `m_assignedMACs` and returned.

