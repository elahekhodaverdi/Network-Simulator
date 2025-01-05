## Analysis

We ran the simulator ten times for each of the RIP and OSPF protocols and recorded the results for Packet Loss, Hop Count Average, Total Waiting Cycles, Max Waiting Cycles, and Average Waiting Cycles. We also paid attention to routers that were unused and those that were most utilized and analyzed them. To improve the analysis, we calculated the average of the results from these ten runs for each protocol.

This is a sample of what we checked in each simulation:

```
Enter command for analysis (or 'Exit' to end):
Packet-loss
Packet loss percentage: 46.6833 %  Total packets: 6000
Enter command for analysis (or 'Exit' to end):
Hop-count-avg
Average hop count: 5.06846
Enter command for analysis (or 'Exit' to end):
Waiting-cycles-stat
Waiting Cycles Stats: Total: 6343 Min: 0 Max: 20 Avg: 1.98281
Enter command for analysis (or 'Exit' to end):
Poor-routers
Routers not used during simulation: QSet("192.168.100.7", "192.168.100.13", "192
.168.200.23", "192.168.100.5", "192.168.100.14", "192.168.100.9")
Enter command for analysis (or 'Exit' to end):
Top-routers
Top routers by packet deliveries:
Router: "192.168.200.20" Packets: 1574        
Router: "192.168.100.4" Packets: 1552
Router: "192.168.100.2" Packets: 1456
Router: "192.168.100.3" Packets: 1450
```

### OSPF

After averaging the results and some simple analysis we get this results:

```
Average Packet loss percentage: 41.8 %  Total packets: 6000
Average of Average hop count: 5.24
Average of waiting Cycles Stats: Total: 93031 Min: 0 Max: 19.6 Avg: 2.664
```

In this protocol, the number of unused routers in all our results was consistently 6, although they were different routers in each case. Additionally, the routers with the highest utilization were always the same 4 routers, with IDs 2, 3, 4, and 20. The reason these routers are the most utilized is that, in most scenarios and routes, when there is a choice between two routers leading to the same destination with equal cost, these routers are given priority. As a result, they end up transmitting the highest number of packets.

### RIP

```
Average Packet loss percentage: 39.9576 %  Total packets: 6000
Average of Average hop count: 5.3038
Average of waiting Cycles Stats: Total: 92536 Min: 0 Max: 20.3 Avg: 2.569
```

In this protocol, the number of unused routers in all our results was consistently 4, although they were different routers in each case. Similarly, the routers with the highest utilization were always the same 4 routers, with IDs 2, 3, 4, and 20. The reason for this behavior in this protocol is similar to the explanation above: in most scenarios and routes, when there is a choice between two routers leading to the same destination with equal cost, these routers are prioritized, resulting in them transmitting the highest number of packets.


## Comparison

### **1. Packet Loss**
- **RIP**: Average packet loss is **39.96%**.  
- **OSPF**: Average packet loss is **41.8%**.  
- **Comparison**:  
  - RIP has a slightly **lower packet loss** compared to OSPF, indicating that RIP is marginally better at successfully delivering packets.  
  - The difference, however, is minor (1.84%), and additional tests with different network conditions might show variability.


### **2. Hop Count (Average)**  
- **RIP**: Average hop count is **5.3038**.  
- **OSPF**: Average hop count is **5.24**.  
- **Comparison**:  
  - OSPF has a **slightly lower average hop count**, meaning packets travel through fewer routers on average.  
  - This might suggest that OSPF's routing decisions are slightly more efficient in terms of path selection compared to RIP.  


### **3. Waiting Cycles (Latency)**
- **RIP**:  
  - **Total waiting cycles**: **92,536**  
  - **Maximum waiting cycles**: **20.3**  
  - **Average waiting cycles**: **2.569**  
- **OSPF**:  
  - **Total waiting cycles**: **93,031**  
  - **Maximum waiting cycles**: **19.6**  
  - **Average waiting cycles**: **2.664**  
- **Comparison**:  
  - **RIP** has slightly **lower total waiting cycles** and **lower average waiting cycles**, which indicates it might process packets with less overall latency.  
  - However, **OSPF** has a marginally **lower maximum waiting cycle** (19.6 vs. 20.3), suggesting that OSPF might be better at avoiding extreme delays in some scenarios.  


### **4. Router Utilization**  
- **RIP**:  
  - **Unused Routers**: Consistently **4**, but different ones across runs.  
  - **Most Utilized Routers**: Routers with IDs **2, 3, 4, and 20**.  
- **OSPF**:  
  - **Unused Routers**: Consistently **6**, but different ones across runs.  
  - **Most Utilized Routers**: Same routers as in RIP: **2, 3, 4, and 20**.  
- **Comparison**:  
  - RIP appears to utilize more routers on average since fewer are consistently unused.  
  - In both protocols, the same 4 routers are highly utilized. This suggests a **bottleneck in the network topology**, where certain routers are preferred due to equal-cost paths or routing algorithms prioritizing them.



| **Metric**              | **RIP**                      | **OSPF**                     | **Better Protocol**  |
|--------------------------|------------------------------|------------------------------|----------------------|
| **Packet Loss**          | 39.96%                      | 41.8%                       | **RIP**              |
| **Hop Count (Avg)**      | 5.3038                      | 5.24                        | **OSPF**             |
| **Avg Waiting Cycles**   | 2.569                       | 2.664                       | **RIP**              |
| **Max Waiting Cycles**   | 20.3                        | 19.6                        | **OSPF**             |
| **Router Utilization**   | Fewer unused routers (4)     | More unused routers (6)      | **RIP**              |


### **Conclusion**  
- **RIP** performs better in terms of **packet delivery success**, **average waiting cycles**, and **router utilization**, making it more effective for reducing network bottlenecks in this simulation.  
- **OSPF**, however, shows advantages in **path efficiency (lower hop count)** and slightly **lower maximum latency**, indicating better path optimization and consistent performance in extreme conditions.  
- The choice between the two protocols would depend on specific network requirements.  
  - For networks where **packet delivery and utilization** are critical, **RIP** might be preferable.  
  - For networks prioritizing **path efficiency and handling extreme delays**, **OSPF** could be better.  
