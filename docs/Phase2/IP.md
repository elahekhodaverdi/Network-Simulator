
## **IP Class**

The **IP** class template provides a structured representation of IPv4 and IPv6 addresses, along with utility methods for manipulation and validation. It inherits from the `AbstractIP` base class.

### **AbstractIP Base Class**

**Fields**

- **m_ipValue**: A `uint64_t` value representing the raw IP address.

**Constructor**

- Default: Initializes `m_ipValue` to `0`.
- Copy Constructor.



### **IP (Specialization for IPv4)**

 **Fields**
- Inherits `m_ipValue` from `AbstractIP`.

 **Constructors**
- **Default**: Sets the IP to the maximum `uint64_t` value.
- **From String**: Accepts an IPv4 string and parses it into `m_ipValue`.
- **From Integer**: Accepts a `uint64_t` value directly.
- **Copy Constructor**: Creates a copy of an existing IPv4 instance.

 **Methods**
1. **toString**:
   - Converts `m_ipValue` into  `"X.X.X.X"` format.
   - Uses bitwise shifts and masks to extract octets.
2. **toValue**:
   - Returns the `uint64_t` representation of the IP.
3. **fromString**:
   - Parses an IPv4 string and updates `m_ipValue`.
   - Validates the format using `validateIPv4String`.
4. **validateIPv4String**:
   - Checks if the input string conforms to the IPv4 standard.
   - Ensures there are four octets, each between `0` and `255`.

5. **Operators**
    - **Equality Operator**: Compares the `m_ipValue` of two IPv4 addresses.

