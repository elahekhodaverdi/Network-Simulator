

### MACAddress Class
The `MACAddress` class represents and validates MAC addresses, providing functionality to store, convert, and validate MAC addresses. It supports initialization from both a string and a byte array.

### Fields
- **`m_address`**: stores the six bytes of the MAC address.


### Method Prototypes
- **`MACAddress(std::array<uint8_t, 6> address)`**
  Constructor that initializes the MAC address from a 6-byte array.

- **`MACAddress(QString address)`**  
  Constructor that initializes the MAC address from a string. If the input is invalid, the address is set to all zeros.

- **`QString toString() const`**  
  Converts the MAC address to its string representation in the format `XX:XX:XX:XX:XX:XX`.

- **`bool isValid(const QString& address)`**  
  Validates the format of a MAC address string.

- **`bool operator==(const MACAddress& other) const`**  
  Compares two `MACAddress` objects for equality.


### Method Explanations
1. **Constructors**
    - **with array**:
        - This constructor directly initializes the `m_address` field using a 6-byte array.
    -  **with QString**  
        - Parses the input string if it follows the format `XX:XX:XX:XX:XX:XX` using the `isValid()` method.
        - If valid, it converts the string to its 6-byte representation. Invalid inputs result in a warning, and the address is set to all zeros.

3. **`toString()`**  
   - Generates a colon-separated hexadecimal string representation of the MAC address.

4. **`isValid()`**  
   - Uses a regular expression to validate the format of a MAC address string.

5. **Equality Operator (`==`)**  
   - Compares each byte of the `m_address` field between two `MACAddress` objects.
