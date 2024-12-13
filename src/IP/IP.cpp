#include "IP.h"
#include "QDebug"
AbstractIP::AbstractIP(QObject *parent)
    : QObject{parent}
    , m_ipValue(0)
{}

/**
 * ===========================================
 * ===========================================
 * ===========================================
 * @brief The IP class for IPv4.
 * ===========================================
 * ===========================================
 * ===========================================
 */

IP<UT::IPVersion::IPv4>::IP(QObject *parent) :
    AbstractIP(parent)
{
    m_ipValue = std::numeric_limits<uint64_t>::max();
}

IP<UT::IPVersion::IPv4>::IP(const QString &ipString, QObject *parent) :
    AbstractIP(parent)
{
    fromString(ipString);
}

IP<UT::IPVersion::IPv4>::IP(uint64_t ipValue, QObject *parent) :
    AbstractIP(parent)
{
    m_ipValue = ipValue;
}

IP<UT::IPVersion::IPv4>::~IP() {};

QString IP<UT::IPVersion::IPv4>::toString() const
{
    return QString("%1.%2.%3.%4")
        .arg((m_ipValue >> 24) & 0xFF)
        .arg((m_ipValue >> 16) & 0xFF)
        .arg((m_ipValue >> 8) & 0xFF)
        .arg(m_ipValue & 0xFF);
}

uint64_t IP<UT::IPVersion::IPv4>::toValue() const
{
    return m_ipValue;
}
void IP<UT::IPVersion::IPv4>::fromString(const QString &ipString)
{
    try {
        validateIPv4String(ipString);
        QStringList parts = ipString.split('.');
        m_ipValue = (static_cast<uint64_t>(parts[0].toUInt()) << 24)
                    | (static_cast<uint64_t>(parts[1].toUInt()) << 16)
                    | (static_cast<uint64_t>(parts[2].toUInt()) << 8)
                    | static_cast<uint64_t>(parts[3].toUInt());
    } catch (const std::exception &e) {
        qWarning() << "Error in fromString:" << e.what();
    }
}

void IP<UT::IPVersion::IPv4>::validateIPv4String(const QString &ipString) const
{
    QStringList parts = ipString.split('.');
    if (parts.size() != 4)
        throw std::invalid_argument("Invalid IPv4 string format.");

    for (const auto &part : parts) {
        bool ok;
        int value = part.toInt(&ok);
        if (!ok || value < 0 || value > 255)
            throw std::invalid_argument("IPv4 octet out of range.");
    }
}

/**
 * ===========================================
 * ===========================================
 * ===========================================
 * @brief The IP class for IPv6.
 * ===========================================
 * ===========================================
 * ===========================================
 */

// IP<UT::IPVersion::IPv6>::IP(QObject *parent) :
//     AbstractIP(parent)
// {
//     m_ipValue = std::numeric_limits<uint64_t>::max();
// }

// IP<UT::IPVersion::IPv6>::IP(const QString &ipString, QObject *parent) :
//     AbstractIP(parent)
// {
//     m_ipValue = std::numeric_limits<uint64_t>::max();
//     fromString(ipString);
//     toValueImpl();
// }

// IP<UT::IPVersion::IPv6>::IP(uint64_t ipValue, QObject *parent) :
//     AbstractIP(parent)
// {
//     m_ipValue = ipValue;
//     fromIPValue(ipValue);
//     toStringImpl();
// }

// IP<UT::IPVersion::IPv6>::~IP() {};
