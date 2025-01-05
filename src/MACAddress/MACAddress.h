#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <cstdint>
#include <QString>



class MACAddress
{
public:
    MACAddress();
    MACAddress(std::array<uint8_t, 6> address);
    MACAddress(QString address);
    MACAddress(const MACAddress& address);
    QString toString() const;
    bool isValid(const QString& address);
    bool operator==(const MACAddress& other) const;
    MACAddress& operator=(const MACAddress& other);
    ~MACAddress() = default;

private:
    std::array<uint8_t, 6> m_address;
};

#endif // MACADDRESS_H
