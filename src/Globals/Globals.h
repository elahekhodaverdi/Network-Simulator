#ifndef GLOBALS_H
#define GLOBALS_H

#define INF std::numeric_limits<int>::infinity()

namespace UT
{

enum class RoutingProtocol { OSPF, RIP };

enum class IPVersion { IPv4, IPv6 };

enum class PacketType
{
    Data,
    Control
};

enum class PacketControlType
{
    Request,
    Response,
    Acknowledge,
    Error,
    DHCPDiscovery,
    DHCPOffer,
    DHCPRequest,
    DHCPAcknowledge,
    DHCPNak,
    RIP,
    OSPF,
};

enum class DistributionType
{
    Poisson,
    Pareto
};

enum class TopologyType
{
    Mesh,
    RingStar,
    Torus
};
}    // namespace UT

#endif    // GLOBALS_H
