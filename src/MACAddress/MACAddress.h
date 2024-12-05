#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <cstdint>
#include <QString>



class MACAddress
{
public:
    MACAddress(std::array<uint8_t, 6> address);
    MACAddress(QString address);
    QString toString() const;
    bool isValid(const QString& address);
    bool operator==(const MACAddress& other) const;


private:
    std::array<uint8_t, 6> addressBytes;
};

#endif // MACADDRESS_H
