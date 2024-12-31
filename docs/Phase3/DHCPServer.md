## DHCPServer Class

The `DHCPServer` class simulates the behavior of a Dynamic Host Configuration Protocol (DHCP) server, responsible for assigning IP addresses dynamically and responding to client requests within the simulation framework.

### Fields

- **`m_ipRange`:**  
  Represents the base range of IP addresses that the server manages and assigns to clients.

- **`sentOffers`:**  
  A list of client IDs for which IP offers have already been sent.

- **`sentAcks`:**  
  A list of acknowledged IP addresses already assigned to clients.

### Signals

- **`newPacket`:**  
  Emitted when a new packet is generated, along with the triggering port.

### Methods Explanations

1. **`DHCPServer` (Constructor):**  
   Initializes the server with a specified IP range.

2. **`~DHCPServer` (Destructor):**  
   Cleans up the server upon destruction.

3. **`getIP`:**  
   Returns an IP address for a given client ID based on the server's IP range.

4. **`handleDiscoveryPacket`:**  
   Processes DHCP Discovery packets. If a client ID has not yet been offered an IP, generates a DHCP Offer packet and emits it.

5. **`handleRequestPacket`:**  
   Processes DHCP Request packets. If a client has not already been acknowledged, generates a DHCP Acknowledge packet and emits it.
