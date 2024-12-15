## PortBindingManager Class

The `PortBindingManager` class is responsible for managing the bindings between network ports. A binding refers to the connection between two ports, enabling communication between them. This class provides functionality to bind, unbind, and check the status of port connections, supporting the simulation of a network environment where ports can exchange data.

### Fields

1. **`bindings`**:  
   - A static `QMap` that stores the relationships between ports.  
   - The keys are `PortPtr_t` objects representing the ports, and the values are `QList<PortPtr_t>` objects that contain the list of ports that are bound to the key port.

### Methods Explanation

1. **Constructor**:  
   - **`PortBindingManager(QObject *parent = nullptr)`**:  
     Initializes the `PortBindingManager` object. The constructor does not take any specific initialization beyond the `QObject` parent.

2. **`printBindings()`**:  
   - This method prints the current bindings of all ports in the system.  
   - It iterates over the `bindings` map and outputs the port numbers of the bound ports to the console, showing which ports are connected.

3. **`printBindingsForaPort(PortPtr_t port)`**:  
   - This method prints the bindings associated with a specific port.  
   - It checks whether the port exists in the `bindings` map and then outputs the connected ports and their router IPs.

4. **`bind(const PortPtr_t &port1, const PortPtr_t &port2)`**:  
   - This method binds two ports together, enabling them to communicate with each other.  
   - It ensures that a port cannot be bound to itself and that the ports are not already bound.  
   - Once the binding is established, the method connects the `packetSent` signal of both ports to the `receivePacket` slot of the other port, allowing for packet transmission between them.

5. **`unbind(const PortPtr_t &port)`**:  
   - This method unbinds all ports that are bound to a given port.  
   - It removes the port from the `bindings` map and disconnects any established connections between the given port and its bound ports.

6. **`unbind(const PortPtr_t &port1, const PortPtr_t &port2)`**:  
   - This method unbinds two specific ports that are connected.  
   - It removes the connection from the `bindings` map and disconnects the signals and slots that were established for packet transmission between the ports.

7. **`isBounded(const PortPtr_t &port)`**:  
   - This method checks if a specific port has any active bindings.  
   - It returns `true` if the port is bound to any other ports, and `false` otherwise.

### Signals

- **`void bindingChanged(const QString &router1, uint8_t port1, const QString &router2, uint8_t port2, bool bind)`**:  
   - This signal (currently commented out) would be emitted whenever the binding status between two ports changes. It could be used for debugging or logging purposes.
