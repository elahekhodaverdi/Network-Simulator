#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <array>
#include <cstdint>
#include <QString>
#include <QRegularExpression>


class MACAddress
{
public:
    MACAddress(std::array<uint8_t, 6> _address);
    MACAddress(QString _address);

    QString toString() const;
    bool isValid(const QString& address);
private:
    std::array<uint8_t, 6> addressBytes;
};

#endif // MACADDRESS_H
